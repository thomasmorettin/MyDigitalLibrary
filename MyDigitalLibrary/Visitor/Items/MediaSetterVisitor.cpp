/**
 * @file MediaSetterVisitor.h/.cpp
 * @brief Classe Visitor utilizzata per settare il campo dato di un oggetto Media
 * in base al tipo di ruolo che viene richiesto dall'interfaccia.
 * Il metodo viene richiamato in base al tipo concreto dell'oggetto.
 * @author Thomas Morettin
 */

#include "MediaSetterVisitor.h"
#include "../../Model/MediaLibrary.h"
#include "../../Model/Items/Media.h"
#include "../../Model/Items/Album.h"
#include "../../Model/Items/Film.h"
#include "../../Model/Items/Libro.h"
#include "../../Model/Items/Rivista.h"

namespace Visitor {
namespace Items {
MediaSetterVisitor::MediaSetterVisitor(const int role, const QVariant &field) : role(role), field(field), valid(false) {}

bool MediaSetterVisitor::getValid() const { return valid; }          // Campo QVariant richiesto dall'interfaccia in base al ruolo

void MediaSetterVisitor::visitMedia(Model::Items::Media &media) {
    std::vector<std::string> vctStr;

    switch (role) {
    case Model::MediaLibrary::ImageRole:
        media.setPthImg(field.toString().toStdString());
        valid = true; break;
    case Model::MediaLibrary::TitoloRole:
        media.setTitolo(field.toString().toStdString());
        valid = true; break;
    case Model::MediaLibrary::DataRole:
        media.setData(field.toString().toStdString());
        valid = true; break;
    case Model::MediaLibrary::GenereRole:
        for (const QString &str : field.toString().split(", ")) { vctStr.push_back(str.toStdString()); }
        media.setVGenere(vctStr);
        valid = true; break;
    case Model::MediaLibrary::AutoreRole:
        vctStr.clear();
        for (const QString &str : field.toString().split(", ")) { vctStr.push_back(str.toStdString()); }
        media.setVAutore(vctStr);
        valid = true; break;
    case Model::MediaLibrary::LinguaRole:
        media.setLingua(field.toByteArray());
        valid = true; break;
    case Model::MediaLibrary::DescrizioneRole:
        media.setDescrizione(field.toString().toStdString());
        valid = true; break;
    case Model::MediaLibrary::CopieRole:
        media.setNumCopie(field.toInt());
        break;
    default: valid = false;
    }
}

void MediaSetterVisitor::visit(Model::Items::Album &album) {
    this->visitMedia(album);        // Chiamata all'ispezione dei campi generici del Media
    std::vector<std::string> vctStr;

    if (!valid) {       // Nel caso in cui il dato richiesto non fa parte dei campi generici del Media
        switch (role) {
        case Model::MediaLibrary::I_Role:
            for (const QString &str : field.toString().split(", ")) { vctStr.push_back(str.toStdString()); }
            album.setVCanzone(vctStr);
            valid = true; break;
        case Model::MediaLibrary::II_Role:
            vctStr.clear();
            for (const QString &str : field.toString().split(", ")) { vctStr.push_back(str.toStdString()); }
            album.setVFeat(vctStr);
            valid = true; break;
        case Model::MediaLibrary::III_Role:
            album.setDurata(field.toInt());
            valid = true; break;
        case Model::MediaLibrary::IV_Role:
            album.setCasa(field.toString().toStdString());
            valid = true; break;
        case Model::MediaLibrary::V_Role:
            album.setFormato(field.toString().toStdString());
            valid = true; break;
        default: valid = false;
        }
    }
}

void MediaSetterVisitor::visit(Model::Items::Film &film) {
    this->visitMedia(film);     // Chiamata all'ispezione dei campi generici del Media
    std::vector<std::string> vctStr;

    if (!valid) {       // Nel caso in cui il dato richiesto non fa parte dei campi generici del Media
        switch (role) {
        case Model::MediaLibrary::I_Role:
            for (const QString &str : field.toString().split(", ")) { vctStr.push_back(str.toStdString()); }
            film.setVRegista(vctStr);
            valid = true; break;
        case Model::MediaLibrary::II_Role:
            film.setDurata(field.toInt());
            valid = true; break;
        case Model::MediaLibrary::III_Role:
            vctStr.clear();
            for (const QString &str : field.toString().split(", ")) { vctStr.push_back(str.toStdString()); }
            film.setVAttore(vctStr);
            valid = true; break;
        case Model::MediaLibrary::IV_Role:
            vctStr.clear();
            for (const QString &str : field.toString().split(", ")) { vctStr.push_back(str.toStdString()); }
            film.setVCasa(vctStr);
            valid = true; break;
        case Model::MediaLibrary::V_Role:
            film.setNum(field.toInt());
            valid = true; break;
        case Model::MediaLibrary::VI_Role:
            film.setFormato(field.toString().toStdString());
            valid = true; break;
        default: valid = false;
        }
    }
}

void MediaSetterVisitor::visit(Model::Items::Libro &libro) {
    this->visitMedia(libro);        // Chiamata all'ispezione dei campi generici del Media

    if (!valid) {       // Nel caso in cui il dato richiesto non fa parte dei campi generici del Media
        switch (role) {
        case Model::MediaLibrary::I_Role:
            libro.setIsbn(field.toByteArray());
            valid = true; break;
        case Model::MediaLibrary::II_Role:
            libro.setNumPag(field.toInt());
            valid = true; break;
        case Model::MediaLibrary::III_Role:
            libro.setCasa(field.toString().toStdString());
            valid = true; break;
        default: valid = false;
        }
    }
}

void MediaSetterVisitor::visit(Model::Items::Rivista &rivista) {
    this->visitMedia(rivista);      // Chiamata all'ispezione dei campi generici del Media

    if (!valid) {       // Nel caso in cui il dato richiesto non fa parte dei campi generici del Media
        switch (role) {
        case Model::MediaLibrary::I_Role:
            rivista.setNumPag(field.toInt());
            valid = true; break;
        case Model::MediaLibrary::II_Role:
            rivista.setCasa(field.toString().toStdString());
            valid = true; break;
        case Model::MediaLibrary::III_Role:
            rivista.setTipo(field.toString().toStdString());
            valid = true; break;
        default: valid = false;
        }
    }
}
}
}
