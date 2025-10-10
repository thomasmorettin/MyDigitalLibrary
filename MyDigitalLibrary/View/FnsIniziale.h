/**
 * @file FnsIniziale.h/.cpp
 * @brief Classe (estende QWidget) che definisce il layout e le funzioni logiche della
 * finestra di inizializzazione della libreria.
 * @author Thomas Morettin
 */

#ifndef FNSINIZIALE_H
#define FNSINIZIALE_H

#include "../Model/MediaLibrary.h"

#include <QWidget>

namespace View {
class FnsIniziale : public QWidget {
    Q_OBJECT

public:
    explicit FnsIniziale(QWidget *parent = nullptr);
    ~FnsIniziale() = default;

private slots:
    void onInfoRequest();
    void onCleanLibrary();
    void onJsonLibrary();
    // void onDbLibrary();

signals:
    void exportLibrary(Model::MediaLibrary *library);
};
}

#endif // FNSINIZIALE_H
