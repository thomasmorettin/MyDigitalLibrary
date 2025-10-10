/**
 * @file Film.h/.cpp
 * @brief Media concreto di tipologia Film.
 * @author Thomas Morettin
 */

#ifndef FILM_H
#define FILM_H

#include "Media.h"

namespace Model {
namespace Items {
class Film : public Media {
private:
    std::vector<std::string> vRegista;
    unsigned int durata;
    std::vector<std::string> vAttore;
    std::vector<std::string> vCasa;
    unsigned int num;
    std::string formato;

public:
    Film() = default;
    Film(const std::string &pthImg,
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
         const std::string &formato);
    ~Film() = default;

    std::string getType() const override;

    std::vector<std::string> getVRegista() const;
    unsigned int getDurata() const;
    std::vector<std::string> getVAttore() const;
    std::vector<std::string> getVCasa() const;
    unsigned int getNum() const;
    std::string getFormato() const;

    void setVRegista(const std::vector<std::string> &vRegista);
    void setDurata(const unsigned int durata);
    void setVAttore(const std::vector<std::string> &vAttore);
    void setVCasa(const std::vector<std::string> &vCasa);
    void setNum(const unsigned int num);
    void setFormato(const std::string &formato);

    void accept(Visitor::ConstVisitor &v) const override;
    void accept(Visitor::Visitor &v) override;

    void accept(Visitor::DesignVisitor &v) const override;
    void set(Visitor::View::MediaEditorVisitor &v) const override;
};
}
}

#endif // FILM_H
