/**
 * @file FnsDettagli.h/.cpp
 * @brief Classe (estende QWidget) che definisce il layout e le funzioni logiche della
 * finestra di visualizzazione dei dettagli di ciascun Media. Fa uso del MediaDettagliVisitor
 * per il popolamento dei campi generici/specifici per ciascun Media.
 * @author Thomas Morettin
 */

#include "FnsDettagli.h"
#include "../Model/Items/Media.h"

#include <QGridLayout>
#include <QMessageBox>

namespace View {
FnsDettagli::FnsDettagli(QWidget *parent, Model::MediaLibrary *library) : QWidget(parent), library(library) {
    QVBoxLayout *layFnsDettagli = new QVBoxLayout(this);        // Layout principale

    // QWidget e QLayout per il contenimento dei pulsanti azione della finestra
    QWidget *pulsanti = new QWidget(this);
    QHBoxLayout *layPulsanti = new QHBoxLayout(pulsanti);
    layPulsanti->addStretch();      // Stretch a sx dei QPushButton

    btnBack = new QPushButton(pulsanti);
    btnBack->setFixedSize(35, 35);
    btnBack->setCursor(Qt::PointingHandCursor);
    // Icona del QPushButton
    QIcon icnBack(":CrossApp/Back.svg");
    btnBack->setIcon(icnBack);
    btnBack->setIconSize(QSize(20, 20));
    // Connessione tra il click del QPushButton e l'emissione del segnale per il ritorno alla pagina di Catalogo
    connect(btnBack, &QPushButton::clicked, this, [this]() { emit showCatalogo(); });
    layPulsanti->addWidget(btnBack);       // Inserimento del QPushButton nel layout dei pulsanti

    btnDelete = new QPushButton(pulsanti);
    btnDelete->setText("Elimina");
    btnDelete->setObjectName("btnDelete");
    btnDelete->setFixedHeight(35);
    btnDelete->setCursor(Qt::PointingHandCursor);
    // Icona del QPushButton
    QIcon icnDelete(":FnsDettagli/Delete.svg");
    btnDelete->setIcon(icnDelete);
    btnDelete->setIconSize(QSize(20, 20));
    // Connessione tra il click del QPushButton e l'eliminazione del Media visualizzato dal Catalogo
    connect(btnDelete, &QPushButton::clicked, this, &FnsDettagli::onDeleteClicked);
    layPulsanti->addWidget(btnDelete);       // Inserimento del QPushButton nel layout dei pulsanti

    btnEdit = new QPushButton(pulsanti);
    btnEdit->setText("Modifica");
    btnEdit->setFixedHeight(35);
    btnEdit->setCursor(Qt::PointingHandCursor);
    // Icona del QPushButton
    QIcon icnEdit(":FnsDettagli/Edit.svg");
    btnEdit->setIcon(icnEdit);
    btnEdit->setIconSize(QSize(20, 20));
    // Connessione tra il click del QPushButton e l'emissione del segnale per la modifica del Media
    connect(btnEdit, &QPushButton::clicked, this, [this]() { emit showEditor(this->index); });
    layPulsanti->addWidget(btnEdit);       // Inserimento del QPushButton nel layout dei pulsanti

    layPulsanti->addStretch();
    layFnsDettagli->addWidget(pulsanti, 0, Qt::AlignTop);      // Inserimento del container dei pulsanti nel layout principale

    // QWidget e QLayout per la zona di finestra dedicata alla visualizzazione delle informazioni
    QWidget *container = new QWidget(this);
    QGridLayout *layContainer = new QGridLayout(container);

    // Stretch di righe/colonne per mantenere centrale il contenitore generale delle informazioni
    layContainer->setColumnStretch(0, 1);
    layContainer->setColumnStretch(2, 1);
    layContainer->setRowStretch(0, 1);
    layContainer->setRowStretch(2, 1);

    visitor = new Visitor::View::MediaDettagliVisitor();
    layContainer->addWidget(visitor->getInfo(), 1, 1);

    layFnsDettagli->addWidget(container);       // Inserimento del container per lo spazio apposito nel layout principale
}

void FnsDettagli::LoadMedia(const QPersistentModelIndex &index) {       // Funzione per il caricamento dell'index da visualizzare
    if (this->index != index) {     // Se l'index non è lo stesso selezionato precedentemente
        this->index = index;

        visitor->clear();       // Pulizia dei campi specifici (con molta probabilità non corrispondenti a quelli settati in precedenza)
        visitor->setIndex(this->index);
    }

    if (library->getMedia(this->index)) { library->getMedia(this->index)->accept(*visitor); }       // Utilizzo del visitor per il popolamento
}

void FnsDettagli::onDeleteClicked() {
    QMessageBox msgDelete(this);
    msgDelete.setWindowTitle("Conferma cancellazione");
    msgDelete.setText("Sei sicuro di voler eliminare\nil Media?");
    msgDelete.setIcon(QMessageBox::Question);
    msgDelete.setWindowIcon(this->windowIcon());
    QPushButton *btnSi = msgDelete.addButton("Sì", QMessageBox::YesRole);
    msgDelete.addButton("No", QMessageBox::NoRole);

    msgDelete.exec();       // Richiesta per la conferma di cancellazione

    if (msgDelete.clickedButton() == btnSi) { library->deleteMedia(index.row()); }
    emit showCatalogo();        // Signal captato dalla MainWindow per rivisualizzare il catalogo
}
}
