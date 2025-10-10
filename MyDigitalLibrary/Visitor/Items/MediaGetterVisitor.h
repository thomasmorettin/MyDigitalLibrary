/**
 * @file MediaGetterVisitor.h/.cpp
 * @brief Classe Visitor utilizzata per restituire il campo dato di un oggetto Media
 * in base al tipo di ruolo che viene richiesto dall'interfaccia.
 * Il metodo viene richiamato in base al tipo concreto dell'oggetto.
 * @author Thomas Morettin
 */

#ifndef MEDIAGETTERVISITOR_H
#define MEDIAGETTERVISITOR_H

#include "../ConstVisitor.h"

#include <QVariant>

namespace Visitor {
namespace Items {
class MediaGetterVisitor : public ConstVisitor {
private:
    int role;
    QVariant field;

protected:
    void visitMedia(const Model::Items::Media &media) override;

public:
    explicit MediaGetterVisitor(const int role);
    ~MediaGetterVisitor() = default;

    QVariant getField() const;

    void visit(const Model::Items::Album &album) override;
    void visit(const Model::Items::Film &film) override;
    void visit(const Model::Items::Libro &libro) override;
    void visit(const Model::Items::Rivista &rivista) override;
};
}
}

#endif // MEDIAGETTERVISITOR_H
