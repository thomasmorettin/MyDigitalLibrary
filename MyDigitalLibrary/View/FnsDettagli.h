/**
 * @file FnsDettagli.h/.cpp
 * @brief Classe (estende QWidget) che definisce il layout e le funzioni logiche della
 * finestra di visualizzazione dei dettagli di ciascun Media. Fa uso del MediaDettagliVisitor
 * per il popolamento dei campi generici/specifici per ciascun Media.
 * @author Thomas Morettin
 */

#ifndef FNSDETTAGLI_H
#define FNSDETTAGLI_H

#include "../Model/MediaLibrary.h"
#include "../Visitor/View/MediaDettagliVisitor.h"

#include <QWidget>
#include <QPushButton>

namespace View {
class FnsDettagli : public QWidget {
    Q_OBJECT

private:
    Model::MediaLibrary *library;
    QPersistentModelIndex index;        // Memorizzazione dell'index selezionato dall'utente

    QPushButton *btnBack;
    QPushButton *btnDelete;
    QPushButton *btnEdit;

    Visitor::View::MediaDettagliVisitor *visitor;       // Visitor per visualizzare i dettagli

public:
    FnsDettagli(QWidget *parent = nullptr, Model::MediaLibrary *library = nullptr);
    ~FnsDettagli() = default;

    void LoadMedia(const QPersistentModelIndex &index);

public slots:
    void onDeleteClicked();

signals:
    void showCatalogo();
    void showEditor(const QPersistentModelIndex &index);
};
}

#endif // FNSDETTAGLI_H
