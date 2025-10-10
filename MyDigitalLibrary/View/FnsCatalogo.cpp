/**
 * @file FnsCatalogo.h/.cpp
 * @brief Classe (estende QWidget) che definisce il layout e le funzioni logiche della
 * finestra di visualizzazione del catalogo.
 * @author Thomas Morettin
 */

#include "FnsCatalogo.h"
#include "Delegate/MediaDelegate.h"
#include "../Model/Items/Libro.h"
#include "../MediaSerializer/JSON/JsonSerializer.h"

#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QModelIndex>

#define KEY_TIPO "TIPO"     // Variabile globale per tipo di Media selezionato da QPushButton per inserimento

namespace View {
FnsCatalogo::FnsCatalogo(QWidget *parent, Model::MediaLibrary *library) : QWidget(parent), library(library) {
    fltModel = new Model::MediaFilter(this);
    fltModel->setSourceModel(library);

    QVBoxLayout *layFnsCatalogo = new QVBoxLayout(this);

    // QWidget e QLayout per il contenimento dei pulsanti azione della finestra
    QWidget *pulsanti = new QWidget(this);
    QHBoxLayout *layPulsanti = new QHBoxLayout(pulsanti);
    layPulsanti->setContentsMargins(0, 0, 0, 0);

    // QWidget e QLayout per il contenimento dei pulsanti per filtrare i Media per tipologia
    QWidget *filtriMedia = new QWidget(this);
    QHBoxLayout *layFiltriMedia = new QHBoxLayout(filtriMedia);
    layFiltriMedia->setContentsMargins(0, 0, 0, 0);

    // QWidget e QLayout per il contenimento delle QCheckBox per filtrare la ricerca
    QWidget *filtriRicerca = new QWidget(this);
    QHBoxLayout *layFiltriRicerca = new QHBoxLayout(filtriRicerca);
    layFiltriRicerca->setContentsMargins(0, 0, 0, 0);

    btnAdd = new QToolButton(pulsanti);
    btnAdd->setFixedSize(35, 35);
    btnAdd->setToolTip("Nuovo media");
    btnAdd->setCursor(Qt::PointingHandCursor);
    btnAdd->setPopupMode(QToolButton::InstantPopup);
    // Icona del QPushButton
    QIcon icnAdd(":FnsCatalogo/Insert.svg");
    btnAdd->setIcon(icnAdd);
    btnAdd->setIconSize(QSize(20, 20));
    layPulsanti->addWidget(btnAdd, Qt::AlignLeft);       // Inserimento del QPushButton nel layout dei pulsanti

    // Menu per selezionare tipologia di Media da aggiungere al catalogo
    menuAdd = new QMenu(btnAdd);
    menuAdd->setCursor(Qt::PointingHandCursor);
    // Voci "action" per tipologia di Media da aggiungere
    QAction *actLibro = new QAction("Libro", menuAdd);
    actLibro->setProperty(KEY_TIPO, "Libro");
    QAction *actFilm = new QAction("Film", menuAdd);
    actFilm->setProperty(KEY_TIPO, "Film");
    QAction *actRivista = new QAction("Rivista", menuAdd);
    actRivista->setProperty(KEY_TIPO, "Rivista");
    QAction *actAlbum = new QAction("Album", menuAdd);
    actAlbum->setProperty(KEY_TIPO, "Album");
    // Aggiunta delle voci al menu di inserimento
    menuAdd->addAction(actLibro);
    menuAdd->addAction(actFilm);
    menuAdd->addAction(actRivista);
    menuAdd->addAction(actAlbum);
    // Connessione tra il click di una QAction e l'emissione del segnale per l'inserimento di un nuovo Media
    connect(menuAdd, &QMenu::triggered, this, &FnsCatalogo::onAddNewMedia);
    btnAdd->setMenu(menuAdd);       // Aggiunta del menu al QPushButton di inserimento

    btnErase = new QPushButton(pulsanti);
    btnErase->setFixedSize(35, 35);
    btnErase->setToolTip("Elimina libreria");
    btnErase->setCursor(Qt::PointingHandCursor);
    // Icona del QPushButton
    QIcon icnErase(":CrossApp/EraseUndo.svg");
    btnErase->setIcon(icnErase);
    btnErase->setIconSize(QSize(20, 20));
    // Connessione tra il click del QPushButton e l'eliminazione totale della libreria
    connect(btnErase, &QPushButton::clicked, this, &FnsCatalogo::onEraseLibrary);
    layPulsanti->addWidget(btnErase, Qt::AlignLeft);       // Inserimento del QPushButton nel layout dei pulsanti

    btnImport = new QPushButton(pulsanti);
    btnImport->setFixedSize(35, 35);
    btnImport->setToolTip("Importa libreria");
    btnImport->setCursor(Qt::PointingHandCursor);
    // Icona del QPushButton
    QIcon icnImport(":FnsCatalogo/Import.svg");
    btnImport->setIcon(icnImport);
    btnImport->setIconSize(QSize(20, 20));
    // Connessione tra il click del QPushButton e l'emissione del segnale per l'import dei Media da file JSON
    connect(btnImport, &QPushButton::clicked, this, &FnsCatalogo::onImportJsonClicked);
    layPulsanti->addWidget(btnImport, Qt::AlignLeft);       // Inserimento del QPushButton nel layout dei pulsanti

    btnExport = new QPushButton(pulsanti);
    btnExport->setFixedSize(35, 35);
    btnExport->setToolTip("Esporta libreria");
    btnExport->setCursor(Qt::PointingHandCursor);
    // Icona del QPushButton
    QIcon icnExport(":FnsCatalogo/Export.svg");
    btnExport->setIcon(icnExport);
    btnExport->setIconSize(QSize(20, 20));
    // Connessione tra il click del QPushButton e l'emissione del segnale per l'export dei Media su file JSON
    connect(btnExport, &QPushButton::clicked, this, &FnsCatalogo::onExportJsonClicked);
    layPulsanti->addWidget(btnExport, Qt::AlignLeft);       // Inserimento del QPushButton nel layout dei pulsanti

    layPulsanti->addStretch();      // Stretch tra i QPushButton e QLineEdit

    txtSearch = new QLineEdit(pulsanti);
    txtSearch->setFixedSize(200, 35);
    txtSearch->setPlaceholderText("Ricerca per titolo");
    // Connessione tra il cambio del testo di QLineEdit e il cambio dei criteri di ricerca nel MediaFilter
    connect(txtSearch, &QLineEdit::textChanged, fltModel, &Model::MediaFilter::setSearch);
    layPulsanti->addWidget(txtSearch);      // Inserimento del QLineEdit nel layout dei pulsanti

    layFnsCatalogo->addWidget(pulsanti);        // Inserimento della sezione pulsanti nel layout della finestra
    layFiltriRicerca->addStretch();

    btnShowFiltri = new QPushButton(filtriRicerca);
    btnShowFiltri->setObjectName("btnShowFiltri");
    btnShowFiltri->setFixedHeight(35);
    btnShowFiltri->setText("Filtri");
    btnShowFiltri->setLayoutDirection(Qt::RightToLeft);
    btnShowFiltri->setCheckable(true);
    btnShowFiltri->setCursor(Qt::PointingHandCursor);
    // Icona del QPushButton
    QIcon icnShow(":FnsCatalogo/RightArrow.svg");
    btnShowFiltri->setIcon(icnShow);
    btnShowFiltri->setIconSize(QSize(20, 20));
    // Connessione tra il toggle del QPushButton e la visualizzazione dei filtri di ricerca
    connect(btnShowFiltri, &QPushButton::toggled, this, &FnsCatalogo::onShowFiltri);
    layFiltriRicerca->addWidget(btnShowFiltri);       // Inserimento del QPushButton nel layout dei pulsanti

    chkAutore = new QCheckBox(filtriRicerca);
    chkAutore->setText("Ricerca per autore");
    chkAutore->setVisible(false);       // Di default la QCheckBox per eliminazione del Media è invisibile
    chkAutore->setCursor(Qt::PointingHandCursor);
    // Connessione tra il cambio di stato di QCheckBox e l'individuazione di un nuovo filtro
    connect(chkAutore, &QCheckBox::toggled, this, [this](const bool checked) {
        if (checked) {
            QSignalBlocker blocker(chkData);        // Importante per mutua esclusione delle QCheckBox filtro
            chkData->setChecked(false);
        }
        txtSearch->setPlaceholderText("Ricerca per autore");
        fltModel->setFilterMode(Model::MediaFilter::Autore);        // Cambio del criterio di ricerca

        if (!checked && !chkData->isChecked()) {
            txtSearch->setPlaceholderText("Ricerca per titolo");
            fltModel->setFilterMode(Model::MediaFilter::Titolo);
        }
    });
    layFiltriRicerca->addWidget(chkAutore);

    chkData = new QCheckBox(filtriRicerca);
    chkData->setText("Ricerca per data");
    chkData->setVisible(false);       // Di default la QCheckBox per eliminazione del Media è invisibile
    chkData->setCursor(Qt::PointingHandCursor);
    // Connessione tra il cambio di stato di QCheckBox e l'individuazione di un nuovo filtro
    connect(chkData, &QCheckBox::toggled, this, [this](const bool checked) {
        if (checked) {
            QSignalBlocker blocker(chkAutore);
            chkAutore->setChecked(false);
        }
        txtSearch->setPlaceholderText("Ricerca per data");
        fltModel->setFilterMode(Model::MediaFilter::Data);

        if (!checked && !chkAutore->isChecked()) {
            txtSearch->setPlaceholderText("Ricerca per titolo");
            fltModel->setFilterMode(Model::MediaFilter::Titolo);
        }
    });
    layFiltriRicerca->addWidget(chkData);

    layFiltriRicerca->addStretch();
    layFnsCatalogo->addWidget(filtriRicerca);

    view = new QListView();
    view->setModel(fltModel);
    view->setItemDelegate(new Delegate::MediaDelegate(view));
    view->setMouseTracking(true);
    view->setUniformItemSizes(false);
    view->setContextMenuPolicy(Qt::CustomContextMenu);
    view->setAttribute(Qt::WA_Hover, true);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    // Connessione tra il click della QListView e la selezione del Media
    connect(view, &QListView::clicked, this, &FnsCatalogo::onMediaClicked);
    // Connessione tra il click con tasto destro della QListView e visualizzazione di Menu operazioni
    connect(view, &QListView::customContextMenuRequested, this, &FnsCatalogo::onCustomContextMenu);
    layFnsCatalogo->addWidget(view);

    layFiltriMedia->addStretch();       // Inserimento dello stretch da sx

    btnLibro = new QPushButton(filtriMedia);
    btnLibro->setText("Libri");
    btnLibro->setFixedSize(100, 35);
    btnLibro->setCheckable(true);       // Setting del QPushButton come toggle
    btnLibro->setCursor(Qt::PointingHandCursor);
    // Connessione tra il QPushButton per criterio di ricerca e il cambio dei criteri nel MediaFilter
    connect(btnLibro, &QPushButton::toggled, this, &FnsCatalogo::onFiltersChanged);
    layFiltriMedia->addWidget(btnLibro);        // Inserimento del QPushButton nel layout dei filtri

    btnFilm = new QPushButton(filtriMedia);
    btnFilm->setText("Film");
    btnFilm->setFixedSize(100, 35);
    btnFilm->setCheckable(true);
    btnFilm->setCursor(Qt::PointingHandCursor);
    // Connessione tra il QPushButton per criterio di ricerca e il cambio dei criteri nel MediaFilter
    connect(btnFilm, &QPushButton::toggled, this, &FnsCatalogo::onFiltersChanged);
    layFiltriMedia->addWidget(btnFilm);

    btnRivista = new QPushButton(filtriMedia);
    btnRivista->setText("Riviste");
    btnRivista->setFixedSize(100, 35);
    btnRivista->setCheckable(true);
    btnRivista->setCursor(Qt::PointingHandCursor);
    // Connessione tra il QPushButton per criterio di ricerca e il cambio dei criteri nel MediaFilter
    connect(btnRivista, &QPushButton::toggled, this, &FnsCatalogo::onFiltersChanged);
    layFiltriMedia->addWidget(btnRivista);

    btnAlbum = new QPushButton(filtriMedia);
    btnAlbum->setText("Album");
    btnAlbum->setFixedSize(100, 35);
    btnAlbum->setCheckable(true);
    btnAlbum->setCursor(Qt::PointingHandCursor);
    // Connessione tra il QPushButton per criterio di ricerca e il cambio dei criteri nel MediaFilter
    connect(btnAlbum, &QPushButton::toggled, this, &FnsCatalogo::onFiltersChanged);
    layFiltriMedia->addWidget(btnAlbum);

    layFiltriMedia->addStretch();        // Inserimento dello stretch da dx
    layFnsCatalogo->addWidget(filtriMedia);     // Inserimento della sezione filtri nel layout della finestra
}

void FnsCatalogo::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    view->doItemsLayout();      // Aggiornamento della view per eventuale ridimensionamento dei delegate
}

