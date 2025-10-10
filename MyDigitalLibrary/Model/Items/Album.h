/**
 * @file Album.h/.cpp
 * @brief Media concreto di tipologia Album.
 * @author Thomas Morettin
 */

#ifndef ALBUM_H
#define ALBUM_H

#include "Media.h"

namespace Model {
namespace Items {
class Album : public Media {
private:
    std::vector<std::string> vCanzone;
    std::vector<std::string> vFeat;
    unsigned int durata;
    std::string casa;
    std::string formato;

public:
    Album() = default;
    Album(const std::string &pthImg,
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
          const std::string &formato);
    ~Album() = default;

    std::string getType() const override;

    std::vector<std::string> getVCanzone() const;
    std::vector<std::string> getVFeat() const;
    unsigned int getDurata() const;
    std::string getCasa() const;
    std::string getFormato() const;

    void setVCanzone(const std::vector<std::string> &vCanzone);
    void setVFeat(const std::vector<std::string> &vFeat);
    void setDurata(const unsigned int durata);
    void setCasa(const std::string &casa);
    void setFormato(const std::string &formato);

    // Metodi per la gestione del Visitor
    void accept(Visitor::ConstVisitor &v) const override;
    void accept(Visitor::Visitor &v) override;

    void accept(Visitor::DesignVisitor &v) const override;
    void set(Visitor::View::MediaEditorVisitor &v) const override;
};
}
}

#endif // ALBUM_H
