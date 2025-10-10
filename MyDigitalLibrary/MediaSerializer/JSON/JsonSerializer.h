/**
 * @file JsonSerializer.h/.cpp
 * @brief Classe per la serializzazione del file .json, in quanto vengono implementate le
 * funzioni statiche per l'importazione e l'esportazione. Il controllo sul parsing del file
 * viene effettuato in automatico.
 * @author Thomas Morettin
 */

#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include "../../Model/MediaLibrary.h"

namespace MediaSerializer {
namespace JSON {
class JsonSerializer {
public:
    ~JsonSerializer() = default;

    static bool ExportJson(const Model::MediaLibrary &library, const QString &pthFile, const QPersistentModelIndex &addedMedia = QPersistentModelIndex());
    static bool ImportJson(Model::MediaLibrary &library, const QString &pthFile, unsigned int &counter, unsigned int &total);
};
}
}

#endif // JSONSERIALIZER_H
