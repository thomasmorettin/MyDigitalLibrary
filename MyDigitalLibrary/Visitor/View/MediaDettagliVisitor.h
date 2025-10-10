/**
 * @file MediaDettagliVisitor.h/.cpp
 * @brief Classe Visitor utilizzata per creare gli oggetti QWidget per la visualizzazione
 * dei dettagli generici e specifici in base al tipo concreto di Media selezionato dall'utente.
 * @author Thomas Morettin
 */

#ifndef MEDIADETTAGLIVISITOR_H
#define MEDIADETTAGLIVISITOR_H

#include "../DesignVisitor.h"

#include <QLabel>

namespace Visitor {
namespace View {
class MediaDettagliVisitor : public DesignVisitor {
private:
    QLabel *lblLingua;
    QLabel *lblTitolo;
    QLabel *lblImg;
    QLabel *lblData;
    QLabel *lblGeneri;
    QLabel *lblAutori;
    QLabel *lblDescrizione;
    QLabel *lblCopie;

    QWidget *cntSpecific;
    QLabel *lbl1;
    QLabel *lbl2;
    QLabel *lbl3;
    QLabel *lbl4;
    QLabel *lbl5;
    QLabel *lbl6;

protected:
    void visitMedia() override;

public:
    MediaDettagliVisitor();
    ~MediaDettagliVisitor() = default;

    void clear() override;      // Funzione per la pulizia del QWidget contenente i campi specifici del Media

    void visitAlbum() override;
    void visitFilm() override;
    void visitLibro() override;
    void visitRivista() override;
};
}
}

#endif // MEDIADETTAGLIVISITOR_H
