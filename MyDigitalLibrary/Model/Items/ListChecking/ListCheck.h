/**
 * @file ListCheck.h/.cpp
 * @brief Classe per la memorizzazione dei tipi di formato per i Media concreti
 * Album, Film e Rivista.
 * @author Thomas Morettin
 */

#ifndef LISTCHECK_H
#define LISTCHECK_H

#include <QStringList>

namespace Model {
namespace ListChecking {
class ListCheck {
public:
    ~ListCheck() = default;

    static QStringList getLstFormatoAlbum() { return {"CD", "Musicassetta", "Vinile 33 g.", "Vinile 45 g."}; }
    static QStringList getLstFormatoFilm() { return {"DVD", "Blu-ray Disc"}; }
    static QStringList getLstTipoRivista() { return {"Settimanale", "Mensile", "Trimestrale", "Semestrale"}; }
};
}
}

#endif // LISTCHECK_H
