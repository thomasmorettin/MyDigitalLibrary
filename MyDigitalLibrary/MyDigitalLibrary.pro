QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Factory/MediaFactory.cpp \
    MediaSerializer/JSON/JsonSerializer.cpp \
    Model/Items/Album.cpp \
    Model/Items/Film.cpp \
    Model/Items/Libro.cpp \
    Model/Items/Media.cpp \
    Model/Items/Rivista.cpp \
    Model/MediaFilter.cpp \
    Model/MediaLibrary.cpp \
    View/Delegate/MediaDelegate.cpp \
    View/FnsCatalogo.cpp \
    View/FnsDettagli.cpp \
    View/FnsIniziale.cpp \
    View/FrmEditor.cpp \
    Visitor/DesignVisitor.cpp \
    Visitor/Items/MediaSetterVisitor.cpp \
    Visitor/Items/MediaGetterVisitor.cpp \
    Visitor/Items/CheckFormatoVisitor.cpp \
    Visitor/JSON/ToJsonVisitor.cpp \
    Visitor/View/MediaDettagliVisitor.cpp \
    Visitor/View/MediaEditorVisitor.cpp \
    main.cpp \
    View/MainWindow.cpp

HEADERS += \
    Factory/MediaFactory.h \
    MediaSerializer/JSON/JsonSerializer.h \
    Model/Items/Album.h \
    Model/Items/Film.h \
    Model/Items/Libro.h \
    Model/Items/ListChecking/ListCheck.h \
    Model/Items/Media.h \
    Model/Items/Rivista.h \
    Model/MediaFilter.h \
    Model/MediaLibrary.h \
    View/Delegate/MediaDelegate.h \
    View/FnsCatalogo.h \
    View/FnsDettagli.h \
    View/FnsIniziale.h \
    View/FrmEditor.h \
    View/MainWindow.h \
    Visitor/ConstVisitor.h \
    Visitor/DesignVisitor.h \
    Visitor/Items/MediaSetterVisitor.h \
    Visitor/Items/MediaGetterVisitor.h \
    Visitor/Items/CheckFormatoVisitor.h \
    Visitor/JSON/ToJsonVisitor.h \
    Visitor/View/MediaDettagliVisitor.h \
    Visitor/View/MediaEditorVisitor.h \
    Visitor/Visitor.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc

PROGETTO_DIR = $$PWD
DEFINES += DIR_PROGETTO=\\\"$$PROGETTO_DIR\\\"

COPERTINE_DIR = $$PWD/Resources/Images
DEFINES += DIR_COPERTINE=\\\"$$COPERTINE_DIR\\\"

SALVATAGGIO_DIR = $$PWD/Savings
DEFINES += DIR_SALVATAGGIO=\\\"$$SALVATAGGIO_DIR\\\"

RELAZIONE_DIR = $$PWD/../Relazione/RelazioneTecnica.pdf
DEFINES += DIR_RELAZIONE=\\\"$$RELAZIONE_DIR\\\"
