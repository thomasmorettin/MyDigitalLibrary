/**
 * @file Rivista.h/.cpp
 * @brief Media concreto di tipologia Rivista.
 * @author Thomas Morettin
 */

#include "Rivista.h"

namespace Model {
namespace Items {
Rivista::Rivista(const std::string &pthImg,
                 const std::string &titolo,
                 const std::string &data,
                 const std::vector<std::string> &vGenere,
                 const std::vector<std::string> &vAutore,
                 const char *lingua,
                 const std::string &descrizione,
                 unsigned int numCopie,

                 const unsigned int numPag,
                 const std::string &casa,
                 const std::string &tipo) :
    Media(pthImg, titolo, data, vGenere, vAutore, lingua, descrizione, numCopie), numPag(numPag), casa(casa), tipo(tipo) {}

std::string Rivista::getType() const { return "Rivista"; }

unsigned int Rivista::getNumPag() const { return this->numPag; }
std::string Rivista::getCasa() const { return this->casa; }
const std::string Rivista::getTipo() const { return this->tipo; }

void Rivista::setNumPag(const unsigned int numPag) { this->numPag = numPag; }
void Rivista::setCasa(const std::string &casa) { this->casa = casa; }
void Rivista::setTipo(const std::string &tipo) { this->tipo = tipo; }

void Rivista::accept(Visitor::ConstVisitor &v) const { v.visit(*this); }
void Rivista::accept(Visitor::Visitor &v) { v.visit(*this); }

void Rivista::accept(Visitor::DesignVisitor &v) const { v.visitRivista(); }
void Rivista::set(Visitor::View::MediaEditorVisitor &v) const { v.setRivista(); }
}
}
