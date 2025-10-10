#include "Libro.h"

namespace Model {
namespace Items {
Libro::Libro(const std::string &pthImg,
             const std::string &titolo,
             const std::string &data,
             const std::vector<std::string> &vGenere,
             const std::vector<std::string> &vAutore,
             const char *lingua,
             const std::string &descrizione,
             unsigned int numCopie,

             const char *isbn,
             const unsigned int numPag,
             const std::string &casa) :
    Media(pthImg, titolo, data, vGenere, vAutore, lingua, descrizione, numCopie), numPag(numPag), casa(casa) {
    strncpy(this->isbn, isbn, sizeof(this->isbn) - 1);
    this->isbn[sizeof(this->isbn) - 1] = '\0';
}

std::string Libro::getType() const { return "Libro"; }

const char* Libro::getIsbn() const { return this->isbn; }
unsigned int Libro::getNumPag() const { return this->numPag; }
std::string Libro::getCasa() const { return this->casa; }

void Libro::setIsbn(const char *isbn) {
    strncpy(this->isbn, isbn, sizeof(this->isbn) - 1);
    this->isbn[sizeof(this->isbn) - 1] = '\0';
}
void Libro::setNumPag(const unsigned int numPag) { this->numPag = numPag; }
void Libro::setCasa(const std::string &casa) { this->casa = casa; }

void Libro::accept(Visitor::ConstVisitor &v) const { v.visit(*this); }
void Libro::accept(Visitor::Visitor &v) { v.visit(*this); }

void Libro::accept(Visitor::DesignVisitor &v) const { v.visitLibro(); }
void Libro::set(Visitor::View::MediaEditorVisitor &v) const { v.setLibro(); }
}
}
