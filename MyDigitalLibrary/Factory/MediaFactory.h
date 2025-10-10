/**
 * @file MediaFactory.h/.cpp
 * @brief Classe per la ricostruzione di un oggetto Media a partire da un QJSonObject, prendendo
 * come riferimento il valore "media" che ne specifica il tipo concreto.
 * Nel caso in cui i parametri obbligatori risultano mancanti l'oggetto non viene catalogato.
 * @author Thomas Morettin
 */

#ifndef MEDIAFACTORY_H
#define MEDIAFACTORY_H

#include "../Model/Items/Media.h"

#include <QJsonObject>

namespace Factory {
class MediaFactory {
public:
    ~MediaFactory() = default;

    static Model::Items::Media *MediaFromJson(const QJsonObject &obj, QString &ifFormato);
};
}

#endif // MEDIAFACTORY_H