void FnsCatalogo::onExportJsonClicked() {
    // Finestra di dialogo per salvataggio di file JSON per l'esportazione
    QString pthFile = QFileDialog::getSaveFileName(
        this,
        "Esporta media",
        QDir::homePath(),
        "JSON Files (*.json)");

    // Se il percorso non risulta vuoto
    if (!pthFile.isEmpty()) {
        if (!pthFile.endsWith(".json", Qt::CaseInsensitive)) { pthFile += ".json"; }

        // Funzione del JsonSerializer per esportazione dei Media dal Model
        if (MediaSerializer::JSON::JsonSerializer::ExportJson(*library, pthFile)) {
            // Finestra d'informazione per la corretta esportazione dei Media
            QMessageBox::information(
                this,
                "Esportazione completata",
                "Esportazione avvenuta con successo");
        }
        else {
            // Finestra di attenzione per la fallita esportazione dei Media
            QMessageBox::critical(
                this,
                "Esportazione fallita",
                "Si è verificato un errore nell'esportazione.");
        }
    }
}

void FnsCatalogo::onImportJsonClicked() {
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

void FnsCatalogo::onCustomContextMenu(const QPoint &pos) {
    QModelIndex index = fltModel->mapToSource(view->indexAt(pos));
    if (!index.isValid()) { return; }       // Controllo del click su index (Media) valido

    QMenu menu(this);       // Menu a comparsa del click-dx sul delegate
    QAction *actModifica = menu.addAction("Modifica");
    QAction *actElimina = menu.addAction("Elimina");

    QAction *actSelected = menu.exec(view->viewport()->mapToGlobal(pos));

    // Controllo della posizione cliccata dall'utente
    if (actSelected == actModifica) { emit showEditor(index); }
    else if (actSelected == actElimina) { this->DeleteMedia(index); }
}

void FnsCatalogo::DeleteMedia(const QModelIndex &index) {
    QMessageBox msgDelete(this);
    msgDelete.setWindowTitle("Conferma cancellazione");
    msgDelete.setText("Sei sicuro di voler eliminare\nil Media?");
    msgDelete.setIcon(QMessageBox::Question);
    msgDelete.setWindowIcon(this->windowIcon());
    QPushButton *btnSi = msgDelete.addButton("Sì", QMessageBox::YesRole);
    msgDelete.addButton("No", QMessageBox::NoRole);

    msgDelete.exec();       // Richiesta per la conferma di cancellazione

    if (msgDelete.clickedButton() == btnSi) { library->deleteMedia(index.row()); }
}

void FnsCatalogo::onFiltersChanged() {
    QSet<QString> tipi;

    // Ogni volta che viene cliccato un pulsante toggle dei filtri vengono aggiornati i criteri
    if (btnLibro->isChecked()) { tipi.insert("Libro"); }
    else { tipi.remove("Libri"); }
    if (btnFilm->isChecked()) { tipi.insert("Film"); }
    else { tipi.remove("Film"); }
    if (btnRivista->isChecked()) { tipi.insert("Rivista"); }
    else { tipi.remove("Rivista"); }
    if (btnAlbum->isChecked()) { tipi.insert("Album"); }
    else { tipi.remove("Album"); }

    fltModel->setTipi(tipi);        // Cambio dei criteri di filtraggio per tipo concreto di Media
}

void FnsCatalogo::onEraseLibrary() {
    if (library->rowCount() > 0) {
        QMessageBox msgErase(this);
        msgErase.setWindowTitle("Conferma cancellazione");
        msgErase.setText("Sei sicuro di voler eliminare\nl'intera libreria?");
        msgErase.setIcon(QMessageBox::Question);
        msgErase.setWindowIcon(this->windowIcon());
        QPushButton *btnSi = msgErase.addButton("Sì", QMessageBox::YesRole);
        msgErase.addButton("No", QMessageBox::NoRole);

        msgErase.exec();       // Richiesta per la conferma di cancellazione

        if (msgErase.clickedButton() == btnSi) { library->eraseLibrary(); }     // Cancellazione completa della libreria
    } else {
        QMessageBox::information(
            this,
            "Cancellazione libreria",
            "La libreria è vuota,\nnulla da cancellare.");
    }
}

void FnsCatalogo::onMediaClicked(const QModelIndex &index) {
    if (!index.isValid()) { return; }
    emit showDettagli(fltModel->mapToSource(index));        // Signal captato dallo slot della MainWindow per la visualizzazione dettagli del Media
}

void FnsCatalogo::onShowFiltri(const bool checked) {        // Slot per visualizzare/nascondere QCheckBox
    if (!checked) {
        btnShowFiltri->setIcon(QIcon(":FnsCatalogo/RightArrow.svg"));

        chkAutore->setVisible(false);
        chkData->setVisible(false);
    } else {
        btnShowFiltri->setIcon(QIcon(":CrossApp/DownArrow.svg"));

        chkAutore->setVisible(true);
        chkData->setVisible(true);
    }
}

void FnsCatalogo::onAddNewMedia(const QAction *action) {
    QString newMedia = action->property(KEY_TIPO).toString();       // Viene individuato il tipo concreto che l'utente vuole aggiungere dal QMenu

    // Signal captato dallo slot della MainWindow per la visualizzazione della pagina di modifica del Media
    emit showAddMedia(QPersistentModelIndex(library->addMediaByType(newMedia)));
}
}
