/**
 * @file MediaFilter.h/.cpp
 * @brief Classe (estende QSortFilterProxyModel) per il filtraggio, tramite parametri
 * (titolo/autore/data e tipo concreto), dei Media a catalogo.
 * @author Thomas Morettin
 */

#ifndef MEDIAFILTER_H
#define MEDIAFILTER_H

#include <QSortFilterProxyModel>

namespace Model {
class MediaFilter : public QSortFilterProxyModel {
    Q_OBJECT

public:
    enum filterMode { Titolo, Autore, Data };       // enum per i criteri di filtraggio nella ricerca

    explicit MediaFilter(QObject *parent = nullptr);

    void setFilterMode(const filterMode mode);
    void setSearch(const QString &search);
    void setTipi(const QSet<QString> &tipi);

private:
    QString txtSearch;
    QSet<QString> tipi;
    filterMode mode;

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};
}

#endif // MEDIAFILTER_H
