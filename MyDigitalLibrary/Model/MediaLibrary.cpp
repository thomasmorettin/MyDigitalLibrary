/**
 * @file MediaLibrary.h/.cpp
 * @brief Controller - Classe (estende QAbstractListModel) per la memorizzazione e l'esecuzione
 * di tutte le operazioni su QList di Media.
 * @author Thomas Morettin
 */

#include "MediaLibrary.h"
#include "Items/Media.h"
#include "Items/Album.h"
#include "Items/Film.h"
#include "Items/Libro.h"
#include "Items/Rivista.h"
#include "../Visitor/Items/MediaGetterVisitor.h"
#include "../Visitor/Items/MediaSetterVisitor.h"
#include "../Visitor/JSON/ToJsonVisitor.h"

namespace Model {
MediaLibrary::MediaLibrary(QObject *parent) : QAbstractListModel(parent) {}
MediaLibrary::~MediaLibrary() { qDeleteAll(lstMedia); }     // Eliminazione di tutta la QList

int MediaLibrary::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)        // Macro specifica per il compilatore: non utilizzo della variabile
    return lstMedia.size();
}

/* La funzione fa uso di MediaGetterVisitor per la visita (visualizzazione) dei campi generici
 * e dei campi specifici del Media reale. */
QVariant MediaLibrary::data(const QModelIndex &index, int role) const {
    // Controllo sulla validità dell'indice
    if (!index.isValid() || index.row() >= lstMedia.count()) { return QVariant(); }

    const Items::Media *media = lstMedia.at(index.row());

    if (role == Qt::UserRole) {     // Controllo se viene richiesta la serializzazione di un Media in QJsonObject
        Visitor::JSON::ToJsonVisitor jsonVisitor;
        media->accept(jsonVisitor);
        return jsonVisitor.getObj();
    }

    // Visita del Media per restituzione del campo "role"
    Visitor::Items::MediaGetterVisitor visitor(role);
    media->accept(visitor);
    return visitor.getField();
}

/* La funzione fa uso di MediaSetterVisitor per la visita (modifica) dei campi generici
 * e dei campi specifici del Media reale. */
bool MediaLibrary::setData(const QModelIndex &index, const QVariant &field, int role) {
    if (!index.isValid() || index.row() >= lstMedia.size()) { return false; }

    Items::Media *media = lstMedia.at(index.row());

    // Visita del Media per modifica del campo "role"
    Visitor::Items::MediaSetterVisitor visitor(role, field);
    media->accept(visitor);

    if (!visitor.getValid()) { return false; }      // Caso in cui la modifica non è avvenuta con successo

    emit dataChanged(index, index, {role});     // Emissione del segnale di avvenuta modifica
    return true;
}

// La funzione ritorna un oggetto Media. Viene utilizzata per richiamare i Visitor.
Items::Media *MediaLibrary::getMedia(const QModelIndex &index) const {
    if (!index.isValid() || index.row() >= lstMedia.size()) { return nullptr; }
    return lstMedia.at(index.row());      // Restituzione del Media corrispondente
}

/* La funzione viene utilizzata per ritornare una QList di dati QModelIndex, rappresentati
 * i duplicati del Media da ricercare. */
QList<QModelIndex> MediaLibrary::findMediaLstIndex(const int row, const QString &titolo, const QString &lingua, const QString &tipo) const {
    QList<QModelIndex> lstIndex;

    for (int i = 0; i < lstMedia.size(); ++i) {
        if (i != row &&     // Il Media da analizzare non deve essere lo stesso per il quale si sta cercando il duplicato
            QString::fromStdString(lstMedia[i]->getType()).compare(tipo) == 0 &&
            QString::fromStdString(lstMedia[i]->getTitolo()).trimmed().compare(titolo.trimmed(), Qt::CaseInsensitive) == 0 &&
            lstMedia[i]->getLingua() == lingua) { lstIndex.append(this->index(i)); }
    }

    return lstIndex;
}

/* La funzione notifica le View dell'inserimento e aggiunge il Media alla lista.
 * Viene notificato solamente l'inserimento del Media corrente. */
void MediaLibrary::addNewMedia(Items::Media *media) {
    if (!media) { return; }

    beginInsertRows(QModelIndex(), lstMedia.size(), lstMedia.size());       // Notifica alle View di cambiamento del Model
    lstMedia.append(media);
    endInsertRows();        // Notifica alle View di termine del cambiamento
}

void MediaLibrary::changeNumCopie(const QModelIndex &index, const unsigned int copie) {
    unsigned int copieAtt = index.data(CopieRole).toInt();
    this->setData(index, copieAtt + copie, CopieRole);      // All'interno delle copie già presenti vengono sommate quelle nuove
    emit dataChanged(index, index, {CopieRole});
}

QModelIndex MediaLibrary::addMediaByType(const QString &tipo) {
    Model::Items::Media *newMedia = nullptr;

    if (tipo == "Album") { newMedia = new Items::Album(); }
    else if (tipo == "Film") { newMedia = new Items::Film(); }
    else if (tipo == "Libro") { newMedia = new Items::Libro(); }
    else if (tipo == "Rivista") { newMedia = new Items::Rivista(); }

    beginInsertRows(QModelIndex(), lstMedia.size(), lstMedia.size());       // Notifica alle View di cambiamento del Model
    lstMedia.append(newMedia);
    endInsertRows();        // Notifica alle View di termine del cambiamento

    return this->index(this->rowCount() - 1, 0);
}

