/**
 * @file MediaSetterVisitor.h/.cpp
 * @brief Classe Visitor utilizzata per settare il campo dato di un oggetto Media
 * in base al tipo di ruolo che viene richiesto dall'interfaccia.
 * Il metodo viene richiamato in base al tipo concreto dell'oggetto.
 * @author Thomas Morettin
 */

#ifndef MEDIASETTERVISITOR_H
#define MEDIASETTERVISITOR_H

#include "../Visitor.h"

#include <QVariant>

namespace Visitor {
namespace Items {
class MediaSetterVisitor : public Visitor {
private:
    int role;
    QVariant field;
    bool valid;

protected:
    void visitMedia(Model::Items::Media &media) override;

public:
    MediaSetterVisitor(const int role, const QVariant &field);
    ~MediaSetterVisitor() = default;

    bool getValid() const;

    void visit(Model::Items::Album &album) override;
    void visit(Model::Items::Film &film) override;
    void visit(Model::Items::Libro &libro) override;
    void visit(Model::Items::Rivista &rivista) override;
};
}
}

#endif // MEDIASETTERVISITOR_H
