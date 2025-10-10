/**
 * @file MainWindow.h/.cpp
 * @brief Classe (estende QMainWindow) che definisce l'organizzazione delle interfacce grafiche
 * dell'applicazione, tramite QStackedWidget vengono registrate le videate.
 * L'indice addedMedia viene utilizzato per memorizzare l'index dell'ultimo Media inserito vuoto
 * in quanto da dover inserire tutti i dati. Serve per non inserirlo all'interno del file .json
 * nel caso di una chiusura dell'applicazione dalla finestra di inserimento del nuovo Media.
 * @author Thomas Morettin
 */

#include "MainWindow.h"
#include "../MediaSerializer/JSON/JsonSerializer.h"

#include <QDir>
#include <QMessageBox>
#include <QCloseEvent>

namespace View {
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), library(nullptr), addedMedia(QPersistentModelIndex()),
    fnsCat(nullptr), fnsDet(nullptr), fnsEdit(nullptr), fnsAdd(nullptr) {
    this->setWindowIcon(QIcon(":Biblioteca.svg"));
    stkWidget = new QStackedWidget(this);

    fnsInit = new FnsIniziale(stkWidget);       // Istanza della finestra di inizializzazione della libreria
    connect(fnsInit, &FnsIniziale::exportLibrary, this, &MainWindow::onOpenApp);

    stkWidget->addWidget(fnsInit);
    setCentralWidget(stkWidget);
}

MainWindow::~MainWindow() { delete this->library; this->library = nullptr; }

void MainWindow::closeEvent(QCloseEvent *event) {
    if (!fnsInit && library->rowCount() > 0) {
        // Finestra di dialogo per salvataggio di file JSON per l'esportazione
        QString pthFile = QDir(DIR_SALVATAGGIO).filePath("Salvataggio.json");

        QMessageBox msgClose(this);
        msgClose.setWindowTitle("Chiusura applicazione");
        msgClose.setText("Sei sicuro di voler uscire dall'applicazione?\nLa libreria verrà salvata automaticamente in:\n" + pthFile);
        msgClose.setIcon(QMessageBox::Question);
        msgClose.setWindowIcon(this->windowIcon());
        QPushButton *btnSi = msgClose.addButton("Sì", QMessageBox::YesRole);
        msgClose.addButton("No", QMessageBox::RejectRole);

        msgClose.exec();       // Richiesta per la conferma di chiusura

        if (msgClose.clickedButton() == btnSi) {
            // Se il percorso non risulta vuoto
            if (!pthFile.isEmpty()) {
                // Funzione del JsonSerializer per esportazione dei Media dal Model
                if (!MediaSerializer::JSON::JsonSerializer::ExportJson(*library, pthFile, this->addedMedia)) {
                    // Finestra di attenzione per la fallita esportazione dei Media
                    QMessageBox::critical(
                        this,
                        "Esportazione fallita",
                        "Si è verificato un errore nell'esportazione.");
                }

                event->accept();
            }
        } else { event->ignore(); }     // Non viene chiusa l'applicazione
    }
}

void MainWindow::onOpenApp(Model::MediaLibrary *library) {
    this->library = library;
    delete fnsInit; fnsInit = nullptr;      // Eliminazione della finestra di inizializzazione

    // Istanza di tutte le finestre del layout e loro connessioni ai signal emessi da quest'ultime
    fnsCat = new FnsCatalogo(stkWidget, library);
    connect(fnsCat, &FnsCatalogo::showDettagli, this, &MainWindow::showFnsDettagli);
    connect(fnsCat, &FnsCatalogo::showAddMedia, this, &MainWindow::showFnsAddMedia);
    connect(fnsCat, &FnsCatalogo::showEditor, this, &MainWindow::showFnsEditMedia);

    fnsDet = new FnsDettagli(stkWidget, library);
    connect(fnsDet, &FnsDettagli::showCatalogo, this, &MainWindow::showFnsCatalogo);
    connect(fnsDet, &FnsDettagli::showEditor, this, &MainWindow::showFnsEditMedia);

    fnsEdit = new FrmEditor(stkWidget, library);
    connect(fnsEdit, &FrmEditor::showDettagli, this, &MainWindow::showFnsDettagli);
    connect(fnsEdit, &FrmEditor::onUnsave, this, &MainWindow::showFnsDettagli);

    fnsAdd = new FrmEditor(stkWidget, library);
    connect(fnsAdd, &FrmEditor::showDettagli, this, &MainWindow::showFnsDettagli);
    connect(fnsAdd, &FrmEditor::onUnsave, this, &MainWindow::deleteMedia);

    stkWidget->addWidget(fnsCat);
    stkWidget->addWidget(fnsDet);
    stkWidget->addWidget(fnsEdit);
    stkWidget->addWidget(fnsAdd);
}

void MainWindow::showFnsCatalogo() { stkWidget->setCurrentWidget(fnsCat); }

void MainWindow::showFnsDettagli(const QPersistentModelIndex &index) {
    this->addedMedia = QPersistentModelIndex();     // Viene cancellato l'indice vuoto nuovo, poiché si è usciti dall'inserimento del nuovo Media
    fnsDet->LoadMedia(index);
    stkWidget->setCurrentWidget(fnsDet);
}

void MainWindow::showFnsEditMedia(const QPersistentModelIndex &index) {
    fnsEdit->LoadMedia(index);
    stkWidget->setCurrentWidget(fnsEdit);
}

void MainWindow::showFnsAddMedia(const QPersistentModelIndex &index) {
    this->addedMedia = index;       // Viene memorizzato l'indice vuoto appena aggiunto al catalogo per il popolamento dei suoi dati
    fnsAdd->LoadMedia(index);
    stkWidget->setCurrentWidget(fnsAdd);
}

void MainWindow::deleteMedia(const QPersistentModelIndex &index) {
    library->deleteMedia(index.row());
    this->showFnsCatalogo();
}
}
