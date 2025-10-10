/**
 * @file Visitor.h/.cpp
 * @brief Classe Visitor base astratta utilizzata per istanziare un Visitor per
 * oggetti non costanti.
 * @author Thomas Morettin
 */

#ifndef VISITOR_H
#define VISITOR_H

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
class Visitor {
protected:
    virtual void visitMedia(Model::Items::Media &media) = 0;

public:
    virtual ~Visitor() = default;

    // Funzioni pure astratte per campi specifici
    virtual void visit(Model::Items::Album &album) = 0;
    virtual void visit(Model::Items::Film &film) = 0;
    virtual void visit(Model::Items::Libro &libro) = 0;
    virtual void visit(Model::Items::Rivista &rivista) = 0;
};
}

#endif // VISITOR_H
