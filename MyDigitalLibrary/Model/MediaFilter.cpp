/**
 * @file MediaFilter.h/.cpp
 * @brief Classe (estende QSortFilterProxyModel) per il filtraggio, tramite parametri
 * (titolo/autore/data e tipo concreto), dei Media a catalogo.
 * @author Thomas Morettin
 */

#include "MediaFilter.h"
#include "MediaLibrary.h"

namespace Model {
// Istanza del filtro di base per titolo
MediaFilter::MediaFilter(QObject *parent) : QSortFilterProxyModel(parent), mode(Titolo) {}

void MediaFilter::setFilterMode(const filterMode mode) {
    this->mode = mode;
    invalidateFilter();     // Funzione della classe base per il cambio dei filtri di ricerca
}

void MediaFilter::setSearch(const QString &search) {        // Cambiamento del testo da ricercare
    this->txtSearch = search;
    invalidateFilter();
}

void MediaFilter::setTipi(const QSet<QString> &tipi) {      // Cambiamento del tipo concreto da ricercare
    this->tipi = tipi;
    invalidateFilter();
}

bool MediaFilter::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const {
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);     // Intercettazione di un Media nel catalogo

    if (mode == Titolo) {
        QString titolo = index.data(MediaLibrary::TitoloRole).toString();
        if (!txtSearch.isEmpty() && !titolo.contains(txtSearch, Qt::CaseInsensitive)) { return false; }
    } else if (mode == Autore) {
        QString autore = index.data(MediaLibrary::AutoreRole).toString();
        if (!txtSearch.isEmpty() && !autore.contains(txtSearch, Qt::CaseInsensitive)) { return false; }
    } else if (mode == Data) {
        QString data = index.data(MediaLibrary::DataRole).toString();
        if (!txtSearch.isEmpty() && !data.contains(txtSearch, Qt::CaseInsensitive)) { return false; }
    }

    QString tipo = index.data(MediaLibrary::TipoRole).toString();
    if (!tipi.isEmpty() && !tipi.contains(tipo)) { return false; }

    /* Nel caso in cui dell'index corrispondono i parametri viene visualizzato, altrimenti (vedi sopra), per ciascun parametro
    non corrispondente viene ritornato false */
    return true;
}
}
