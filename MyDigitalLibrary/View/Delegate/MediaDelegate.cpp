/**
 * @file MediaDelegate.h/.cpp
 * @brief Classe per la ridefinizione della visualizzazione grafica di ciascun elemento della
 * QListView, all'interno della quale viene visualizzato il catalogo di Media.
 * @author Thomas Morettin
 */

#include "MediaDelegate.h"
#include "../../Model/MediaLibrary.h"

#include <QPainter>
#include <QApplication>
#include <QDir>

namespace View {
namespace Delegate {
void MediaDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();

    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::Antialiasing);

    // Contenitore del Media
    QRect container = option.rect.adjusted(10, 0, -10, -10);
    painter->setBrush(QColor("#2E3440"));
    if (option.state & QStyle::State_MouseOver) { painter->setPen(QColor("#81A1C1")); }     // Cambio colore bordo nel caso di mouse hover
    else { painter->setPen(QColor("#4C566A")); }
    painter->drawRoundedRect(container, 6, 6);

    QString image = index.data(Model::MediaLibrary::ImageRole).toString();
    QPixmap pxmImage(QDir(DIR_COPERTINE).filePath(image));
    if (pxmImage.isNull()) { pxmImage.load(":Error/NoImagePreview.jpg"); }
    pxmImage = pxmImage.scaledToHeight(100, Qt::SmoothTransformation);

    // Contenitore per l'immagine del Media
    QRect imgRect = QRect(container.left() + 10, container.top() + 10, pxmImage.width(), pxmImage.height());
    painter->drawPixmap(imgRect, pxmImage);

    QString lingua = index.data(Model::MediaLibrary::LinguaRole).toString();
    QPixmap pxmLingua(":Lingua/" + lingua + ".png");
    pxmLingua = pxmLingua.scaledToHeight(13, Qt::SmoothTransformation);

    // Contenitore per la bandiera (lingua) del Media
    QRect linguaRect = QRect(imgRect.right() + 10, container.top() + 15, pxmLingua.width(), pxmLingua.height());
    painter->drawPixmap(linguaRect, pxmLingua);

    QFont font(option.font);
    font.setBold(true);
    font.setPixelSize(20);

    QString titolo = index.data(Model::MediaLibrary::TitoloRole).toString();
    int width = container.width() - (imgRect.right() + 10) - linguaRect.width() - 10 - 10;
    titolo = QFontMetrics(font).elidedText(titolo, Qt::ElideRight, width);

    // Contenitore per il titolo del Media
    QRect titoloRect = QRect(linguaRect.right() + 10, container.top() + 10,
                             width, QFontMetrics(font).height());
    painter->setPen(QColor("#FFFFFF"));
    painter->setFont(font);
    painter->drawText(titoloRect, titolo);

    QFont defFont(option.font);
    defFont.setBold(true);

    // Contenitore per "label" degli autori del Media
    QRect infoAutori = QFontMetrics(defFont).boundingRect(QString("Autori"));
    infoAutori.moveTo(imgRect.right() + 10, titoloRect.bottom() + 10);
    infoAutori.adjust(0, 0, 5, 0);
    painter->setPen(QColor("#81A1C1"));
    painter->setFont(defFont);
    painter->drawText(infoAutori, QString("Autori"));

    QString autori = index.data(Model::MediaLibrary::AutoreRole).toString();
    QFontMetrics fm(option.font);
    width = container.width() - (imgRect.right() + 10) - infoAutori.width() - 10 - 10;
    autori = fm.elidedText(autori, Qt::ElideRight, width);

    // Contenitore per gli autori del Media
    QRect autoriRect = QRect(infoAutori.right(), titoloRect.bottom() + 10,
                             width, QFontMetrics(option.font).height());
    painter->setPen(QColor("#FFFFFF"));
    painter->setFont(option.font);
    painter->drawText(autoriRect, autori);

    // Contenitore per "label" della data del Media
    QRect infoData = QFontMetrics(defFont).boundingRect(QString("Data di pubblicazione"));
    infoData.moveTo(imgRect.right() + 10, autoriRect.bottom() + 5);
    infoData.adjust(0, 0, 5, 0);
    painter->setPen(QColor("#81A1C1"));
    painter->setFont(defFont);
    painter->drawText(infoData, QString("Data di pubblicazione"));

    QString data = index.data(Model::MediaLibrary::DataRole).toString();
    width = container.width() - (imgRect.right() + 10) - infoData.width() - 10 - 10;
    data = fm.elidedText(data, Qt::ElideRight, width);

    // Contenitore per la data del Media
    QRect dataRect = QRect(infoData.right() - 12, autoriRect.bottom() + 5,
                             width, QFontMetrics(option.font).height());
    painter->setPen(QColor("#FFFFFF"));
    painter->setFont(option.font);
    painter->drawText(dataRect, data);

    // Contenitore per "label" delle copie del Media
    QRect infoCopie = QFontMetrics(defFont).boundingRect(QString("Copie a catalogo"));
    infoCopie.moveTo(imgRect.right() + 10, dataRect.bottom() + 5);
    infoCopie.adjust(0, 0, 5, 0);
    painter->setPen(QColor("#81A1C1"));
    painter->setFont(defFont);
    painter->drawText(infoCopie, QString("Copie a catalogo"));

    QString copie = index.data(Model::MediaLibrary::CopieRole).toString();
    width = container.width() - (imgRect.right() + 10) - infoCopie.width() - 10 - 10;

    // Contenitore per le copie del Media
    QRect copieRect = QRect(infoCopie.right() - 7, dataRect.bottom() + 5,
                             width, QFontMetrics(option.font).height());
    painter->setPen(QColor("#FFFFFF"));
    painter->setFont(option.font);
    painter->drawText(copieRect, copie);

    painter->restore();
}

// Le dimensioni del riquadro si basano sulle dimensione date agli oggetti al suo interno
QSize MediaDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    Q_UNUSED(index)
    return QSize(option.rect.width() - 20, 130);
}
}
}
