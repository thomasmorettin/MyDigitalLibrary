/**
 * @file ToJsonVisitor.h/.cpp
 * @brief Classe Visitor utilizzata per creare un oggetto QJsonObject a partire
 * dall'oggetto Media che viene preso in considerazione nel ciclo for del serializer.
 * @author Thomas Morettin
 */

#ifndef TOJSONVISITOR_H
#define TOJSONVISITOR_H

#include "../ConstVisitor.h"

#include <QJsonObject>

namespace Visitor {
namespace JSON {
class ToJsonVisitor : public ConstVisitor {
private:
    QJsonObject obj;

protected:
    void visitMedia(const Model::Items::Media &media) override;

public:
    ~ToJsonVisitor() = default;

    QJsonObject getObj() const;

    void visit(const Model::Items::Album &album) override;
    void visit(const Model::Items::Film &film) override;
    void visit(const Model::Items::Libro &libro) override;
    void visit(const Model::Items::Rivista &rivista) override;
};
}
}

#endif // TOJSONVISITOR_H
