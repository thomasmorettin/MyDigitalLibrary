/**
 * @file ToJsonVisitor.h/.cpp
 * @brief Classe Visitor utilizzata per creare un oggetto QJsonObject a partire
 * dall'oggetto Media che viene preso in considerazione nel ciclo for del serializer.
 * @author Thomas Morettin
 */

#include "ToJsonVisitor.h"
#include "../../Model/Items/Media.h"
#include "../../Model/Items/Album.h"
#include "../../Model/Items/Film.h"
#include "../../Model/Items/Libro.h"
#include "../../Model/Items/Rivista.h"

#include <QJsonArray>

namespace Visitor {
namespace JSON {
QJsonObject ToJsonVisitor::getObj() const { return this->obj; }

void ToJsonVisitor::visitMedia(const Model::Items::Media &media) {      // Funzione per la serializzazione dei campi generici del Media
    obj["pthImg"] = QString::fromStdString(media.getPthImg());
    obj["titolo"] = QString::fromStdString(media.getTitolo());
    obj["data"] = QString::fromStdString(media.getData());
    obj["lingua"] = QString::fromUtf8(media.getLingua());
    obj["descrizione"] = QString::fromStdString(media.getDescrizione());
    obj["numCopie"] = QString::number(media.getNumCopie());

    QJsonArray array;
    for (const std::string &str : media.getVGenere()) { array.append(QString::fromStdString(str)); }
    obj["generi"] = array;

    array = QJsonArray();
    for (const std::string &str : media.getVAutore()) { array.append(QString::fromStdString(str)); }
    obj["autori"] = array;
}

void ToJsonVisitor::visit(const Model::Items::Album &album) {
    this->visitMedia(album);        // Chiamata al setting dei campi generici del Media

    obj["media"] = "Album";
    obj["durata"] = QString::number(album.getDurata());
    obj["casa"] = QString::fromStdString(album.getCasa());
    obj["formato"] = QString::fromStdString(album.getFormato());

    QJsonArray array;
    for (const std::string &str : album.getVCanzone()) { array.append(QString::fromStdString(str)); }
    obj["canzoni"] = array;

    array = QJsonArray();
    for (const std::string &str : album.getVFeat()) { array.append(QString::fromStdString(str)); }
    obj["feats"] = array;
}

void ToJsonVisitor::visit(const Model::Items::Film &film) {
    this->visitMedia(film);        // Chiamata al setting dei campi generici del Media

    obj["media"] = "Film";
    obj["durata"] = QString::number(film.getDurata());
    obj["numero"] = QString::number(film.getNum());
    obj["formato"] = QString::fromStdString(film.getFormato());

    QJsonArray array;
    for (const std::string &str : film.getVRegista()) { array.append(QString::fromStdString(str)); }
    obj["registi"] = array;

    array = QJsonArray();
    for (const std::string &str : film.getVAttore()) { array.append(QString::fromStdString(str)); }
    obj["attori"] = array;

    array = QJsonArray();
    for (const std::string &str : film.getVCasa()) { array.append(QString::fromStdString(str)); }
    obj["case"] = array;
}

void ToJsonVisitor::visit(const Model::Items::Libro &libro) {
    this->visitMedia(libro);        // Chiamata al setting dei campi generici del Media

    obj["media"] = "Libro";
    obj["isbn"] = QString::fromUtf8(libro.getIsbn());
    obj["numPag"] = QString::number(libro.getNumPag());
    obj["casa"] = QString::fromStdString(libro.getCasa());
}

void ToJsonVisitor::visit(const Model::Items::Rivista &rivista) {
    this->visitMedia(rivista);        // Chiamata al setting dei campi generici del Media

    obj["media"] = "Rivista";
    obj["numPag"] = QString::number(rivista.getNumPag());
    obj["casa"] = QString::fromStdString(rivista.getCasa());
    obj["tipo"] = QString::fromStdString(rivista.getTipo());
}
}
}
