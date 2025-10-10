/**
 * @file DesignVisitor.h/.cpp
 * @brief Classe Visitor base astratta utilizzata per istanziare un Visitor per
 * manipolare i QWidget per visualizzazione/inserimento dei campi del Media.
 * @author Thomas Morettin
 */

#include "DesignVisitor.h"

#include <QWidget>

namespace Visitor {
DesignVisitor::DesignVisitor() {
    // QWidget e QLayout per la visualizzazione delle informazioni del Media
    cntInfo = new QWidget();
    cntInfo->setObjectName("infoContainer");
    layCntInfo = new QGridLayout(cntInfo);
    cntInfo->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    layCntInfo->setContentsMargins(20, 20, 20, 20);
    layCntInfo->setSpacing(20);     // Spazio tra i campi del layout

    // Stretch di righe per fare spazio alla riga contenente la descrizione del Media
    layCntInfo->setRowStretch(0, 0);
    layCntInfo->setRowStretch(1, 1);
    layCntInfo->setRowStretch(2, 1);
}

DesignVisitor::~DesignVisitor() { delete cntInfo; }

void DesignVisitor::setIndex(const QPersistentModelIndex &index) { this->index = index; }
QWidget *DesignVisitor::getInfo() const { return cntInfo; }
}
