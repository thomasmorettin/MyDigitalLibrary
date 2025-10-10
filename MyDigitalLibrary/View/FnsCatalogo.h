/**
 * @file FnsCatalogo.h/.cpp
 * @brief Classe (estende QWidget) che definisce il layout e le funzioni logiche della
 * finestra di visualizzazione del catalogo.
 * @author Thomas Morettin
 */

#ifndef FNSCATALOGO_H
#define FNSCATALOGO_H

#include "../Model/MediaLibrary.h"
#include "../Model/MediaFilter.h"

#include <QWidget>
#include <QToolButton>
#include <QPushButton>
#include <QLineEdit>
#include <QListView>
#include <QMenu>
#include <QCheckBox>

namespace View {
class FnsCatalogo : public QWidget {
    Q_OBJECT

private:
    Model::MediaLibrary *library;
    Model::MediaFilter *fltModel;

    QToolButton *btnAdd;
    QMenu *menuAdd;
    QPushButton *btnImport;
    QPushButton *btnExport;
    QPushButton *btnSelect;
    QPushButton *btnDelete;
    QPushButton *btnErase;
    QLineEdit *txtSearch;

    QPushButton *btnShowFiltri;
    QCheckBox *chkAutore;
    QCheckBox *chkData;

    QListView *view;

    QPushButton *btnLibro;
    QPushButton *btnFilm;
    QPushButton *btnRivista;
    QPushButton *btnAlbum;

    void DeleteMedia(const QModelIndex &index);

protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    FnsCatalogo(QWidget *parent = nullptr, Model::MediaLibrary *library = nullptr);
    ~FnsCatalogo() = default;

private slots:
    void onExportJsonClicked();
    void onImportJsonClicked();
    void onCustomContextMenu(const QPoint &pos);
    void onFiltersChanged();
    void onEraseLibrary();
    void onMediaClicked(const QModelIndex &index);
    void onShowFiltri(const bool checked);
    void onAddNewMedia(const QAction *action);

signals:
    void showDettagli(const QPersistentModelIndex &index);
    void showAddMedia(const QPersistentModelIndex &index);
    void showEditor(const QPersistentModelIndex &index);
};
}

#endif // FNSCATALOGO_H
