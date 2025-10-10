/**
 * @file Album.h/.cpp
 * @brief Media concreto di tipologia Album.
 * @author Thomas Morettin
 */

#include "Album.h"

namespace Model {
namespace Items {
Album::Album(const std::string &pthImg,
             const std::string &titolo,
             const std::string &data,
             const std::vector<std::string> &vGenere,
             const std::vector<std::string> &vAutore,
             const char *lingua,
             const std::string &descrizione,
             const unsigned int numCopie,

             const std::vector<std::string> &vCanzone,
             const std::vector<std::string> &vFeat,
             const unsigned int durata,
             const std::string &casa,
             const std::string &formato) :
    Media(pthImg, titolo, data, vGenere, vAutore, lingua, descrizione, numCopie), vCanzone(vCanzone), vFeat(vFeat), durata(durata), casa(casa), formato(formato) {}

std::string Album::getType() const { return "Album"; }

std::vector<std::string> Album::getVCanzone() const { return this->vCanzone; }
std::vector<std::string> Album::getVFeat() const { return this->vFeat; }
unsigned int Album::getDurata() const { return this->durata; }
std::string Album::getCasa() const { return this->casa; }
std::string Album::getFormato() const { return this->formato; }

void Album::setVCanzone(const std::vector<std::string> &vCanzone) { this->vCanzone = vCanzone; }
void Album::setVFeat(const std::vector<std::string> &vFeat) { this->vFeat = vFeat; }
void Album::setDurata(const unsigned int durata) { this->durata = durata; }
void Album::setCasa(const std::string &casa) { this->casa = casa; }
void Album::setFormato(const std::string &formato) { this->formato = formato; }

void Album::accept(Visitor::ConstVisitor &v) const { v.visit(*this); }
void Album::accept(Visitor::Visitor &v) { v.visit(*this); }

void Album::accept(Visitor::DesignVisitor &v) const { v.visitAlbum(); }
void Album::set(Visitor::View::MediaEditorVisitor &v) const { v.setAlbum(); }
}
}
