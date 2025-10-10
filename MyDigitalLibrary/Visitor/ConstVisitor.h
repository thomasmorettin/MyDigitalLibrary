/**
 * @file ConstVisitor.h/.cpp
 * @brief Classe Visitor base astratta utilizzata per istanziare un Visitor per
 * oggetti costanti.
 * @author Thomas Morettin
 */

#ifndef CONSTVISITOR_H
#define CONSTVISITOR_H

namespace Model {
namespace Items {
class Media;
class Album;
class Film;
class Libro;
class Rivista;
}
}

namespace Visitor {
class ConstVisitor {
protected:
    virtual void visitMedia(const Model::Items::Media &media) = 0;

public:
    virtual ~ConstVisitor() = default;

    // Funzioni pure astratte per campi specifici
    virtual void visit(const Model::Items::Album &album) = 0;
    virtual void visit(const Model::Items::Film &film) = 0;
    virtual void visit(const Model::Items::Libro &libro) = 0;
    virtual void visit(const Model::Items::Rivista &rivista) = 0;
};
}

#endif // CONSTVISITOR_H
