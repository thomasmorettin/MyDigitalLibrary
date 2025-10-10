/**
 * @file MediaEditorVisitor.h/.cpp
 * @brief Classe Visitor utilizzata per creare gli oggetti QWidget per la visualizzazione
 * dei dettagli generici e specifici in base al tipo concreto di Media selezionato dall'utente e
 * permetterne la modifica tramite apposite box di testo.
 * @author Thomas Morettin
 */

#include "MediaEditorVisitor.h"
#include "../../Model/MediaLibrary.h"
#include "../../Model/Items/ListChecking/ListCheck.h"

#include <QDir>
#include <QFormLayout>
#include <QFileDialog>
#include <QVariant>
#include <QMessageBox>

#define KEY_LINGUA "LINGUA"      // Variabile globale per lingua del Media dal QPushButton per la lingua

namespace Visitor {
namespace View {
MediaEditorVisitor::MediaEditorVisitor(Model::MediaLibrary *library) : DesignVisitor(), library(library), cntSpecific(nullptr),
    txt1(nullptr), txt2(nullptr), txt3(nullptr), txt4(nullptr), txt5(nullptr), txt6(nullptr), cmbJolly(nullptr) {
    btnImg = new QPushButton(cntInfo);
    btnImg->setToolTip("Modifica copertina");
    btnImg->setObjectName("btnInfo");
    // Setting della copertina "No Image"
    QPixmap pxmNoImage(":Error/NoImageEditor.jpg");
    pxmNoImage = pxmNoImage.scaledToHeight(350, Qt::SmoothTransformation);
    btnImg->setIcon(QIcon(pxmNoImage));
    btnImg->setIconSize(pxmNoImage.size());
    btnImg->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnImg->setCursor(Qt::PointingHandCursor);
    // Connessione tra il click del QPushButton e il cambio di copertina del Media
    connect(btnImg, &QPushButton::clicked, this, &MediaEditorVisitor::onChangeCopertina);
    layCntInfo->addWidget(btnImg, 0, 0, 3, 1);        // Inserimento del QPushButton nel layout delle informazioni del Media

    // QWidget e QLayout per il contenimento della bandiera (lingua) + titolo del Media
    QWidget *cntLinTit = new QWidget(cntInfo);
    QHBoxLayout *layCntLinTit = new QHBoxLayout(cntLinTit);
    layCntLinTit->setContentsMargins(0, 0, 0, 0);

    btnLingua = new QToolButton(cntLinTit);
    btnLingua->setFixedHeight(17);
    btnLingua->setObjectName("btnInfo");
    btnLingua->setToolTip("Modifica lingua");
    // Icona del QPushButton
    QIcon icnLingua(":MediaEditorVisitor/Lingua.svg");
    btnLingua->setIcon(icnLingua);
    btnLingua->setCursor(Qt::PointingHandCursor);
    btnLingua->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnLingua->setPopupMode(QToolButton::InstantPopup);
    layCntLinTit->addWidget(btnLingua, 0, Qt::AlignLeft);       // Inserimento del QPushButton nel layout per lingua + titolo

    // Menu per selezionare bandiera (lingua) del Media da modificare
    menuLingua = new QMenu(btnLingua);
    menuLingua->setCursor(Qt::PointingHandCursor);
    // Voci "action" per lingua del Media
    QAction* actIT = new QAction("Italiano", menuLingua);
    actIT->setProperty(KEY_LINGUA, "IT");
    QAction* actEN = new QAction("Inglese", menuLingua);
    actEN->setProperty(KEY_LINGUA, "EN");
    QAction* actES = new QAction("Spagnolo", menuLingua);
    actES->setProperty(KEY_LINGUA, "ES");
    // Aggiunta delle voci al menu di inserimento
    menuLingua->addAction(actIT);
    menuLingua->addAction(actEN);
    menuLingua->addAction(actES);
    connect(menuLingua, &QMenu::triggered, this, &MediaEditorVisitor::onChangeLingua);       // Connessione tra il click di una QAction e lo slot per il cambio di lingua
    btnLingua->setMenu(menuLingua);       // Aggiunta del menu al QPushButton per la lingua

    txtTitolo = new QLineEdit(cntLinTit);
    txtTitolo->setPlaceholderText("Titolo");
    txtTitolo->setObjectName("txtTitolo");
    txtTitolo->setAlignment(Qt::AlignLeft);      // Allineamento del testo a sinistra
    layCntLinTit->addWidget(txtTitolo);     // Inserimento della QTextEdit nel layout per lingua + titolo

    layCntInfo->addWidget(cntLinTit, 0, 1, 1, 2, Qt::AlignTop);       // Inserimento del container per lingua + titolo nel layout delle informazioni

    txtDescrizione = new QTextEdit(cntInfo);
    txtDescrizione->setPlaceholderText("Descrizione");
    txtDescrizione->setAlignment(Qt::AlignTop);     // Allineamento del testo in alto
    txtDescrizione->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layCntInfo->addWidget(txtDescrizione, 1, 1, 1, 2);      // Inserimento della QTextEdit nel layout delle informazioni del Media

    // QWidget e QLayout per il contenimento delle informazioni generali di Media (data, generi, autori e copie)
    QWidget *infoMedia = new QWidget(cntInfo);
    QFormLayout *layInfoMedia = new QFormLayout(infoMedia);
    layInfoMedia->setContentsMargins(0, 0, 0, 0);
    layInfoMedia->setRowWrapPolicy(QFormLayout::WrapAllRows);

    txtData = new QLineEdit(cntInfo);
    txtData->setAlignment(Qt::AlignTop);
    txtData->setInputMask("00/00/0000;_");      // Input mask per l'inserimento formattato della data
    // Inserimento della QLineEdit nel layout delle info generali del Media (data, generi, autori e copie)
    layInfoMedia->addRow("<span style='color: #81A1C1; font-weight: bold'>Data di pubblicazione</span>", txtData);

    txtGeneri = new QTextEdit(cntInfo);
    txtGeneri->setAlignment(Qt::AlignTop);
    // Inserimento della QTextEdit nel layout delle info generali del Media (data, generi, autori e copie)
    layInfoMedia->addRow("<span style='color: #81A1C1; font-weight: bold'>Generi </span><span style='color: #81A1C1'>| Separare con \", \"</span>", txtGeneri);

    txtAutori = new QTextEdit(cntInfo);
    txtAutori->setAlignment(Qt::AlignTop);
    // Inserimento della QTextEdit nel layout delle info generali del Media (data, generi, autori e copie)
    layInfoMedia->addRow("<span style='color: #81A1C1; font-weight: bold'>Autori </span><span style='color: #81A1C1'>| Separare con \", \"</span>", txtAutori);

    txtCopie = new QLineEdit(cntInfo);
    txtCopie->setAlignment(Qt::AlignTop);
    // Inserimento della QLineEdit nel layout delle info generali del Media (data, generi, autori e copie)
    layInfoMedia->addRow("<span style='color: #81A1C1; font-weight: bold'>Copie a catalogo </span><span style='color: #81A1C1'>| > 0</span>", txtCopie);

    layCntInfo->addWidget(infoMedia, 2, 1, 1, 1, Qt::AlignBottom);       // Inserimento del container per le info generali del Media (data, generi e autori) nel layout per le informazioni
}

void MediaEditorVisitor::clear() {      // Funzione per la pulizia dei campi specifici del Media
    delete cntSpecific; cntSpecific = nullptr;
    txt1 = nullptr;
    txt2 = nullptr;
    txt3 = nullptr;
    txt4 = nullptr;
    txt5 = nullptr;
    txt6 = nullptr;
    cmbJolly = nullptr;
}

void MediaEditorVisitor::visitMedia() {
    this->lingua = index.data(Model::MediaLibrary::LinguaRole).toString();
    this->pthImage = index.data(Model::MediaLibrary::ImageRole).toString();

    // Setting della bandiera (lingua) da file .qrc, scalata su altezza di 17px
    QPixmap pxmLingua(":Lingua/" + index.data(Model::MediaLibrary::LinguaRole).toString() + ".png");
    if (!pxmLingua.isNull()) {
        pxmLingua = pxmLingua.scaledToHeight(17, Qt::SmoothTransformation);
        btnLingua->setIcon(QIcon(pxmLingua));
        btnLingua->setIconSize(pxmLingua.size());
    }
    else {      // Icona standard nel caso in cui non ci sia una lingua disponibile
        QIcon icnLingua(":MediaEditorVisitor/Lingua.svg");
        btnLingua->setIcon(icnLingua);
    }
    txtTitolo->setText(index.data(Model::MediaLibrary::TitoloRole).toString());     // Setting del titolo

    // Setting della copertina, da cartella del progetto, scalata su altezza di 350px
    QPixmap pxmImage(QDir(DIR_COPERTINE).filePath(index.data(Model::MediaLibrary::ImageRole).toString()));
    if (!pxmImage.isNull()) {
        pxmImage = pxmImage.scaledToHeight(350, Qt::SmoothTransformation);
        btnImg->setIcon(QIcon(pxmImage));
        btnImg->setIconSize(pxmImage.size());
    }
    else {      // Immagine standard nel caso in cui non ci sia l'immagine di copertina
        QPixmap pxmNoImage(":Error/NoImageEditor.jpg");
        pxmNoImage = pxmNoImage.scaledToHeight(350, Qt::SmoothTransformation);
        btnImg->setIcon(QIcon(pxmNoImage));
        btnImg->setIconSize(pxmNoImage.size());
    }

    // Setting della data
    txtData->setText(index.data(Model::MediaLibrary::DataRole).toString());

    // Setting dei generi e degli autori
    txtGeneri->setText(index.data(Model::MediaLibrary::GenereRole).toString());
    txtAutori->setText(index.data(Model::MediaLibrary::AutoreRole).toString());

    // Setting della descrizione e copie
    txtDescrizione->setText(index.data(Model::MediaLibrary::DescrizioneRole).toString());
    txtCopie->setText(index.data(Model::MediaLibrary::CopieRole).toString());
}

void MediaEditorVisitor::visitAlbum() {
    // Controllo sulla creazione degli elementi per info specifiche
    if (!cntSpecific) {
        // Connessione agli elementi del form per valutare errori/mancanze dei campi sotto elencati alla modifica delle box di testo
        connect(menuLingua, &QMenu::triggered, this, &MediaEditorVisitor::validateAlbum);
        connect(txtTitolo, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateAlbum);
        connect(txtDescrizione, &QTextEdit::textChanged, this, &MediaEditorVisitor::validateAlbum);
        connect(txtData, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateAlbum);
        connect(txtGeneri, &QTextEdit::textChanged, this, &MediaEditorVisitor::validateAlbum);
        connect(txtAutori, &QTextEdit::textChanged, this, &MediaEditorVisitor::validateAlbum);
        connect(txtCopie, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateAlbum);

        // QWidget e QLayout per il contenimento delle informazioni specifiche dell'Album
        cntSpecific = new QWidget(cntInfo);
        QFormLayout *layInfoAlbum = new QFormLayout(cntSpecific);
        layInfoAlbum->setContentsMargins(0, 0, 0, 0);
        layInfoAlbum->setRowWrapPolicy(QFormLayout::WrapAllRows);

        // QTextEdit per vettore canzoni dell'Album
        txt1 = new QTextEdit(cntSpecific);
        txt1->setAlignment(Qt::AlignTop);
        // Connessione tra il cambio del testo di QTextEdit e la verifica dei dati inseriti
        connect(txt1, &QTextEdit::textChanged, this, &MediaEditorVisitor::validateAlbum);
        // Inserimento della QTextEdit nel layout delle info specifiche dell'Album
        layInfoAlbum->addRow("<span style='color: #81A1C1; font-weight: bold'>Canzoni </span><span style='color: #81A1C1'>| Separare con \", \"</span>", txt1);

        // QTextEdit per vettore featuring dell'Album
        txt2 = new QTextEdit(cntSpecific);
        txt2->setAlignment(Qt::AlignTop);
        // Connessione tra il cambio del testo di QTextEdit e la verifica dei dati inseriti
        connect(txt2, &QTextEdit::textChanged, this, &MediaEditorVisitor::validateAlbum);
        // Inserimento della QTextEdit nel layout delle info specifiche dell'Album
        layInfoAlbum->addRow("<span style='color: #81A1C1; font-weight: bold'>Featuring </span><span style='color: #81A1C1'>| Separare con \", \"</span>", txt2);

        // QLineEdit per durata (min.) dell'Album
        txt4 = new QLineEdit(cntSpecific);
        txt4->setAlignment(Qt::AlignTop);
        // Connessione tra il cambio del testo di QLineEdit e la verifica dei dati inseriti
        connect(txt4, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateAlbum);
        // Inserimento della QLineEdit nel layout delle info specifiche dell'Album
        layInfoAlbum->addRow("<span style='color: #81A1C1; font-weight: bold'>Durata (min.) </span><span style='color: #81A1C1'>| > 4</span>", txt4);

        // QLineEdit per casa di produzione dell'Album
        txt5 = new QLineEdit(cntSpecific);
        txt5->setAlignment(Qt::AlignTop);
        // Connessione tra il cambio del testo di QLineEdit e la verifica dei dati inseriti
        connect(txt5, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateAlbum);
        // Inserimento della QLineEdit nel layout delle info specifiche dell'Album
        layInfoAlbum->addRow("<span style='color: #81A1C1; font-weight: bold'>Casa di prod.</span>", txt5);

        // QComboBox per formato dell'Album
        cmbJolly = new QComboBox(cntSpecific);
        cmbJolly->addItems(Model::ListChecking::ListCheck::getLstFormatoAlbum());
        // Connessione tra il cambio del testo di QComboBox e la verifica dei dati inseriti
        connect(cmbJolly, &QComboBox::currentTextChanged, this, &MediaEditorVisitor::validateAlbum);
        // Inserimento della QComboBox nel layout delle info specifiche dell'Album
        layInfoAlbum->addRow("<span style='color: #81A1C1; font-weight: bold'>Formato</span>", cmbJolly);

        // Inserimento del container per le info specifiche dell'Album nel layout per le informazioni
        layCntInfo->addWidget(cntSpecific, 2, 2, 1, 1);
    }

    this->visitMedia();     // Setting dei campi generici del Media
    // Setting dei campi specifici del Media
    txt1->setText(index.data(Model::MediaLibrary::I_Role).toString());
    txt2->setText(index.data(Model::MediaLibrary::II_Role).toString());
    txt4->setText(index.data(Model::MediaLibrary::III_Role).toString());
    txt5->setText(index.data(Model::MediaLibrary::IV_Role).toString());
    cmbJolly->setCurrentText(index.data(Model::MediaLibrary::V_Role).toString());
}
void MediaEditorVisitor::visitFilm() {
    // Controllo sulla creazione degli elementi per info specifiche
    if (!cntSpecific) {
        // Connessione agli elementi del form per valutare errori/mancanze dei campi sotto elencati alla modifica delle box di testo
        connect(menuLingua, &QMenu::triggered, this, &MediaEditorVisitor::validateFilm);
        connect(txtTitolo, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateFilm);
        connect(txtDescrizione, &QTextEdit::textChanged, this, &MediaEditorVisitor::validateFilm);
        connect(txtData, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateFilm);
        connect(txtGeneri, &QTextEdit::textChanged, this, &MediaEditorVisitor::validateFilm);
        connect(txtAutori, &QTextEdit::textChanged, this, &MediaEditorVisitor::validateFilm);
        connect(txtCopie, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateFilm);

        // QWidget e QLayout per il contenimento delle informazioni specifiche dell'Album
        cntSpecific = new QWidget(cntInfo);
        QFormLayout *layInfoFilm = new QFormLayout(cntSpecific);
        layInfoFilm->setContentsMargins(0, 0, 0, 0);
        layInfoFilm->setRowWrapPolicy(QFormLayout::WrapAllRows);

        // QTextEdit per vettore registi del Film
        txt1 = new QTextEdit(cntSpecific);
        txt1->setAlignment(Qt::AlignTop);
        // Connessione tra il cambio del testo di QTextEdit e la verifica dei dati inseriti
        connect(txt1, &QTextEdit::textChanged, this, &MediaEditorVisitor::validateFilm);
        // Inserimento della QTextEdit nel layout delle info specifiche del Film
        layInfoFilm->addRow("<span style='color: #81A1C1; font-weight: bold'>Registi </span><span style='color: #81A1C1'>| Separare con \", \"</span>", txt1);

        // QLineEdit per durata (min.) del Film
        txt4 = new QLineEdit(cntSpecific);
        txt4->setAlignment(Qt::AlignTop);
        // Connessione tra il cambio del testo di QLineEdit e la verifica dei dati inseriti
        connect(txt4, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateFilm);
        // Inserimento della QLineEdit nel layout delle info specifiche del Film
        layInfoFilm->addRow("<span style='color: #81A1C1; font-weight: bold'>Durata (min.) </span><span style='color: #81A1C1'>| > 4</span>", txt4);

        // QTextEdit per vettore attori del Film
        txt2 = new QTextEdit(cntSpecific);
        txt2->setAlignment(Qt::AlignTop);
        // Connessione tra il cambio del testo di QTextEdit e la verifica dei dati inseriti
        connect(txt2, &QTextEdit::textChanged, this, &MediaEditorVisitor::validateFilm);
        // Inserimento della QTextEdit nel layout delle info specifiche del Film
        layInfoFilm->addRow("<span style='color: #81A1C1; font-weight: bold'>Attori </span><span style='color: #81A1C1'>| Separare con \", \"</span>", txt2);

        // QTextEdit per vettore case di produzione del Film
        txt3 = new QTextEdit(cntSpecific);
        txt3->setAlignment(Qt::AlignTop);
        // Connessione tra il cambio del testo di QTextEdit e la verifica dei dati inseriti
        connect(txt3, &QTextEdit::textChanged, this, &MediaEditorVisitor::validateFilm);
        // Inserimento della QTextEdit nel layout delle info specifiche del Film
        layInfoFilm->addRow("<span style='color: #81A1C1; font-weight: bold'>Case di prod. </span><span style='color: #81A1C1'>| Separare con \", \"</span>", txt3);

        // QLineEdit per numero nella serie del Film
        txt5 = new QLineEdit(cntSpecific);
        txt5->setAlignment(Qt::AlignTop);
        // Connessione tra il cambio del testo di QTextEdit e la verifica dei dati inseriti
        connect(txt5, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateFilm);
        // Inserimento della QLineEdit nel layout delle info specifiche del Film
        layInfoFilm->addRow("<span style='color: #81A1C1; font-weight: bold'>Num. nella serie </span><span style='color: #81A1C1'>| > 0</span>", txt5);

        // QComboBox per formato del Film
        cmbJolly = new QComboBox(cntSpecific);
        cmbJolly->addItems(Model::ListChecking::ListCheck::getLstFormatoFilm());
        // Connessione tra il cambio del testo di QComboBox e la verifica dei dati inseriti
        connect(cmbJolly, &QComboBox::currentTextChanged, this, &MediaEditorVisitor::validateFilm);
        // Inserimento della QComboBox nel layout delle info specifiche del Film
        layInfoFilm->addRow("<span style='color: #81A1C1; font-weight: bold'>Formato</span>", cmbJolly);

        // Inserimento del container per le info specifiche del Film nel layout per le informazioni
        layCntInfo->addWidget(cntSpecific, 2, 2, 1, 1);
    }

    this->visitMedia();     // Setting dei campi generici del Media
    // Setting dei campi specifici del Media
    txt1->setText(index.data(Model::MediaLibrary::I_Role).toString());
    txt4->setText(index.data(Model::MediaLibrary::II_Role).toString());
    txt2->setText(index.data(Model::MediaLibrary::III_Role).toString());
    txt3->setText(index.data(Model::MediaLibrary::IV_Role).toString());
    txt5->setText(index.data(Model::MediaLibrary::V_Role).toString());
    cmbJolly->setCurrentText(index.data(Model::MediaLibrary::VI_Role).toString());
}
void MediaEditorVisitor::visitLibro() {
    if (!cntSpecific) {
        // Connessione agli elementi del form per valutare errori/mancanze dei campi sotto elencati alla modifica delle box di testo
        connect(menuLingua, &QMenu::triggered, this, &MediaEditorVisitor::validateLibro);
        connect(txtTitolo, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateLibro);
        connect(txtDescrizione, &QTextEdit::textChanged, this, &MediaEditorVisitor::validateLibro);
        connect(txtData, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateLibro);
        connect(txtGeneri, &QTextEdit::textChanged, this, &MediaEditorVisitor::validateLibro);
        connect(txtAutori, &QTextEdit::textChanged, this, &MediaEditorVisitor::validateLibro);
        connect(txtCopie, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateLibro);

        // QWidget e QLayout per il contenimento delle informazioni specifiche dell'Album
        cntSpecific = new QWidget(cntInfo);
        QFormLayout *layInfoLibro = new QFormLayout(cntSpecific);
        layInfoLibro->setContentsMargins(0, 0, 0, 0);
        layInfoLibro->setRowWrapPolicy(QFormLayout::WrapAllRows);

        // QLineEdit per ISBN del Libro
        txt4 = new QLineEdit(cntSpecific);
        txt4->setAlignment(Qt::AlignTop);
        txt4->setInputMask("0000000000000;_");      // Input mask per l'inserimento formattato dell'ISBN
        // Connessione tra il cambio del testo di QLineEdit e la verifica dei dati inseriti
        connect(txt4, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateLibro);
        // Inserimento della QLineEdit nel layout delle info specifiche del Libro
        layInfoLibro->addRow("<span style='color: #81A1C1; font-weight: bold'>ISBN </span><span style='color: #81A1C1'>| Formato 13 char</span>", txt4);

        // QLineEdit per numero di pagine del Libro
        txt5 = new QLineEdit(cntSpecific);
        txt5->setAlignment(Qt::AlignTop);
        // Connessione tra il cambio del testo di QLineEdit e la verifica dei dati inseriti
        connect(txt5, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateLibro);
        // Inserimento della QLineEdit nel layout delle info specifiche del Libro
        layInfoLibro->addRow("<span style='color: #81A1C1; font-weight: bold'>Num. pagine </span><span style='color: #81A1C1'>| > 4</span>", txt5);

        // QLineEdit per casa editoriale del Libro
        txt6 = new QLineEdit(cntSpecific);
        txt6->setAlignment(Qt::AlignTop);
        // Connessione tra il cambio del testo di QLineEdit e la verifica dei dati inseriti
        connect(txt6, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateLibro);
        // Inserimento della QLineEdit nel layout delle info specifiche del Libro
        layInfoLibro->addRow("<span style='color: #81A1C1; font-weight: bold'>Casa editoriale</span>", txt6);

        // Inserimento del container per le info specifiche del Film nel layout per le informazioni
        layCntInfo->addWidget(cntSpecific, 2, 2, 1, 1);
    }

    this->visitMedia();     // Setting dei campi generici del Media
    // Setting dei campi specifici del Media
    txt4->setText(index.data(Model::MediaLibrary::I_Role).toString());
    txt5->setText(index.data(Model::MediaLibrary::II_Role).toString());
    txt6->setText(index.data(Model::MediaLibrary::III_Role).toString());
}
void MediaEditorVisitor::visitRivista() {
    if (!cntSpecific) {
        // Connessione agli elementi del form per valutare errori/mancanze dei campi sotto elencati alla modifica delle box di testo
        connect(menuLingua, &QMenu::triggered, this, &MediaEditorVisitor::validateRivista);
        connect(txtTitolo, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateRivista);
        connect(txtDescrizione, &QTextEdit::textChanged, this, &MediaEditorVisitor::validateRivista);
        connect(txtData, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateRivista);
        connect(txtGeneri, &QTextEdit::textChanged, this, &MediaEditorVisitor::validateRivista);
        connect(txtAutori, &QTextEdit::textChanged, this, &MediaEditorVisitor::validateRivista);
        connect(txtCopie, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateRivista);

        // QWidget e QLayout per il contenimento delle informazioni specifiche dell'Album
        cntSpecific = new QWidget(cntInfo);
        QFormLayout *layInfoRivista = new QFormLayout(cntSpecific);
        layInfoRivista->setContentsMargins(0, 0, 0, 0);
        layInfoRivista->setRowWrapPolicy(QFormLayout::WrapAllRows);

        // QLineEdit per numero pagine della Rivista
        txt4 = new QLineEdit(cntSpecific);
        txt4->setAlignment(Qt::AlignTop);
        // Connessione tra il cambio del testo di QLineEdit e la verifica dei dati inseriti
        connect(txt4, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateRivista);
        // Inserimento della QLineEdit nel layout delle info specifiche della Rivista
        layInfoRivista->addRow("<span style='color: #81A1C1; font-weight: bold'>Num. pagine </span><span style='color: #81A1C1'>| > 4</span>", txt4);

        // QLineEdit per casa editoriale della Rivista
        txt5 = new QLineEdit(cntSpecific);
        txt5->setAlignment(Qt::AlignTop);
        // Connessione tra il cambio del testo di QLineEdit e la verifica dei dati inseriti
        connect(txt5, &QLineEdit::textChanged, this, &MediaEditorVisitor::validateRivista);
        // Inserimento della QLineEdit nel layout delle info specifiche della Rivista
        layInfoRivista->addRow("<span style='color: #81A1C1; font-weight: bold'>Casa editoriale</span>", txt5);

        // QComboBox per tipo di Rivista
        cmbJolly = new QComboBox(cntSpecific);
        cmbJolly->addItems(Model::ListChecking::ListCheck::getLstTipoRivista());
        cmbJolly->setFixedWidth(150);
        // Connessione tra il cambio del testo di QComboBox e la verifica dei dati inseriti
        connect(cmbJolly, &QComboBox::currentTextChanged, this, &MediaEditorVisitor::validateRivista);
        // Inserimento della QComboBox nel layout delle info specifiche della Rivista
        layInfoRivista->addRow("<span style='color: #81A1C1; font-weight: bold'>Tipologia</span>", cmbJolly);

        // Inserimento del container per le info specifiche del Film nel layout per le informazioni
        layCntInfo->addWidget(cntSpecific, 2, 2, 1, 1);
    }

    this->visitMedia();     // Setting dei campi generici del Media
    // Setting dei campi specifici del Media
    txt4->setText(index.data(Model::MediaLibrary::I_Role).toString());
    txt5->setText(index.data(Model::MediaLibrary::II_Role).toString());
    cmbJolly->setCurrentText(index.data(Model::MediaLibrary::III_Role).toString());
}

void MediaEditorVisitor::validateAlbum() {
    if (txt1 && txt4 && txt5) {     // Controllo sulla validità dei campi di input
        // Signal captato dalla FnsEditor per mostrare i campi non validi e azionare il salvataggio dei dati
        emit onValidation(library->validateAlbum(lingua,
                                                 txtTitolo->text(),
                                                 txtDescrizione->toPlainText(),
                                                 txtData->text(),
                                                 txtGeneri->toPlainText(),
                                                 txtAutori->toPlainText(),
                                                 txtCopie->text().toInt(),
                                                 txt1->toPlainText(),
                                                 txt4->text().toInt(),
                                                 txt5->text()));
    }
}

void MediaEditorVisitor::validateFilm() {
    if (txt1 && txt4 && txt3 && txt5) {     // Controllo sulla validità dei campi di input
        // Signal captato dalla FnsEditor per mostrare i campi non validi e azionare il salvataggio dei dati
        emit onValidation(library->validateFilm(lingua,
                                                txtTitolo->text(),
                                                txtDescrizione->toPlainText(),
                                                txtData->text(),
                                                txtGeneri->toPlainText(),
                                                txtAutori->toPlainText(),
                                                txtCopie->text().toInt(),
                                                txt1->toPlainText(),
                                                txt4->text().toInt(),
                                                txt3->toPlainText(),
                                                txt5->text().toInt()));
    }
}

void MediaEditorVisitor::validateLibro() {
    if (txt4 && txt5 && txt6) {     // Controllo sulla validità dei campi di input
        // Signal captato dalla FnsEditor per mostrare i campi non validi e azionare il salvataggio dei dati
        emit onValidation(library->validateLibro(lingua,
                                                 txtTitolo->text(),
                                                 txtDescrizione->toPlainText(),
                                                 txtData->text(),
                                                 txtGeneri->toPlainText(),
                                                 txtAutori->toPlainText(),
                                                 txtCopie->text().toInt(),
                                                 txt4->text(),
                                                 txt5->text().toInt(),
                                                 txt6->text()));
    }
}

void MediaEditorVisitor::validateRivista() {
    if (txt4 && txt5) {     // Controllo sulla validità dei campi di input
        // Signal captato dalla FnsEditor per mostrare i campi non validi e azionare il salvataggio dei dati
        emit onValidation(library->validateRivista(lingua,
                                                   txtTitolo->text(),
                                                   txtDescrizione->toPlainText(),
                                                   txtData->text(),
                                                   txtGeneri->toPlainText(),
                                                   txtAutori->toPlainText(),
                                                   txtCopie->text().toInt(),
                                                   txt4->text().toInt(),
                                                   txt5->text()));
    }
}

void MediaEditorVisitor::onChangeCopertina() {      // Funzione per il caricamento della copertina sul form (senza modificare il valore nel Media)
    QString origin = QFileDialog::getOpenFileName(
        this,
        "Selezione nuova copertina",
        QDir::homePath(),
        "Immagini (*.png *.jpg *.jpeg)");

    if (!origin.isEmpty()) {        // Controllo che il percorso esista
        QString fileName = QFileInfo(origin).fileName();
        QString dest = QDir(DIR_COPERTINE).filePath(fileName);      // Inserimento dell'immagine di copertina nella cartella dedicata di progetto

        if (!QFile::exists(dest)) { QFile::copy(origin, dest); }

        // Cambio della variabile locale e della copertina visualizzata a video
        pthImage = fileName;
        QPixmap pxmImage(QDir(DIR_COPERTINE).filePath(pthImage));
        pxmImage = pxmImage.scaledToHeight(350, Qt::SmoothTransformation);
        btnImg->setIcon(QIcon(pxmImage));
        btnImg->setIconSize(pxmImage.size());
    }
}

void MediaEditorVisitor::onChangeLingua(const QAction *action) {        // Funzione per il caricamento della lingua sul form (senza modificare il valore nel Media)
    QString newLingua = action->property(KEY_LINGUA).toString();

    // Cambio della variabile locale e della lingua visualizzata a video
    lingua = newLingua;
    QPixmap pxmLingua(":Lingua/" + newLingua + ".png");
    pxmLingua = pxmLingua.scaledToHeight(17, Qt::SmoothTransformation);
    btnLingua->setIcon(QIcon(pxmLingua));
    btnLingua->setIconSize(pxmLingua.size());
}

void MediaEditorVisitor::setMedia() {       // Funzione per il setting dei campi generici del Media
    library->setData(index, this->pthImage, Model::MediaLibrary::ImageRole);
    library->setData(index, txtTitolo->text(), Model::MediaLibrary::TitoloRole);
    library->setData(index, txtData->text(), Model::MediaLibrary::DataRole);
    library->setData(index, txtGeneri->toPlainText(), Model::MediaLibrary::GenereRole);
    library->setData(index, txtAutori->toPlainText(), Model::MediaLibrary::AutoreRole);
    library->setData(index, this->lingua, Model::MediaLibrary::LinguaRole);
    library->setData(index, txtDescrizione->toPlainText(), Model::MediaLibrary::DescrizioneRole);
    library->setData(index, txtCopie->text(), Model::MediaLibrary::CopieRole);

    // Disconnessione dei segnali in quanto dovranno validare campi differenti e su tipi concreti di Media differenti
    txtTitolo->disconnect();
    txtDescrizione->disconnect();
    txtData->disconnect();
    txtGeneri->disconnect();
    txtAutori->disconnect();
    txtCopie->disconnect();
}

void MediaEditorVisitor::setAlbum() {       // Funzione per il setting dei campi specifici del Media
    // Controllo sulla presenza o meno di un Media identico già presente
    QList<QModelIndex> foundIndex = library->findMediaLstIndex(index.row(), txtTitolo->text(),
                                                               this->lingua,
                                                               QString("Album"));

    unsigned int i = 0;
    while (i < foundIndex.size() && cmbJolly->currentText().compare(foundIndex[i].data(Model::MediaLibrary::V_Role).toString()) != 0) { ++i; }

    if (i == foundIndex.size()) {       // Nel caso in cui non siano presenti Media con lo stesso formato in quelli identici
        this->setMedia();
        disconnect(menuLingua, &QMenu::triggered, this, &MediaEditorVisitor::validateAlbum);

        library->setData(index, txt1->toPlainText(), Model::MediaLibrary::I_Role);
        library->setData(index, txt2->toPlainText(), Model::MediaLibrary::II_Role);
        library->setData(index, txt4->text(), Model::MediaLibrary::III_Role);
        library->setData(index, txt5->text(), Model::MediaLibrary::IV_Role);
        library->setData(index, cmbJolly->currentText(), Model::MediaLibrary::V_Role);

        emit action(index);     // Signal captato dalla FrmEditor per confermare l'avvenuta modifica
    } else {
        QMessageBox msgCheck(this);
        msgCheck.setWindowTitle("Media duplicato");
        msgCheck.setText("Questo Media esiste già.\nProseguendo verranno aggiunte le copie\nal Media già presente. Proseguire?");
        msgCheck.setIcon(QMessageBox::Question);
        msgCheck.setWindowIcon(this->windowIcon());
        QPushButton *btnSi = msgCheck.addButton("Sì", QMessageBox::YesRole);
        msgCheck.addButton("No", QMessageBox::NoRole);

        msgCheck.exec();

        if (msgCheck.clickedButton() == btnSi) {
            disconnect(menuLingua, &QMenu::triggered, this, &MediaEditorVisitor::validateAlbum);

            // Inserimento delle nuove copie all'interno del Media identico anche per formato/tipo/ISBN
            library->changeNumCopie(foundIndex[i], txtCopie->text().toInt());

            library->deleteMedia(index.row());
            emit action(foundIndex[i]);
        }
    }
}

void MediaEditorVisitor::setFilm() {       // Funzione per il setting dei campi specifici del Media
    // Controllo sulla presenza o meno di un Media identico già presente
    QList<QModelIndex> foundIndex = library->findMediaLstIndex(index.row(), txtTitolo->text(),
                                                               this->lingua,
                                                               QString("Film"));

    unsigned int i = 0;
    while (i < foundIndex.size() && cmbJolly->currentText().compare(foundIndex[i].data(Model::MediaLibrary::VI_Role).toString()) != 0) { ++i; }

    if (i == foundIndex.size()) {       // Nel caso in cui non siano presenti Media con lo stesso formato in quelli identici
        this->setMedia();
        disconnect(menuLingua, &QMenu::triggered, this, &MediaEditorVisitor::validateFilm);

        library->setData(index, txt1->toPlainText(), Model::MediaLibrary::I_Role);
        library->setData(index, txt4->text(), Model::MediaLibrary::II_Role);
        library->setData(index, txt2->toPlainText(), Model::MediaLibrary::III_Role);
        library->setData(index, txt3->toPlainText(), Model::MediaLibrary::IV_Role);
        library->setData(index, txt5->text(), Model::MediaLibrary::V_Role);
        library->setData(index, cmbJolly->currentText(), Model::MediaLibrary::VI_Role);

        emit action(index);     // Signal captato dalla FrmEditor per confermare l'avvenuta modifica
    } else {
        QMessageBox msgCheck(this);
        msgCheck.setWindowTitle("Media duplicato");
        msgCheck.setText("Questo Media esiste già.\nProseguendo verranno aggiunte le copie\nal Media già presente. Proseguire?");
        msgCheck.setIcon(QMessageBox::Question);
        msgCheck.setWindowIcon(this->windowIcon());
        QPushButton *btnSi = msgCheck.addButton("Sì", QMessageBox::YesRole);
        msgCheck.addButton("No", QMessageBox::NoRole);

        msgCheck.exec();

        if (msgCheck.clickedButton() == btnSi) {
            disconnect(menuLingua, &QMenu::triggered, this, &MediaEditorVisitor::validateAlbum);

            // Inserimento delle nuove copie all'interno del Media identico anche per formato/tipo/ISBN
            library->changeNumCopie(foundIndex[i], txtCopie->text().toInt());

            library->deleteMedia(index.row());
            emit action(foundIndex[i]);
        }
    }
}

void MediaEditorVisitor::setLibro() {       // Funzione per il setting dei campi specifici del Media
    // Controllo sulla presenza o meno di un Media identico già presente
    QList<QModelIndex> foundIndex = library->findMediaLstIndex(index.row(), txtTitolo->text(),
                                                               this->lingua,
                                                               QString("Libro"));

    // Nel caso in cui non siano presenti Media con lo stesso formato in quelli identici
    if (foundIndex.isEmpty() || txt4->text().compare(foundIndex[0].data(Model::MediaLibrary::I_Role).toString()) != 0) {
        this->setMedia();
        disconnect(menuLingua, &QMenu::triggered, this, &MediaEditorVisitor::validateLibro);

        library->setData(index, txt4->text(), Model::MediaLibrary::I_Role);
        library->setData(index, txt5->text(), Model::MediaLibrary::II_Role);
        library->setData(index, txt6->text(), Model::MediaLibrary::III_Role);

        emit action(index);
    } else {
        QMessageBox msgCheck(this);
        msgCheck.setWindowTitle("Media duplicato");
        msgCheck.setText("Questo Media esiste già.\nProseguendo verranno aggiunte le copie\nal Media già presente. Proseguire?");
        msgCheck.setIcon(QMessageBox::Question);
        msgCheck.setWindowIcon(this->windowIcon());
        QPushButton *btnSi = msgCheck.addButton("Sì", QMessageBox::YesRole);
        msgCheck.addButton("No", QMessageBox::NoRole);

        msgCheck.exec();

        if (msgCheck.clickedButton() == btnSi) {
            disconnect(menuLingua, &QMenu::triggered, this, &MediaEditorVisitor::validateAlbum);

            // Inserimento delle nuove copie all'interno del Media identico anche per formato/tipo/ISBN
            library->changeNumCopie(foundIndex[0], txtCopie->text().toInt());

            library->deleteMedia(index.row());
            emit action(foundIndex[0]);
        }
    }
}

void MediaEditorVisitor::setRivista() {       // Funzione per il setting dei campi specifici del Media
    // Controllo sulla presenza o meno di un Media identico già presente
    QList<QModelIndex> foundIndex = library->findMediaLstIndex(index.row(), txtTitolo->text(),
                                                               this->lingua,
                                                               QString("Rivista"));

    unsigned int i = 0;
    while (i < foundIndex.size() && cmbJolly->currentText().compare(foundIndex[i].data(Model::MediaLibrary::III_Role).toString()) != 0) { ++i; }

    if (i == foundIndex.size()) {       // Nel caso in cui non siano presenti Media con lo stesso formato in quelli identici
        this->setMedia();
        disconnect(menuLingua, &QMenu::triggered, this, &MediaEditorVisitor::validateRivista);

        library->setData(index, txt4->text(), Model::MediaLibrary::I_Role);
        library->setData(index, txt5->text(), Model::MediaLibrary::II_Role);
        library->setData(index, cmbJolly->currentText(), Model::MediaLibrary::III_Role);

        emit action(index);
    } else {
        QMessageBox msgCheck(this);
        msgCheck.setWindowTitle("Media duplicato");
        msgCheck.setText("Questo Media esiste già.\nProseguendo verranno aggiunte le copie\nal Media già presente. Proseguire?");
        msgCheck.setIcon(QMessageBox::Question);
        msgCheck.setWindowIcon(this->windowIcon());
        QPushButton *btnSi = msgCheck.addButton("Sì", QMessageBox::YesRole);
        msgCheck.addButton("No", QMessageBox::NoRole);

        msgCheck.exec();

        if (msgCheck.clickedButton() == btnSi) {
            disconnect(menuLingua, &QMenu::triggered, this, &MediaEditorVisitor::validateAlbum);

            // Inserimento delle nuove copie all'interno del Media identico anche per formato/tipo/ISBN
            library->changeNumCopie(foundIndex[i], txtCopie->text().toInt());

            library->deleteMedia(index.row());
            emit action(foundIndex[i]);
        }
    }
}
}
}
