/**
 * @file MediaDettagliVisitor.h/.cpp
 * @brief Classe Visitor utilizzata per creare gli oggetti QWidget per la visualizzazione
 * dei dettagli generici e specifici in base al tipo concreto di Media selezionato dall'utente.
 * @author Thomas Morettin
 */

#include "MediaDettagliVisitor.h"
#include "../../Model/MediaLibrary.h"

#include <QDir>

namespace Visitor {
namespace View {
MediaDettagliVisitor::MediaDettagliVisitor() : DesignVisitor(), cntSpecific(nullptr),
    lbl1(nullptr), lbl2(nullptr), lbl3(nullptr), lbl4(nullptr), lbl5(nullptr), lbl6(nullptr) {
    lblImg = new QLabel(cntInfo);
    lblImg->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QPixmap pxmImage(":Error/NoImageDettagli.jpg");
    lblImg->setPixmap(pxmImage.scaledToHeight(350, Qt::SmoothTransformation));
    layCntInfo->addWidget(lblImg, 0, 0, 3, 1);        // Inserimento della QLabel nel layout delle informazioni del Media

    // QWidget e QLayout per il contenimento della bandiera (lingua) + titolo del Media
    QWidget *cntLinTit = new QWidget(cntInfo);
    QHBoxLayout *layCntLinTit = new QHBoxLayout(cntLinTit);
    layCntLinTit->setContentsMargins(0, 0, 0, 0);

    lblLingua = new QLabel(cntLinTit);
    lblLingua->setFixedHeight(17);
    lblLingua->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    layCntLinTit->addWidget(lblLingua, 0, Qt::AlignLeft);       // Inserimento della QLabel nel layout per lingua + titolo

    lblTitolo = new QLabel(cntLinTit);
    lblTitolo->setObjectName("titoloDettagli");
    lblTitolo->setWordWrap(true);
    lblTitolo->setAlignment(Qt::AlignLeft);     // Allineamento del testo a sinistra
    lblTitolo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layCntLinTit->addWidget(lblTitolo);     // Inserimento della QLabel nel layout per lingua + titolo

    layCntInfo->addWidget(cntLinTit, 0, 1, 1, 2);       // Inserimento del container per lingua + titolo nel layout delle informazioni

    lblDescrizione = new QLabel(cntInfo);
    lblDescrizione->setWordWrap(true);
    lblDescrizione->setAlignment(Qt::AlignTop);     // Allineamento del testo in alto
    layCntInfo->addWidget(lblDescrizione, 1, 1, 1, 2);      // Inserimento della QLabel nel layout per lingua + titolo

    // QWidget e QLayout per il contenimento delle informazioni generali di Media (data, generi, autori e copie)
    QWidget *infoMedia = new QWidget(cntInfo);
    QVBoxLayout *layInfoMedia = new QVBoxLayout(infoMedia);
    layInfoMedia->setContentsMargins(0, 0, 0, 0);

    lblData = new QLabel(infoMedia);
    lblData->setWordWrap(true);
    lblData->setAlignment(Qt::AlignTop);
    layInfoMedia->addWidget(lblData);       // Inserimento della QLabel nel layout delle info generali del Media (data, generi, autori e copie)

    lblGeneri = new QLabel(infoMedia);
    lblGeneri->setWordWrap(true);
    lblData->setAlignment(Qt::AlignTop);
    layInfoMedia->addWidget(lblGeneri);     // Inserimento della QLabel nel layout delle info generali del Media (data, generi, autori e copie)

    lblAutori = new QLabel(infoMedia);
    lblAutori->setWordWrap(true);
    lblData->setAlignment(Qt::AlignTop);
    layInfoMedia->addWidget(lblAutori);     // Inserimento della QLabel nel layout delle info generali del Media (data, generi, autori e copie)

    lblCopie = new QLabel(infoMedia);
    lblCopie->setWordWrap(true);
    lblCopie->setAlignment(Qt::AlignTop);
    layInfoMedia->addWidget(lblCopie);     // Inserimento della QLabel nel layout delle info generali del Media (data, generi, autori e copie)

    layCntInfo->addWidget(infoMedia, 2, 1, 1, 1);       // Inserimento del container per le info generali del Media (data, generi, autori e copie) nel layout per le informazioni
}

void MediaDettagliVisitor::clear() {        // Funzione di pulizia del QWidget per campi specifici del Media
    delete cntSpecific;
    cntSpecific = nullptr;
    lbl1 = nullptr;
    lbl2 = nullptr;
    lbl3 = nullptr;
    lbl4 = nullptr;
    lbl5 = nullptr;
    lbl6 = nullptr;
}

void MediaDettagliVisitor::visitMedia() {
    // Setting della bandiera (lingua) da file .qrc, scalata su altezza di 17px
    QPixmap pxmLingua(":Lingua/" + index.data(Model::MediaLibrary::LinguaRole).toString() + ".png");
    lblLingua->setPixmap(pxmLingua.scaledToHeight(17, Qt::SmoothTransformation));
    lblTitolo->setText(index.data(Model::MediaLibrary::TitoloRole).toString());     // Setting del titolo

    // Setting della copertina, da cartella del progetto, scalata su altezza di 350px
    QPixmap pxmImage(QDir(DIR_COPERTINE).filePath(index.data(Model::MediaLibrary::ImageRole).toString()));
    if (!pxmImage.isNull()) { lblImg->setPixmap(pxmImage.scaledToHeight(350, Qt::SmoothTransformation)); }
    else {      // Caso in cui la copertina non è disponibile
        QPixmap pxmNoImage(":Error/NoImageDettagli.jpg");
        lblImg->setPixmap(pxmNoImage.scaledToHeight(350, Qt::SmoothTransformation));
    }

    // Setting della data
    lblData->setText("<span style='color: #81A1C1; font-weight: bold'>Data di pubblicazione</span> " + index.data(Model::MediaLibrary::DataRole).toString());

    // Setting dei generi e degli autori
    lblGeneri->setText("<span style='color: #81A1C1; font-weight: bold'>Generi</span> " + index.data(Model::MediaLibrary::GenereRole).toString());
    lblAutori->setText("<span style='color: #81A1C1; font-weight: bold'>Autori</span> " + index.data(Model::MediaLibrary::AutoreRole).toString());

    // Setting della descrizione e delle copie
    lblDescrizione->setText(index.data(Model::MediaLibrary::DescrizioneRole).toString());
    lblCopie->setText("<span style='color: #81A1C1; font-weight: bold'>Copie a catalogo</span> " + index.data(Model::MediaLibrary::CopieRole).toString());
}

void MediaDettagliVisitor::visitAlbum() {
    this->visitMedia();

    if (!cntSpecific) {
        // QWidget e QLayout per il contenimento delle informazioni generali di Media (data, generi e autori)
        cntSpecific = new QWidget(cntInfo);
        QVBoxLayout *layInfoAlbum = new QVBoxLayout(cntSpecific);
        layInfoAlbum->setContentsMargins(0, 0, 0, 0);

        lbl1 = new QLabel(cntSpecific);
        lbl1->setWordWrap(true);
        lbl1->setAlignment(Qt::AlignTop);
        layInfoAlbum->addWidget(lbl1);

        lbl2 = new QLabel(cntSpecific);
        lbl2->setWordWrap(true);
        lbl2->setAlignment(Qt::AlignTop);
        layInfoAlbum->addWidget(lbl2);

        lbl3 = new QLabel(cntSpecific);
        lbl3->setWordWrap(true);
        lbl3->setAlignment(Qt::AlignTop);
        layInfoAlbum->addWidget(lbl3);

        lbl4 = new QLabel(cntSpecific);
        lbl4->setWordWrap(true);
        lbl4->setAlignment(Qt::AlignTop);
        layInfoAlbum->addWidget(lbl4);

        lbl5 = new QLabel(cntSpecific);
        lbl5->setWordWrap(true);
        lbl5->setAlignment(Qt::AlignTop);
        layInfoAlbum->addWidget(lbl5);
    }

    // Setting dei vettori di canzoni e featuring
    lbl1->setText("<span style='color: #81A1C1; font-weight: bold'>Canzoni</span> " + index.data(Model::MediaLibrary::I_Role).toString());
    lbl2->setText("<span style='color: #81A1C1; font-weight: bold'>Featuring</span> " + index.data(Model::MediaLibrary::II_Role).toString());

    // Setting della durata (in min.)
    lbl3->setText("<span style='color: #81A1C1; font-weight: bold'>Durata (min.)</span> " + index.data(Model::MediaLibrary::III_Role).toString());
    // Setting della casa di produzione
    lbl4->setText("<span style='color: #81A1C1; font-weight: bold'>Casa di prod.</span> " + index.data(Model::MediaLibrary::IV_Role).toString());
    // Setting del formato
    lbl5->setText("<span style='color: #81A1C1; font-weight: bold'>Formato</span> " + index.data(Model::MediaLibrary::V_Role).toString());

    // Inserimento del container per le info specifiche dell'Album nel layout per le informazioni
    layCntInfo->addWidget(cntSpecific, 2, 2, 1, 1);
}

void MediaDettagliVisitor::visitFilm() {
    this->visitMedia();

    if (!cntSpecific) {
        // QWidget e QLayout per il contenimento delle informazioni generali di Media (data, generi e autori)
        cntSpecific = new QWidget(cntInfo);
        QVBoxLayout *layInfoAlbum = new QVBoxLayout(cntSpecific);
        layInfoAlbum->setContentsMargins(0, 0, 0, 0);

        lbl1 = new QLabel(cntSpecific);
        lbl1->setWordWrap(true);
        lbl1->setAlignment(Qt::AlignTop);
        layInfoAlbum->addWidget(lbl1);

        lbl2 = new QLabel(cntSpecific);
        lbl2->setWordWrap(true);
        lbl2->setAlignment(Qt::AlignTop);
        layInfoAlbum->addWidget(lbl2);

        lbl3 = new QLabel(cntSpecific);
        lbl3->setWordWrap(true);
        lbl3->setAlignment(Qt::AlignTop);
        layInfoAlbum->addWidget(lbl3);

        lbl4 = new QLabel(cntSpecific);
        lbl4->setWordWrap(true);
        lbl4->setAlignment(Qt::AlignTop);
        layInfoAlbum->addWidget(lbl4);

        lbl5 = new QLabel(cntSpecific);
        lbl5->setWordWrap(true);
        lbl5->setAlignment(Qt::AlignTop);
        layInfoAlbum->addWidget(lbl5);

        lbl6 = new QLabel(cntSpecific);
        lbl6->setWordWrap(true);
        lbl6->setAlignment(Qt::AlignTop);
        layInfoAlbum->addWidget(lbl6);
    }

    // Setting del vettore di registi
    lbl1->setText("<span style='color: #81A1C1; font-weight: bold'>Registi</span> " + index.data(Model::MediaLibrary::I_Role).toString());
    // Setting della durata (in min.)
    lbl2->setText("<span style='color: #81A1C1; font-weight: bold'>Durata (min.)</span> " + index.data(Model::MediaLibrary::II_Role).toString());
    // Setting dei vettori di attori e case di produzione
    lbl3->setText("<span style='color: #81A1C1; font-weight: bold'>Attori</span> " + index.data(Model::MediaLibrary::III_Role).toString());
    lbl4->setText("<span style='color: #81A1C1; font-weight: bold'>Case di prod.</span> " + index.data(Model::MediaLibrary::IV_Role).toString());
    // Setting del numero nella serie
    lbl5->setText("<span style='color: #81A1C1; font-weight: bold'>Num. nella serie</span> " + index.data(Model::MediaLibrary::V_Role).toString());
    // Setting del formato
    lbl6->setText("<span style='color: #81A1C1; font-weight: bold'>Formato</span> " + index.data(Model::MediaLibrary::VI_Role).toString());

    // Inserimento del container per le info specifiche dell'Album nel layout per le informazioni
    layCntInfo->addWidget(cntSpecific, 2, 2, 1, 1);
}

void MediaDettagliVisitor::visitLibro() {
    this->visitMedia();

    if (!cntSpecific) {
        // QWidget e QLayout per il contenimento delle informazioni generali di Media (data, generi e autori)
        cntSpecific = new QWidget(cntInfo);
        QVBoxLayout *layInfoAlbum = new QVBoxLayout(cntSpecific);
        layInfoAlbum->setContentsMargins(0, 0, 0, 0);

        lbl1 = new QLabel(cntSpecific);
        lbl1->setWordWrap(true);
        lbl1->setAlignment(Qt::AlignTop);
        layInfoAlbum->addWidget(lbl1);

        lbl2 = new QLabel(cntSpecific);
        lbl2->setWordWrap(true);
        lbl2->setAlignment(Qt::AlignTop);
        layInfoAlbum->addWidget(lbl2);

        lbl3 = new QLabel(cntSpecific);
        lbl3->setWordWrap(true);
        lbl3->setAlignment(Qt::AlignTop);
        layInfoAlbum->addWidget(lbl3);
    }

    // Setting del codice ISBN
    lbl1->setText("<span style='color: #81A1C1; font-weight: bold'>ISBN</span> " + index.data(Model::MediaLibrary::I_Role).toString());
    // Setting del numero di pagine
    lbl2->setText("<span style='color: #81A1C1; font-weight: bold'>Num. pagine</span> " + index.data(Model::MediaLibrary::II_Role).toString());
    // Setting della casa editoriale
    lbl3->setText("<span style='color: #81A1C1; font-weight: bold'>Casa editoriale</span> " + index.data(Model::MediaLibrary::III_Role).toString());

    // Inserimento del container per le info specifiche dell'Album nel layout per le informazioni
    layCntInfo->addWidget(cntSpecific, 2, 2, 1, 1);
}

void MediaDettagliVisitor::visitRivista() {
    this->visitMedia();

    if (!cntSpecific) {
        // QWidget e QLayout per il contenimento delle informazioni generali di Media (data, generi e autori)
        cntSpecific = new QWidget(cntInfo);
        QVBoxLayout *layInfoAlbum = new QVBoxLayout(cntSpecific);
        layInfoAlbum->setContentsMargins(0, 0, 0, 0);

        lbl1 = new QLabel(cntSpecific);
        lbl1->setWordWrap(true);
        lbl1->setAlignment(Qt::AlignTop);
        layInfoAlbum->addWidget(lbl1);

        lbl2 = new QLabel(cntSpecific);
        lbl2->setWordWrap(true);
        lbl2->setAlignment(Qt::AlignTop);
        layInfoAlbum->addWidget(lbl2);

        lbl3 = new QLabel(cntSpecific);
        lbl3->setWordWrap(true);
        lbl3->setAlignment(Qt::AlignTop);
        layInfoAlbum->addWidget(lbl3);
    }

    // Setting del numero di pagine
    lbl1->setText("<span style='color: #81A1C1; font-weight: bold'>Num. pag.</span> " + index.data(Model::MediaLibrary::I_Role).toString());
    // Setting della casa editoriale
    lbl2->setText("<span style='color: #81A1C1; font-weight: bold'>Casa editoriale</span> " + index.data(Model::MediaLibrary::II_Role).toString());
    // Setting del tipo di rivista
    lbl3->setText("<span style='color: #81A1C1; font-weight: bold'>Tipo</span> " + index.data(Model::MediaLibrary::III_Role).toString());

    // Inserimento del container per le info specifiche dell'Album nel layout per le informazioni
    layCntInfo->addWidget(cntSpecific, 2, 2, 1, 1);
}
}
}
