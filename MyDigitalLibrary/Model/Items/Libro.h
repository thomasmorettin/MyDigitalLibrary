/**
 * @file Libro.h/.cpp
 * @brief Media concreto di tipologia Libro.
 * @author Thomas Morettin
 */

#ifndef LIBRO_H
#define LIBRO_H

#include "Media.h"

namespace Model {
namespace Items {
class Libro : public Media {
private:
    char isbn[14];      // ISBN a 13 numeri + '\0' (es. "9788831901710\0")
    unsigned int numPag;
    std::string casa;

public:
    Libro() = default;
    Libro(const std::string &pthImg,
          const std::string &titolo,
          const std::string &data,
          const std::vector<std::string> &vGenere,
          const std::vector<std::string> &vAutore,
          const char *lingua,
          const std::string &descrizione,
          unsigned int numCopie,

          const char *isbn,
          const unsigned int numPag,
          const std::string &casa);
    ~Libro() = default;

    std::string getType() const override;

    const char* getIsbn() const;
    unsigned int getNumPag() const;
    std::string getCasa() const;

    void setIsbn(const char *isbn);
    void setNumPag(const unsigned int numPag);
    void setCasa(const std::string &casa);

    void accept(Visitor::ConstVisitor &v) const override;
    void accept(Visitor::Visitor &v) override;

    void accept(Visitor::DesignVisitor &v) const override;
    void set(Visitor::View::MediaEditorVisitor &v) const override;
};
}
}

#endif // LIBRO_H
