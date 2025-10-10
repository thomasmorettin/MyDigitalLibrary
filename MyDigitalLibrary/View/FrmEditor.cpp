#include "FrmEditor.h"
#include "../Model/Items/Media.h"

#include <QVBoxLayout>
#include <QMessageBox>

namespace View {
FrmEditor::FrmEditor(QWidget *parent, Model::MediaLibrary *library) : QWidget(parent), library(library) {
    QVBoxLayout *layFrmEditor = new QVBoxLayout(this);      // Layout principale

    // QWidget e QLayout per il contenimento dei pulsanti azione della finestra
    QWidget *pulsanti = new QWidget(this);
    QHBoxLayout *layPulsanti = new QHBoxLayout(pulsanti);

    // QWidget e QLayout per la zona di finestra dedicata alla visualizzazione delle informazioni
    QWidget *container = new QWidget(this);
    QGridLayout *layContainer = new QGridLayout(container);

    // Stretch di righe/colonne per mantenere centrale il contenitore generale delle informazioni
    layContainer->setColumnStretch(0, 1);
    layContainer->setColumnStretch(2, 1);
    layContainer->setRowStretch(0, 1);
    layContainer->setRowStretch(2, 1);

    visitor = new Visitor::View::MediaEditorVisitor(this->library);
    layContainer->addWidget(visitor->getInfo(), 1, 1);
    // Connessione tra la validazione dei campi info del Media e abilitazione al salvataggio
    connect(visitor, &Visitor::View::MediaEditorVisitor::onValidation, this, [this](const QVector<QString> &vctValori) { this->vctValori = vctValori; });
    connect(visitor, &Visitor::View::MediaEditorVisitor::action, this, [this](const QPersistentModelIndex &index) { emit showDettagli(index); });

    layFrmEditor->addWidget(container);       // Inserimento del container per lo spazio apposito nel layout principale
    layPulsanti->addStretch();

    btnSave = new QPushButton(pulsanti);
    btnSave->setText("Salva");
    btnSave->setFixedHeight(35);
    btnSave->setObjectName("btnSave");
    btnSave->setCursor(Qt::PointingHandCursor);
    // Icona del QPushButton
    QIcon icnSave(":FrmEditor/Save.svg");
    btnSave->setIcon(icnSave);
    btnSave->setIconSize(QSize(20, 20));
    // Connessione tra il click del QPushButton e il salvataggio del Media
    connect(btnSave, &QPushButton::clicked, this, [this]() {
        if (this->vctValori.isEmpty()) { (this->library)->getMedia(index)->set(*visitor); }     // Se i valori sono tutti corretti vengono settati sul Media
        else {
            QString strValori;
            for (QString &str : this->vctValori) { strValori += "\n· " + str; }

            // QMessageBox di valori errati/mancanti
            QMessageBox::warning(
                this,
                "Attenzione sul form",
                "I seguenti campi risultano mancanti/errati:" + strValori);
        }
    });
    layPulsanti->addWidget(btnSave);       // Inserimento del QPushButton nel layout dei pulsanti

    btnReset = new QPushButton(pulsanti);
    btnReset->setText("Ripristina dati");
    btnReset->setFixedHeight(35);
    btnReset->setCursor(Qt::PointingHandCursor);
    // Icona del QPushButton
    QIcon icnReset(":FrmEditor/Restore.svg");
    btnReset->setIcon(icnReset);
    btnReset->setIconSize(QSize(20, 20));
    connect(btnReset, &QPushButton::clicked, this, [this]() {
        QMessageBox msgReset(this);
        msgReset.setWindowTitle("Conferma reset dati");
        msgReset.setText("Sei sicuro di voler resettare i dati?");
        msgReset.setIcon(QMessageBox::Question);
        msgReset.setWindowIcon(this->windowIcon());
        QPushButton *btnSi = msgReset.addButton("Sì", QMessageBox::YesRole);
        msgReset.addButton("No", QMessageBox::NoRole);

        msgReset.exec();       // Richiesta per la conferma di cancellazione

        if (msgReset.clickedButton() == btnSi) { (this->library)->getMedia(index)->accept(*visitor); }
    });
    layPulsanti->addWidget(btnReset);       // Inserimento del QPushButton nel layout dei pulsanti

    btnUnsave = new QPushButton(pulsanti);
    btnUnsave->setText("Annulla");
    btnUnsave->setObjectName("btnUnsave");
    btnUnsave->setFixedHeight(35);
    btnUnsave->setCursor(Qt::PointingHandCursor);
    // Icona del QPushButton
    QIcon icnUnsave(":CrossApp/EraseUndo.svg");
    btnUnsave->setIcon(icnUnsave);
    btnUnsave->setIconSize(QSize(20, 20));
    // Connessione tra il click del QPushButton e l'emissione del segnale di annullamento delle modifiche
    connect(btnUnsave, &QPushButton::clicked, this, [this]() { emit onUnsave(this->index); });
    layPulsanti->addWidget(btnUnsave);     // Inserimento del QPushButton nel layout dei pulsanti

    layPulsanti->addStretch();
    layFrmEditor->addWidget(pulsanti, 0, Qt::AlignBottom);     // Inserimento del container dei pulsanti nel layout principale
}

void FrmEditor::LoadMedia(const QPersistentModelIndex &index) {
    if (this->index != index) {
        this->index = index;

        visitor->clear();
        visitor->setIndex(this->index);     // Pulizia dei campi specifici (con molta probabilità non corrispondenti a quelli settati in precedenza)
    }

    if (library->getMedia(this->index)) { library->getMedia(this->index)->accept(*visitor); }
}
}
