/**
 * @file CheckFormatoVisitor.h/.cpp
 * @brief Classe Visitor utilizzata per confrontare il campo formato/tipo/ISBN di un Media
 * con il suo campo in base alla tipologia concreta di oggetto.
 * Il metodo viene richiamato in base al tipo concreto dell'oggetto.
 * @author Thomas Morettin
 */

#include "CheckFormatoVisitor.h"
#include "../../Model/Items/Media.h"
#include "../../Model/Items/Album.h"
#include "../../Model/Items/Film.h"
#include "../../Model/Items/Libro.h"
#include "../../Model/Items/Rivista.h"

namespace Visitor {
namespace Items {
CheckFormatoVisitor::CheckFormatoVisitor(const QString val) : ris(false), val(val) {}

bool CheckFormatoVisitor::getRis() const { return this->ris; }      // Funzione che ritorna il risultato del confronto, se i valori sono uguali o meno

void CheckFormatoVisitor::visitMedia(const Model::Items::Media &media) { Q_UNUSED(media) }
void CheckFormatoVisitor::visit(const Model::Items::Album &album) { if (QString::fromStdString(album.getFormato()).compare(val) == 0) { this->ris = true; } }
void CheckFormatoVisitor::visit(const Model::Items::Film &film) { if (QString::fromStdString(film.getFormato()).compare(val) == 0) { this->ris = true; } }
void CheckFormatoVisitor::visit(const Model::Items::Libro &libro) { if (QString::fromUtf8(libro.getIsbn()).compare(val) == 0) { this->ris = true; } }
void CheckFormatoVisitor::visit(const Model::Items::Rivista &rivista) { if (QString::fromStdString(rivista.getTipo()).compare(val) == 0) { this->ris = true; } }

}
}
