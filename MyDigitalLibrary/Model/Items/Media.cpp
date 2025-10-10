#include "Media.h"

namespace Model {
namespace Items {
Media::Media(const std::string &pthImg,
             const std::string &titolo,
             const std::string &data,
             const std::vector<std::string> &vGenere,
             const std::vector<std::string> &vAutore,
             const char *lingua,
             const std::string &descrizione,
             const unsigned int numCopie) :
    pthImg(pthImg), titolo(titolo), data(data), vGenere(vGenere), vAutore(vAutore), descrizione(descrizione), numCopie(numCopie) {
    strncpy(this->lingua, lingua, sizeof(this->lingua) - 1);
    this->lingua[sizeof(this->lingua) - 1] = '\0';
}

std::string Media::getPthImg() const { return this->pthImg; }
std::string Media::getTitolo() const { return this->titolo; }
std::string Media::getData() const { return this->data; }
std::vector<std::string> Media::getVGenere() const { return this->vGenere; }
std::vector<std::string> Media::getVAutore() const { return this->vAutore; }
const char* Media::getLingua() const { return this->lingua; }
std::string Media::getDescrizione() const { return this->descrizione; }
unsigned int Media::getNumCopie() const { return this->numCopie; }

void Media::setPthImg(const std::string &pthImg) { this->pthImg = pthImg; }
void Media::setTitolo(const std::string &titolo) { this->titolo = titolo; }
void Media::setData(const std::string &data) { this->data = data; }
void Media::setVGenere(const std::vector<std::string> &vGenere) { this->vGenere = vGenere; }
void Media::setVAutore(const std::vector<std::string> &vAutore) { this->vAutore = vAutore; }
void Media::setLingua(const char *lingua) {
    strncpy(this->lingua, lingua, sizeof(this->lingua) - 1);
    this->lingua[sizeof(this->lingua) - 1] = '\0';
}
void Media::setDescrizione(const std::string &descrizione) { this->descrizione = descrizione; }
void Media::setNumCopie(const unsigned int numCopie) { this->numCopie = numCopie; }
}
}
