/**
 * @file FnsIniziale.h/.cpp
 * @brief Classe (estende QWidget) che definisce il layout e le funzioni logiche della
 * finestra di inizializzazione della libreria.
 * @author Thomas Morettin
 */

#include "FnsIniziale.h"
#include "../MediaSerializer/JSON/JsonSerializer.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QDesktopServices>
#include <QMessageBox>
#include <QFileDialog>

namespace View {
FnsIniziale::FnsIniziale(QWidget *parent) : QWidget(parent) {
    this->setMinimumSize(350, 500);
    QGridLayout *layFnsIniziale = new QGridLayout(this);
    layFnsIniziale->setContentsMargins(0, 0, 0, 0);

    // Centering degli elementi grafici
    layFnsIniziale->setColumnStretch(0, 1);
    layFnsIniziale->setColumnStretch(2, 1);

    QWidget *container = new QWidget(this);
    QVBoxLayout *layContainer = new QVBoxLayout(container);

    layContainer->addStretch();

    // Box di informazioni: logo, titolo e sottotitolo
    QIcon icnLogo(":Biblioteca.svg");
    QPixmap pxmLogo = icnLogo.pixmap(45, 45);
    QLabel *lblLogo = new QLabel(container);
    lblLogo->setPixmap(pxmLogo);
    lblLogo->setFixedSize(QSize(pxmLogo.size()));
    layContainer->addWidget(lblLogo, 0, Qt::AlignCenter);

    QLabel *lblTitolo = new QLabel(container);
    lblTitolo->setText("MyDigitalLibrary");
    lblTitolo->setObjectName("titoloApp");
    layContainer->addWidget(lblTitolo, 0, Qt::AlignCenter);

    QLabel *lblSottotitolo = new QLabel(container);
    lblSottotitolo->setAlignment(Qt::AlignCenter);
    lblSottotitolo->setObjectName("sottotitoloApp");
    lblSottotitolo->setText("La tua libreria per:\nLibri, Film, Album e Riviste");
    layContainer->addWidget(lblSottotitolo, 0, Qt::AlignCenter);

    layContainer->addStretch();

    QLabel *lblInfo = new QLabel(container);
    lblInfo->setAlignment(Qt::AlignCenter);
    lblInfo->setText("Scegli come iniziare");
    layContainer->addWidget(lblInfo, 0, Qt::AlignCenter);

    // QPushButton per la creazione di una libreria vuota
    QPushButton *btnCleanLibrary = new QPushButton(container);
    QHBoxLayout *lay1 = new QHBoxLayout(btnCleanLibrary);
    QLabel *lblIcon1 = new QLabel(btnCleanLibrary);
    lblIcon1->setPixmap(QIcon(":FnsIniziale/Book.svg").pixmap(17, 20));
    lay1->addWidget(lblIcon1, 0, Qt::AlignLeft);
    lay1->addStretch();
    QLabel *lblText1 = new QLabel(btnCleanLibrary);
    lblText1->setText("Libreria vuota");
    lay1->addWidget(lblText1);
    lay1->addStretch();
    btnCleanLibrary->setFixedSize(200, 35);
    btnCleanLibrary->setCursor(Qt::PointingHandCursor);
    // Connessione tra il click del QPushButton e lo slot per la creazione della libreria vuota
    connect(btnCleanLibrary, &QPushButton::clicked, this, &FnsIniziale::onCleanLibrary);
    layContainer->addWidget(btnCleanLibrary, 0, Qt::AlignCenter);

    // QPushButton per la creazione di una libreria a partire da file .json
    QPushButton *btnJson = new QPushButton(container);
    QHBoxLayout *lay2 = new QHBoxLayout(btnJson);
    QLabel *lblIcon2 = new QLabel(btnJson);
    lblIcon2->setPixmap(QIcon(":FnsIniziale/File.svg").pixmap(17, 20));
    lay2->addWidget(lblIcon2, 0, Qt::AlignLeft);
    lay2->addStretch();
    QLabel *lblText2 = new QLabel(btnJson);
    lblText2->setText("File .json");
    lay2->addWidget(lblText2);
    lay2->addStretch();
    btnJson->setFixedSize(200, 35);
    btnJson->setCursor(Qt::PointingHandCursor);
    // Connessione tra il click del QPushButton e lo slot per la creazione della libreria a partire da file .json
    connect(btnJson, &QPushButton::clicked, this, &FnsIniziale::onJsonLibrary);
    layContainer->addWidget(btnJson, 0, Qt::AlignCenter);

    // QPushButton per la creazione di una libreria a partire da un database remoto (funzinalità da sviluppare in futuro)
    QPushButton *btnDatabase = new QPushButton(container);
    QHBoxLayout *lay3 = new QHBoxLayout(btnDatabase);
    QLabel *lblIcon3 = new QLabel(btnDatabase);
    lblIcon3->setPixmap(QIcon(":FnsIniziale/Database.svg").pixmap(17, 20));
    lay3->addWidget(lblIcon3, 0, Qt::AlignLeft);
    lay3->addStretch();
    QLabel *lblText3 = new QLabel(btnDatabase);
    lblText3->setText("<span style='color: #979AA0;'>Database remoto<br><span style='font-size: 10px;'>Coming soon</span></span>");
    lblText3->setAlignment(Qt::AlignCenter);
    lay3->addWidget(lblText3);
    lay3->addStretch();
    btnDatabase->setFixedSize(200, lblText3->height() + 20);
    btnDatabase->setEnabled(false);
    // connect(btnDatabase, &QPushButton::clicked, this, &FnsIniziale::onDbLibrary);
    layContainer->addWidget(btnDatabase, 0, Qt::AlignCenter);

    layContainer->addStretch();

    // QPushButton per la visualizzazione della relazione tecnica (informazioni sul funzionamento dell'applicazione)
    QPushButton *btnInfo = new QPushButton(container);
    QHBoxLayout *lay4 = new QHBoxLayout(btnInfo);
    QLabel *lblIcon4 = new QLabel(btnInfo);
    lblIcon4->setPixmap(QIcon(":FnsIniziale/Info.svg").pixmap(20, 20));
    lay4->addWidget(lblIcon4, 0, Qt::AlignLeft);
    lay4->addStretch();
    QLabel *lblText4 = new QLabel(btnInfo);
    lblText4->setText("Informazioni");
    lay4->addWidget(lblText4);
    lay4->addStretch();
    btnInfo->setFixedSize(200, 35);
    btnInfo->setCursor(Qt::PointingHandCursor);
    // Connessione tra il click del QPushButton e l'apertura della relazione tecnica
    connect(btnInfo, &QPushButton::clicked, this, &FnsIniziale::onInfoRequest);
    layContainer->addWidget(btnInfo, 0, Qt::AlignCenter);

    layContainer->addStretch();
    layFnsIniziale->addWidget(container, 1, 1);
}

void FnsIniziale::onInfoRequest() {
    QUrl pdf = QUrl::fromLocalFile(QDir(DIR_RELAZIONE).path());     // Individuazione del path per la relazione tecnica
    if (QDesktopServices::openUrl(pdf)) {       // Se l'apertura su PDF Reader va a buon fine
        QMessageBox::information(
            this,
            "Richiesta informazioni",
            "Apertura della relazione tecnica.\nBuona lettura!");
    } else {
        QMessageBox::critical(
            this,
            "Richiesta informazioni",
            "Qualcosa è andato storto.");
    }
}

void FnsIniziale::onCleanLibrary() {
    Model::MediaLibrary *library = new Model::MediaLibrary();
    emit exportLibrary(library);        // Signal captato dalla MainWindow per inviare il puntatore alla libreria
}

void FnsIniziale::onJsonLibrary() {
    Model::MediaLibrary *library = new Model::MediaLibrary();

    unsigned int counter = 0, total = 0;        // Valori per identificare num. Media falliti e totali

    // Finestra di dialogo per selezione di file JSON per l'importazione
    QString pthFile = QFileDialog::getOpenFileName(
        this,
        "Importa media",
        QDir::homePath(),
        "JSON Files (*.json)");

    // Se il percorso non risulta vuoto
    if (!pthFile.isEmpty()) {
        // Funzione del JSONHandler per importazione dei Media all'interno del QList
        if (MediaSerializer::JSON::JsonSerializer::ImportJson(*library, pthFile, counter, total)) {
            // Finestra d'informazione per il corretto inserimento dei Media alla QList
            QMessageBox::information(
                this,
                "Importazione completata",
                "Importazione avvenuta per:\n" + QString::number(counter) + "/" + QString::number(total) + " media totali.");

            emit exportLibrary(library);
        }
        else {
            // Finestra di attenzione per il fallito inserimento dei Media alla QList
            QMessageBox::critical(
                this,
                "Importazione fallita",
                "Si è verificato un errore nell'importazione.");
        }
    }
}

// void FnsIniziale::onDbLibrary() {}
}
