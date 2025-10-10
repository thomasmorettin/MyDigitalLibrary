/**
 * @file JsonSerializer.h/.cpp
 * @brief Classe per la serializzazione del file .json, in quanto vengono implementate le
 * funzioni statiche per l'importazione e l'esportazione. Il controllo sul parsing del file
 * viene effettuato in automatico.
 * @author Thomas Morettin
 */

#include "JsonSerializer.h"
#include "../../Factory/MediaFactory.h"
#include "Visitor/Items/CheckFormatoVisitor.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

namespace MediaSerializer {
namespace JSON {
bool JsonSerializer::ExportJson(const Model::MediaLibrary &library,
                                const QString &pthFile, const QPersistentModelIndex &addedMedia) {
    QJsonArray jsonArray;
    for (int row = 0; row < library.rowCount(); ++row) {        // Scorrimento della libreria
        QModelIndex index = library.index(row, 0);

        // Controllo se il Media in oggetto corrisponde al Media che si sta inserendo nella schermata di inserimento (sarebbe un Media vuoto)
        if (addedMedia != index) {
            QJsonObject obj = index.data(Qt::UserRole).toJsonObject();      // Il ruolo UserRole determina l'operazione di creazione di un QJsonObject per il Media
            jsonArray.append(obj);
        }
    }

    QJsonDocument jsDoc(jsonArray);
    QFile file(pthFile);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {        // Apertura del file in scrittura di testo
        file.write(jsDoc.toJson(QJsonDocument::Indented));      // Indentazione del file
        file.close();
        return true;
    }

    return false;
}

bool JsonSerializer::ImportJson(Model::MediaLibrary &library, const QString &pthFile, unsigned int &counter, unsigned int &total) {
    int found = false;
    QFile file(pthFile);
    QString ifFormato;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {     // Apertura del file in lettura di testo
        QByteArray data = file.readAll();
        file.close();

        QJsonParseError parser;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parser);

        // Controllo sulla correttezza del parsing
        if (parser.error == QJsonParseError::NoError && doc.isArray()) {
            QJsonArray array = doc.array();

            for (const QJsonValue &js : array) {        // Scorrimento di ciascun QJsonValue (Media)
                if (js.isObject()) {
                    QJsonObject obj = js.toObject();

                    // Controllo se la creazione è andata a buon fine
                    if (Model::Items::Media *media = Factory::MediaFactory::MediaFromJson(obj, ifFormato)) {
                        // Rintracciamento di tutti i Media "uguali" (potrebbero essere > 1 per via del formato/tipo/ISBN differente)
                        QList<QModelIndex> lstIndex = library.findMediaLstIndex(-1, QString::fromStdString(media->getTitolo()),
                                                                   QString::fromStdString(media->getLingua()),
                                                                   QString::fromStdString(media->getType()));

                        if (!lstIndex.isEmpty()) {      // Se non esistono duplicati
                            for (const QModelIndex &index : lstIndex) {
                                Visitor::Items::CheckFormatoVisitor visitor(ifFormato);     // Visitor di confronto del formato
                                library.getMedia(index)->accept(visitor);

                                if (visitor.getRis()) {     // Esiste il duplicato
                                    library.changeNumCopie(index, media->getNumCopie());        // Aggiornamento copie, Media vecchio
                                    found = true;
                                    delete media;
                                }
                            } if (!found) { library.addNewMedia(media); }
                        } else { library.addNewMedia(media); }

                        counter++;
                    }

                    total++;
                } else { return false; }
            }

            return true;
        }
    }

    return false;
}
}
}