/* La funzione notifica le View dell'eliminazione e rimuove il Media dalla lista.
 * Viene notificata solamente la cancellazione del Media corrente. */
void MediaLibrary::deleteMedia(const int row) {
    // Controllo sulla validità dell'indice
    if (row < 0 || row >= lstMedia.size()) { return; }

    beginRemoveRows(QModelIndex(), row, row);
    delete lstMedia.takeAt(row);
    endRemoveRows();
}

void MediaLibrary::eraseLibrary() {
    // Controllo sulla "validità" della lista
    if (lstMedia.isEmpty()) { return; }

    beginResetModel();      // Notifica alle view di reset del Model
    lstMedia.clear();
    endResetModel();
}

QList<QString> MediaLibrary::validateMedia(const QString &lingua,
                                           const QString &titolo,
                                           const QString &descrizione,
                                           const QString &data,
                                           const QString &generi,
                                           const QString &autori,
                                           const unsigned int copie) {
    QVector<QString> vctValori;     // QString da visualizzare per l'utente dei campi incompleti/errati

    if (lingua.isEmpty()) { vctValori.append("Lingua"); }
    if (titolo.isEmpty()) { vctValori.append("Titolo"); }
    if (descrizione.isEmpty()) { vctValori.append("Descrizione"); }
    QDate dataFormat = QDate::fromString(data, "dd/MM/yyyy");
    if (!dataFormat.isValid() || dataFormat > QDate::currentDate()) { vctValori.append("Data di pubblicazione"); }
    if (generi.isEmpty()) { vctValori.append("Generi"); }
    if (autori.isEmpty()) { vctValori.append("Autori"); }
    if (copie < 1) { vctValori.append("Copie a catalogo"); }

    return vctValori;
}

QList<QString> MediaLibrary::validateAlbum(const QString &lingua,
                                           const QString &titolo,
                                           const QString &descrizione,
                                           const QString &data,
                                           const QString &generi,
                                           const QString &autori,
                                           const unsigned int copie,
                                           const QString &canzoni,
                                           const unsigned int durata,
                                           const QString &casa) {
    // Ricezione dei campi già controllati generici
    QVector<QString> vctValori = this->validateMedia(lingua, titolo, descrizione, data, generi, autori, copie);

    if (canzoni.isEmpty()) { vctValori.append("Canzoni"); }
    if (durata < 5) { vctValori.append("Durata (min.)"); }
    if (casa.isEmpty()) { vctValori.append("Casa di prod."); }

    return vctValori;
}

QList<QString> MediaLibrary::validateFilm(const QString &lingua,
                                          const QString &titolo,
                                          const QString &descrizione,
                                          const QString &data,
                                          const QString &generi,
                                          const QString &autori,
                                          const unsigned int copie,
                                          const QString &registi,
                                          const unsigned int durata,
                                          const QString &caseProd,
                                          const unsigned int numero) {
    // Ricezione dei campi già controllati generici
    QVector<QString> vctValori = this->validateMedia(lingua, titolo, descrizione, data, generi, autori, copie);

    if (registi.isEmpty()) { vctValori.append("Registi"); }
    if (durata < 5) { vctValori.append("Durata (min.)"); }
    if (caseProd.isEmpty()) { vctValori.append("Case di prod."); }
    if (numero < 1) { vctValori.append("Numero nella serie"); }

    return vctValori;
}

QList<QString> MediaLibrary::validateLibro(const QString &lingua,
                                           const QString &titolo,
                                           const QString &descrizione,
                                           const QString &data,
                                           const QString &generi,
                                           const QString &autori,
                                           const unsigned int copie,
                                           const QString &isbn,
                                           const unsigned int numPag,
                                           const QString &casa) {
    // Ricezione dei campi già controllati generici
    QVector<QString> vctValori = this->validateMedia(lingua, titolo, descrizione, data, generi, autori, copie);

    QRegularExpression patternIsbn("^\\d{13}$");
    if (!patternIsbn.match(isbn).hasMatch()) { vctValori.append("ISBN"); }
    if (numPag < 5) { vctValori.append("Num. pagine"); }
    if (casa.isEmpty()) { vctValori.append("Casa editoriale"); }

    return vctValori;
}

QList<QString> MediaLibrary::validateRivista(const QString &lingua,
                                             const QString &titolo,
                                             const QString &descrizione,
                                             const QString &data,
                                             const QString &generi,
                                             const QString &autori,
                                             const unsigned int copie,
                                             const unsigned int numPag,
                                             const QString &casa) {
    // Ricezione dei campi già controllati generici
    QVector<QString> vctValori = this->validateMedia(lingua, titolo, descrizione, data, generi, autori, copie);

    if (numPag < 5) { vctValori.append("Num. pagine"); }
    if (casa.isEmpty()) { vctValori.append("Casa editoriale"); }

    return vctValori;
}
}
