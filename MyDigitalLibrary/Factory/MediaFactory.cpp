/**
 * @file MediaFactory.h/.cpp
 * @brief Classe per la ricostruzione di un oggetto Media a partire da un QJSonObject, prendendo
 * come riferimento il valore "media" che ne specifica il tipo concreto.
 * Nel caso in cui i parametri obbligatori risultano mancanti l'oggetto non viene catalogato.
 * @author Thomas Morettin
 */

#include "MediaFactory.h"
#include "../Model/Items/Album.h"
#include "../Model/Items/Film.h"
#include "../Model/Items/Libro.h"
#include "../Model/Items/Rivista.h"
#include "../Model/Items/ListChecking/ListCheck.h"

#include <QJsonArray>

namespace Factory {
Model::Items::Media *MediaFactory::MediaFromJson(const QJsonObject &obj, QString &ifFormato) {
    std::string tipo;
    char lingua[3];
    std::string titolo;
    std::string data;
    std::string pthImg;
    std::string descrizione;
    unsigned int numCopie;
    std::vector<std::string> vGenere;
    std::vector<std::string> vAutore;

    // Parametro di identificazione del tipo concreto (se non c'è: Media null)
    if (obj.contains("media") && obj["media"].isString()) { tipo = obj["media"].toString().toStdString(); }
    else { return nullptr; }

    if (obj.contains("lingua") && obj["lingua"].isString()) {
        QRegularExpression patternLingua("[A-Z]{2}");
        QString strLingua = obj["lingua"].toString();
        unsigned int lengthLingua = sizeof(lingua);
        // Controllo sul matching della lingua inserita
        if (patternLingua.match(strLingua).hasMatch() && strLingua.size() == lengthLingua - 1) {
            strncpy(lingua, strLingua.toUtf8(), lengthLingua - 1);
            lingua[lengthLingua - 1] = '\0';
        }
        else { return nullptr; }
    } else { return nullptr; }

    if (obj.contains("titolo") && obj["titolo"].isString() && !obj["titolo"].toString().isEmpty()) {
        titolo = obj["titolo"].toString().toStdString();
    } else { return nullptr; }

    if (obj.contains("data") && obj["data"].isString()) {
        QDate dataFormat = QDate::fromString(obj["data"].toString(), "dd/MM/yyyy");
        // Controllo sulla validità della data inserita (compresa di formato)
        if (dataFormat.isValid() && dataFormat <= QDate::currentDate()) { data = obj["data"].toString().toStdString(); }
        else { return nullptr; }
    } else { return nullptr; }

    if (obj.contains("pthImg") && obj["pthImg"].isString()) { pthImg = obj["pthImg"].toString().toStdString(); }

    if (obj.contains("descrizione") && obj["descrizione"].isString()) {
        descrizione = obj["descrizione"].toString().toStdString();
    }

    if (obj.contains("numCopie") && obj["numCopie"].isString() && obj["numCopie"].toString().toInt() > 0) {
        numCopie = obj["numCopie"].toString().toInt();
    } else { return nullptr; }

    // Inserimento dei parametri in vector
    QJsonArray array;
    std::vector<std::string> vct;
    if (obj.contains("generi") && obj["generi"].isArray()) {
        array = obj["generi"].toArray();
        for (const QJsonValue &js : array) { vct.push_back(js.toString().toStdString()); }
        vGenere = vct;
    }

    if (obj.contains("autori") && obj["autori"].isArray()) {
        array = obj["autori"].toArray();
        vct.clear();
        for (const QJsonValue &js : array) { vct.push_back(js.toString().toStdString()); }
        vAutore = vct;
    }

    // Oggetto concreto Libro
    if (tipo == "Libro") {
        char isbn[14];
        unsigned int numPag;
        std::string casa;

        if (obj.contains("isbn") && obj["isbn"].isString()) {
            QRegularExpression patternIsbn("^\\d{13}$");
            QString strIsbn = obj["isbn"].toString();
            unsigned int lengthIsbn = sizeof(isbn);
            // Controllo sulla validità del formato del codice ISBN
            if (patternIsbn.match(strIsbn).hasMatch() && strIsbn.size() == lengthIsbn - 1) {
                ifFormato = strIsbn;
                strncpy(isbn, strIsbn.toUtf8(), lengthIsbn - 1);
                isbn[lengthIsbn - 1] = '\0';
            }
            else { return nullptr; }
        } else { return nullptr; }

        if (obj.contains("numPag") && obj["numPag"].isString() && obj["numPag"].toString().toInt() > 4) {
            numPag = obj["numPag"].toString().toInt();
        } else { return nullptr; }

        if (obj.contains("casa") && obj["casa"].isString()) { casa = obj["casa"].toString().toStdString(); }

        return new Model::Items::Libro(pthImg, titolo, data, vGenere, vAutore, lingua, descrizione, numCopie,
                                       isbn, numPag, casa);
    } else if (tipo == "Film") {        // Oggetto concreto Film
        std::vector<std::string> vRegista;
        unsigned int durata;
        std::vector<std::string> vAttore;
        std::vector<std::string> vCasa;
        unsigned int num;
        std::string formato;

        if (obj.contains("formato") && obj["formato"].isString() && !obj["formato"].toString().isEmpty()) {
            // Controllo sulla validità del formato inserito (se accettabile da app)
            if (Model::ListChecking::ListCheck::getLstFormatoFilm().contains(obj["formato"].toString())) {
                ifFormato = obj["formato"].toString();
                formato = obj["formato"].toString().toStdString();
            } else { return nullptr; }
        } else { return nullptr; }

        if (obj.contains("numero") && obj["numero"].isString() && obj["numero"].toString().toInt() > 0) {
            num = obj["numero"].toString().toInt();
        } else { return nullptr; }

        if (obj.contains("durata") && obj["durata"].isString() && obj["durata"].toString().toInt() > 4) {
            durata = obj["durata"].toString().toInt();
        } else { return nullptr; }

        QJsonArray array;
        std::vector<std::string> vct;
        if (obj.contains("registi") && obj["registi"].isArray()) {
            array = obj["registi"].toArray();
            for (const QJsonValue &js : array) { vct.push_back(js.toString().toStdString()); }
            vRegista = vct;
        }

        if (obj.contains("attori") && obj["attori"].isArray()) {
            array = obj["attori"].toArray();
            vct.clear();
            for (const QJsonValue &js : array) { vct.push_back(js.toString().toStdString()); }
            vAttore = vct;
        }

        if (obj.contains("case") && obj["case"].isArray()) {
            array = obj["case"].toArray();
            vct.clear();
            for (const QJsonValue &js : array) { vct.push_back(js.toString().toStdString()); }
            vCasa = vct;
        }

        return new Model::Items::Film(pthImg, titolo, data, vGenere, vAutore, lingua, descrizione, numCopie,
                                      vRegista, durata, vAttore, vCasa, num, formato);
    } else if (tipo == "Rivista") {     // Oggetto concreto Rivista
        unsigned int numPag;
        std::string casa;
        std::string tipo;

        if (obj.contains("tipo") && obj["tipo"].isString() && !obj["tipo"].toString().isEmpty()) {
            // Controllo sulla validità del formato inserito (se accettabile da app)
            if (Model::ListChecking::ListCheck::getLstTipoRivista().contains(obj["tipo"].toString())) {
                ifFormato = obj["tipo"].toString();
                tipo = obj["tipo"].toString().toStdString();
            } else { return nullptr; }
        } else { return nullptr; }

        if (obj.contains("numPag") && obj["numPag"].isString() && obj["numPag"].toString().toInt() > 4) {
            numPag = obj["numPag"].toString().toInt();
        } else { return nullptr; }

        if (obj.contains("casa") && obj["casa"].isString()) { casa = obj["casa"].toString().toStdString(); }

        return new Model::Items::Rivista(pthImg, titolo, data, vGenere, vAutore, lingua, descrizione, numCopie,
                                         numPag, casa, tipo);
    } else if (tipo == "Album") {       // Oggetto concreto Album
        std::vector<std::string> vCanzone;
        std::vector<std::string> vFeat;
        unsigned int durata;
        std::string casa;
        std::string formato;

        if (obj.contains("formato") && obj["formato"].isString() && !obj["formato"].toString().isEmpty()) {
            // Controllo sulla validità del formato inserito (se accettabile da app)
            if (Model::ListChecking::ListCheck::getLstFormatoAlbum().contains(obj["formato"].toString())) {
                ifFormato = obj["formato"].toString();
                formato = obj["formato"].toString().toStdString();
            } else { return nullptr; }
        } else { return nullptr; }

        if (obj.contains("durata") && obj["durata"].isString() && obj["durata"].toString().toInt() > 4) {
            durata = obj["durata"].toString().toInt();
        } else { return nullptr; }

        if (obj.contains("casa") && obj["casa"].isString()) { casa = obj["casa"].toString().toStdString(); }

        QJsonArray array;
        std::vector<std::string> vct;
        if (obj.contains("canzoni") && obj["canzoni"].isArray() && !obj["canzoni"].toArray().isEmpty()) {
            array = obj["canzoni"].toArray();
            for (const QJsonValue &js : array) { vct.push_back(js.toString().toStdString()); }
            vCanzone = vct;
        } else { return nullptr; }

        if (obj.contains("feats") && obj["feats"].isArray()) {
            array = obj["feats"].toArray();
            vct.clear();
            for (const QJsonValue &js : array) { vct.push_back(js.toString().toStdString()); }
            vFeat = vct;
        }

        return new Model::Items::Album(pthImg, titolo, data, vGenere, vAutore, lingua, descrizione, numCopie,
                                       vCanzone, vFeat, durata, casa, formato);
    }

    return nullptr;
}
}
