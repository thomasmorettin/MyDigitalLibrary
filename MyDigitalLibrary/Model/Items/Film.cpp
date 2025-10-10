/**
 * @file Film.h/.cpp
 * @brief Media concreto di tipologia Film.
 * @author Thomas Morettin
 */

#include "Film.h"

namespace Model {
namespace Items {
Film::Film(const std::string &pthImg,
           const std::string &titolo,
           const std::string &data,
           const std::vector<std::string> &vGenere,
           const std::vector<std::string> &vAutore,
           const char *lingua,
           const std::string &descrizione,
           const unsigned int numCopie,

           const std::vector<std::string> &vRegista,
           const unsigned int durata,
           const std::vector<std::string> &vAttore,
           const std::vector<std::string> &vCasa,
           const unsigned int num,
           const std::string &formato) :
    Media(pthImg, titolo, data, vGenere, vAutore, lingua, descrizione, numCopie), vRegista(vRegista), durata(durata), vAttore(vAttore), vCasa(vCasa), num(num), formato(formato) {}

std::string Film::getType() const { return "Film"; }

std::vector<std::string> Film::getVRegista() const { return this->vRegista; }
unsigned int Film::getDurata() const { return this->durata; }
std::vector<std::string> Film::getVAttore() const { return this->vAttore; }
std::vector<std::string> Film::getVCasa() const { return this->vCasa; }
unsigned int Film::getNum() const { return this->num; }
std::string Film::getFormato() const { return this->formato; }

void Film::setVRegista(const std::vector<std::string> &vRegista) { this->vRegista = vRegista; }
void Film::setDurata(const unsigned int durata) { this->durata = durata; }
void Film::setVAttore(const std::vector<std::string> &vAttore) { this->vAttore = vAttore; }
void Film::setVCasa(const std::vector<std::string> &vCasa) { this->vCasa = vCasa; }
void Film::setNum(const unsigned int num) { this->num = num; }
void Film::setFormato(const std::string &formato) {this->formato = formato; }

void Film::accept(Visitor::ConstVisitor &v) const { v.visit(*this); }
void Film::accept(Visitor::Visitor &v) { v.visit(*this); }

void Film::accept(Visitor::DesignVisitor &v) const { v.visitFilm(); }
void Film::set(Visitor::View::MediaEditorVisitor &v) const { v.setFilm(); }
}
}
