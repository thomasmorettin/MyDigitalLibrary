/**
 * @file CheckFormatoVisitor.h/.cpp
 * @brief Classe Visitor utilizzata per confrontare il campo formato/tipo/ISBN di un Media
 * con il suo campo in base alla tipologia concreta di oggetto.
 * Il metodo viene richiamato in base al tipo concreto dell'oggetto.
 * @author Thomas Morettin
 */

#ifndef CHECKFORMATOVISITOR_H
#define CHECKFORMATOVISITOR_H

#include "../ConstVisitor.h"

#include <QString>

namespace Visitor {
namespace Items {
class CheckFormatoVisitor : public ConstVisitor {
private:
    bool ris;
    QString val;

protected:
    void visitMedia(const Model::Items::Media &media) override;

public:
    explicit CheckFormatoVisitor(const QString val);
    ~CheckFormatoVisitor() = default;

    bool getRis() const;

    void visit(const Model::Items::Album &album) override;
    void visit(const Model::Items::Film &film) override;
    void visit(const Model::Items::Libro &libro) override;
    void visit(const Model::Items::Rivista &rivista) override;
};
}
}

#endif // CHECKFORMATOVISITOR_H
