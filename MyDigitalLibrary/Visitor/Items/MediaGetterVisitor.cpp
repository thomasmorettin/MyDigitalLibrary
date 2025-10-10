/**
 * @file MediaGetterVisitor.h/.cpp
 * @brief Classe Visitor utilizzata per restituire il campo dato di un oggetto Media
 * in base al tipo di ruolo che viene richiesto dall'interfaccia.
 * Il metodo viene richiamato in base al tipo concreto dell'oggetto.
 * @author Thomas Morettin
 */

#include "MediaGetterVisitor.h"
#include "../../Model/MediaLibrary.h"
#include "../../Model/Items/Media.h"
#include "../../Model/Items/Album.h"
#include "../../Model/Items/Film.h"
#include "../../Model/Items/Libro.h"
#include "../../Model/Items/Rivista.h"

namespace Visitor {
namespace Items {
MediaGetterVisitor::MediaGetterVisitor(const int role) : role(role) {}

QVariant MediaGetterVisitor::getField() const { return field; }     // Campo QVariant richiesto dall'interfaccia in base al ruolo

void MediaGetterVisitor::visitMedia(const Model::Items::Media &media) {
    QStringList lstQStr;

    switch (role) {
    case Model::MediaLibrary::ImageRole:
        field = QString::fromStdString(media.getPthImg());
        break;
    case Model::MediaLibrary::TitoloRole:
        field = QString::fromStdString(media.getTitolo());
        break;
    case Model::MediaLibrary::DataRole:
        field = QString::fromStdString(media.getData());
        break;
    case Model::MediaLibrary::GenereRole:
        for (const std::string &str : media.getVGenere()) { lstQStr.append(QString::fromStdString(str)); }
        field = lstQStr.join(", ");
        break;
    case Model::MediaLibrary::AutoreRole:
        lstQStr.clear();
        for (const std::string &str : media.getVAutore()) { lstQStr.append(QString::fromStdString(str)); }
        field = lstQStr.join(", ");
        break;
    case Model::MediaLibrary::LinguaRole:
        field = QString::fromUtf8(media.getLingua());
        break;
    case Model::MediaLibrary::DescrizioneRole:
        field = QString::fromStdString(media.getDescrizione());
        break;
    case Model::MediaLibrary::CopieRole:
        field = QString::number(media.getNumCopie());
        break;
    default: field = QVariant();
    }
}

void MediaGetterVisitor::visit(const Model::Items::Album &album) {
    this->visitMedia(album);        // Chiamata all'ispezione dei campi generici del Media
    QStringList lstQStr;

    if (field.isNull()) {       // Nel caso in cui il dato richiesto non fa parte dei campi generici del Media
        switch (role) {
        case Model::MediaLibrary::TipoRole:
            field = QString::fromStdString(album.getType());
            break;
        case Model::MediaLibrary::I_Role:
            for (const std::string &str : album.getVCanzone()) { lstQStr.append(QString::fromStdString(str)); }
            field = lstQStr.join(", ");
            break;
        case Model::MediaLibrary::II_Role:
            lstQStr.clear();
            for (const std::string &str : album.getVFeat()) { lstQStr.append(QString::fromStdString(str)); }
            field = lstQStr.join(", ");
            break;
        case Model::MediaLibrary::III_Role:
            field = album.getDurata();
            break;
        case Model::MediaLibrary::IV_Role:
            field = QString::fromStdString(album.getCasa());
            break;
        case Model::MediaLibrary::V_Role:
            field = QString::fromStdString(album.getFormato());
            break;
        default: field = QVariant();
        }
    }
}

void MediaGetterVisitor::visit(const Model::Items::Film &film) {
    this->visitMedia(film);     // Chiamata all'ispezione dei campi generici del Media
    QStringList lstQStr;

    if (field.isNull()) {       // Nel caso in cui il dato richiesto non fa parte dei campi generici del Media
        switch (role) {
        case Model::MediaLibrary::TipoRole:
            field = QString::fromStdString(film.getType());
            break;
        case Model::MediaLibrary::I_Role:
            for (const std::string &str : film.getVRegista()) { lstQStr.append(QString::fromStdString(str)); }
            field = lstQStr.join(", ");
            break;
        case Model::MediaLibrary::II_Role:
            field = film.getDurata();
            break;
        case Model::MediaLibrary::III_Role:
            lstQStr.clear();
            for (const std::string &str : film.getVAttore()) { lstQStr.append(QString::fromStdString(str)); }
            field = lstQStr.join(", ");
            break;
        case Model::MediaLibrary::IV_Role:
            lstQStr.clear();
            for (const std::string &str : film.getVCasa()) { lstQStr.append(QString::fromStdString(str)); }
            field = lstQStr.join(", ");
            break;
        case Model::MediaLibrary::V_Role:
            field = film.getNum();
            break;
        case Model::MediaLibrary::VI_Role:
            field = QString::fromStdString(film.getFormato());
            break;
        default: field = QVariant();
        }
    }
}

void MediaGetterVisitor::visit(const Model::Items::Libro &libro) {
    this->visitMedia(libro);        // Chiamata all'ispezione dei campi generici del Media

    if (field.isNull()) {       // Nel caso in cui il dato richiesto non fa parte dei campi generici del Media
        switch (role) {
        case Model::MediaLibrary::TipoRole:
            field = QString::fromStdString(libro.getType());
            break;
        case Model::MediaLibrary::I_Role:
            field = QString::fromUtf8(libro.getIsbn());
            break;
        case Model::MediaLibrary::II_Role:
            field = libro.getNumPag();
            break;
        case Model::MediaLibrary::III_Role:
            field = QString::fromStdString(libro.getCasa());
            break;
        default: field = QVariant();
        }
    }
}

void MediaGetterVisitor::visit(const Model::Items::Rivista &rivista) {
    this->visitMedia(rivista);      // Chiamata all'ispezione dei campi generici del Media

    if (field.isNull()) {       // Nel caso in cui il dato richiesto non fa parte dei campi generici del Media
        switch (role) {
        case Model::MediaLibrary::TipoRole:
            field = QString::fromStdString(rivista.getType());
            break;
        case Model::MediaLibrary::I_Role:
            field = rivista.getNumPag();
            break;
        case Model::MediaLibrary::II_Role:
            field = QString::fromStdString(rivista.getCasa());
            break;
        case Model::MediaLibrary::III_Role:
            field = QString::fromStdString(rivista.getTipo());
            break;
        default: field = QVariant();
        }
    }
}
}
}
