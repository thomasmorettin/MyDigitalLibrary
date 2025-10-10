/**
 * @file Media.h/.cpp
 * @brief Classe base astratta da cui ereditano i Media concreti.
 * @author Thomas Morettin
 */

#ifndef MEDIA_H
#define MEDIA_H

#include "Visitor/ConstVisitor.h"
#include "Visitor/Visitor.h"
#include "Visitor/View/MediaEditorVisitor.h"

#include <string>
#include <cstring>
#include <vector>

namespace Model {
namespace Items {
class Media {
private:
    std::string pthImg;
    std::string titolo;
    std::string data;
    std::vector<std::string> vGenere;
    std::vector<std::string> vAutore;
    char lingua[3];     // Sigla a 2 lettere + '\0' (es. "IT\0")
    std::string descrizione;
    unsigned int numCopie;

protected:
    Media(const std::string &pthImg,
          const std::string &titolo,
          const std::string &data,
          const std::vector<std::string> &vGenere,
          const std::vector<std::string> &vAutore,
          const char *lingua,
          const std::string &descrizione,
          const unsigned int numCopie);

public:
    Media() = default;
    virtual ~Media() = default;

    virtual std::string getType() const = 0;

    std::string getPthImg() const;
    std::string getTitolo() const;
    std::string getData() const;
    std::vector<std::string> getVGenere() const;
    std::vector<std::string> getVAutore() const;
    const char *getLingua() const;
    std::string getDescrizione() const;
    unsigned int getNumCopie() const;

    void setPthImg(const std::string &pthImg);
    void setTitolo(const std::string &titolo);
    void setData(const std::string &data);
    void setVGenere(const std::vector<std::string> &vGenere);
    void setVAutore(const std::vector<std::string> &vAutore);
    void setLingua(const char *lingua);
    void setDescrizione(const std::string &descrizione);
    void setNumCopie(const unsigned int numCopie);

    virtual void accept(Visitor::ConstVisitor &v) const = 0;
    virtual void accept(Visitor::Visitor &v) = 0;

    virtual void accept(Visitor::DesignVisitor &v) const = 0;
    virtual void set(Visitor::View::MediaEditorVisitor &v) const = 0;
};
}
}

#endif // MEDIA_H
