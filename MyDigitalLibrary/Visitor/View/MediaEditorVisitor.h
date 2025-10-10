/**
 * @file MediaEditorVisitor.h/.cpp
 * @brief Classe Visitor utilizzata per creare gli oggetti QWidget per la visualizzazione
 * dei dettagli generici e specifici in base al tipo concreto di Media selezionato dall'utente e
 * permetterne la modifica tramite apposite box di testo.
 * @author Thomas Morettin
 */

#ifndef MEDIAEDITORVISITOR_H
#define MEDIAEDITORVISITOR_H

#include "../DesignVisitor.h"

#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QToolButton>
#include <QPushButton>
#include <QMenu>
#include <QComboBox>

namespace Model {
class MediaLibrary;
}

namespace Visitor {
namespace View {
class MediaEditorVisitor : public DesignVisitor {
    Q_OBJECT

private:
    Model::MediaLibrary *library;

    // Variabili provvisorie per la visualizzazione della bandiera (lingua) + copertina
    QString lingua;
    QString pthImage;

    QToolButton *btnLingua;
    QMenu *menuLingua;
    QPushButton *btnImg;
    QLineEdit *txtTitolo;
    QLineEdit *txtData;
    QTextEdit *txtGeneri;
    QTextEdit *txtAutori;
    QTextEdit *txtDescrizione;
    QLineEdit *txtCopie;

    QWidget *cntSpecific;
    QTextEdit *txt1;
    QTextEdit *txt2;
    QTextEdit *txt3;
    QLineEdit *txt4;
    QLineEdit *txt5;
    QLineEdit *txt6;
    QComboBox *cmbJolly;

    void setMedia();

protected:
    void visitMedia() override;

public:
    explicit MediaEditorVisitor(Model::MediaLibrary *library);
    ~MediaEditorVisitor() = default;

    void clear() override;

    void visitAlbum() override;
    void visitFilm() override;
    void visitLibro() override;
    void visitRivista() override;

    // Funzioni per il setting dei nuovi dati inseriti dall'utente
    void setAlbum();
    void setFilm();
    void setLibro();
    void setRivista();

private slots:
    // Slots privati per la validazione dei campi di input in base a criteri stabiliti nella MediaLibrary
    void validateAlbum();
    void validateFilm();
    void validateLibro();
    void validateRivista();

    void onChangeCopertina();
    void onChangeLingua(const QAction *action);

signals:
    void onValidation(const QVector<QString> vctValori);
    void action(const QPersistentModelIndex &index);
};
}
}

#endif // MEDIAEDITORVISITOR_H
