/**
 * @file FrmEditor.h/.cpp
 * @brief Classe (estende QWidget) che definisce il layout e le funzioni logiche della
 * finestra di modifica dei dati del Media. Per manutenibilità ed efficienza la classe
 * viene utilizzata sia per la creazione di un nuovo Media che per la sua modifica.
 * @author Thomas Morettin
 */

#ifndef FRMEDITOR_H
#define FRMEDITOR_H

#include "../Model/MediaLibrary.h"
#include "../Visitor/View/MediaEditorVisitor.h"

#include <QWidget>
#include <QPushButton>
#include <QPersistentModelIndex>

namespace View {
class FrmEditor : public QWidget {
    Q_OBJECT

private:
    Model::MediaLibrary *library;
    QPersistentModelIndex index;

    QVector<QString> vctValori;     // QVector per la memorizzazione dei dati errati/mancanti nel form

    QPushButton *btnUnsave;
    QPushButton *btnReset;
    QPushButton *btnSave;

    Visitor::View::MediaEditorVisitor *visitor;

public:
    FrmEditor(QWidget *parent = nullptr, Model::MediaLibrary *library = nullptr);
    ~FrmEditor() = default;

    void LoadMedia(const QPersistentModelIndex &index);

signals:
    void showDettagli(const QPersistentModelIndex &index);
    void onUnsave(const QPersistentModelIndex &index);
};
}

#endif // FRMEDITOR_H
