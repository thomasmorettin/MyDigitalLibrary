/**
 * @file Rivista.h/.cpp
 * @brief Media concreto di tipologia Rivista.
 * @author Thomas Morettin
 */

#ifndef RIVISTA_H
#define RIVISTA_H

#include "Media.h"

namespace Model {
namespace Items {
class Rivista : public Media {
private:
    unsigned int numPag;
    std::string casa;
    std::string tipo;

public:
    Rivista() = default;
    Rivista(const std::string &pthImg,
            const std::string &titolo,
            const std::string &data,
            const std::vector<std::string> &vGenere,
            const std::vector<std::string> &vAutore,
            const char *lingua,
            const std::string &descrizione,
            unsigned int numCopie,

            const unsigned int numPag,
            const std::string &casa,
            const std::string &tipo);
    ~Rivista() = default;

    std::string getType() const override;

    unsigned int getNumPag() const;
    std::string getCasa() const;
    const std::string getTipo() const;

    void setNumPag(const unsigned int numPag);
    void setCasa(const std::string &casa);
    void setTipo(const std::string &tipo);

    void accept(Visitor::ConstVisitor &v) const override;
    void accept(Visitor::Visitor &v) override;

    void accept(Visitor::DesignVisitor &v) const override;
    void set(Visitor::View::MediaEditorVisitor &v) const override;
};
}
}

#endif // RIVISTA_H
