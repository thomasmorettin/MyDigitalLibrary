/**
 * @file DesignVisitor.h/.cpp
 * @brief Classe Visitor base astratta utilizzata per istanziare un Visitor per
 * manipolare i QWidget per visualizzazione/inserimento dei campi del Media.
 * @author Thomas Morettin
 */

#ifndef DESIGNVISITOR_H
#define DESIGNVISITOR_H

#include <QWidget>
#include <QGridLayout>
#include <QModelIndex>

namespace Visitor {
class DesignVisitor : public QWidget {
    Q_OBJECT

protected:
    QPersistentModelIndex index;

    // Parametri ereditati dalle classi concrete per popolarli con i campi QLabel/QTextEdit/...
    QWidget *cntInfo;
    QGridLayout *layCntInfo;

    DesignVisitor();

    virtual void visitMedia() = 0;

public:
    virtual ~DesignVisitor();

    void setIndex(const QPersistentModelIndex &index);      // Funzione per il setting di default dell'index
    QWidget *getInfo() const;

    virtual void clear() = 0;       // Funzione per la pulizia dei campi specifichi

    // Funzioni pure astratte per Media specifici
    virtual void visitAlbum() = 0;
    virtual void visitFilm() = 0;
    virtual void visitLibro() = 0;
    virtual void visitRivista() = 0;
};
}

#endif // DESIGNVISITOR_H
