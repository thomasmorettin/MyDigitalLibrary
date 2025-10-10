#include "View/MainWindow.h"

#include <QApplication>
#include <QFile>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFontDatabase::addApplicationFont(":InterRegular.ttf");
    QApplication::setFont(QFont("Inter 24pt", 10));

    // Caricamento del foglio di stile su applicazione
    QFile style(":Style.qss");
    if (style.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(style.readAll());
        app.setStyleSheet(styleSheet);
    }

    View::MainWindow win;
    win.show();
    return app.exec();
}
