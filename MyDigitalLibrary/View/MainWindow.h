/**
 * @file MainWindow.h/.cpp
 * @brief Classe (estende QMainWindow) che definisce l'organizzazione delle interfacce grafiche
 * dell'applicazione, tramite QStackedWidget vengono registrate le videate.
 * @author Thomas Morettin
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "FnsIniziale.h"
#include "FnsCatalogo.h"
#include "FnsDettagli.h"
#include "FrmEditor.h"
#include "../Model/MediaLibrary.h"

#include <QMainWindow>
#include <QStackedWidget>

namespace View {
class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Model::MediaLibrary *library;
    QPersistentModelIndex addedMedia;

    QStackedWidget *stkWidget;

    FnsIniziale *fnsInit;
    FnsCatalogo *fnsCat;
    FnsDettagli *fnsDet;
    FrmEditor *fnsEdit;
    FrmEditor *fnsAdd;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onOpenApp(Model::MediaLibrary *library);

    void showFnsCatalogo();
    void showFnsDettagli(const QPersistentModelIndex &index);
    void showFnsEditMedia(const QPersistentModelIndex &index);
    void showFnsAddMedia(const QPersistentModelIndex &index);

    void deleteMedia(const QPersistentModelIndex &index);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
}

#endif // MAINWINDOW_H
