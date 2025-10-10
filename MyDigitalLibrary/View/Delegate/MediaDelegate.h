/**
 * @file MediaDelegate.h/.cpp
 * @brief Classe per la ridefinizione della visualizzazione grafica di ciascun elemento della
 * QListView, all'interno della quale viene visualizzato il catalogo di Media.
 * @author Thomas Morettin
 */

#ifndef MEDIADELEGATE_H
#define MEDIADELEGATE_H

#include <QStyledItemDelegate>

namespace View {
namespace Delegate {
class MediaDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    ~MediaDelegate() = default;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};
}
}

#endif // MEDIADELEGATE_H
