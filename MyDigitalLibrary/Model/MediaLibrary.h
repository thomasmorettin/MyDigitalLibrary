/**
 * @file MediaLibrary.h/.cpp
 * @brief Controller - Classe (estende QAbstractListModel) per la memorizzazione e l'esecuzione
 * di tutte le operazioni su QList di Media.
 * NB La ridefinizione dei metodi rowCount e data è OBBLIGATORIA per estendere la classe base.
 * @author Thomas Morettin
 */

#ifndef MEDIALIBRARY_H
#define MEDIALIBRARY_H

#include "Items/Media.h"

#include <QAbstractListModel>

namespace Model {
class MediaLibrary : public QAbstractListModel {
    Q_OBJECT

private:
    QList<Items::Media*> lstMedia;      // Collezione di Media

    QList<QString> validateMedia(const QString &lingua,
                                 const QString &titolo,
                                 const QString &descrizione,
                                 const QString &data,
                                 const QString &generi,
                                 const QString &autori,
                                 const unsigned int copie);

public:
    enum mediaRoles {
        TipoRole = Qt::UserRole + 1,       // Evita conflitti con i ruoli Qt-predefiniti
        ImageRole,
        TitoloRole,
        DataRole,
        GenereRole,
        AutoreRole,
        LinguaRole,
        DescrizioneRole,
        CopieRole,
        // Ruoli specifici per tipo di Media
        I_Role,
        II_Role,
        III_Role,
        IV_Role,
        V_Role,
        VI_Role,
    };      // enum per i ruoli di ciascun dato

    explicit MediaLibrary(QObject *parent = nullptr);
    ~MediaLibrary();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Items::Media *getMedia(const QModelIndex &index) const;

    QList<QModelIndex> findMediaLstIndex(const int row, const QString &titolo, const QString &lingua, const QString &tipo) const;
    void addNewMedia(Items::Media *media);
    void changeNumCopie(const QModelIndex &index, const unsigned int copie);
    QModelIndex addMediaByType(const QString &tipo);
    void deleteMedia(const int row);
    void eraseLibrary();

    QList<QString> validateAlbum(const QString &lingua,
                                 const QString &titolo,
                                 const QString &descrizione,
                                 const QString &data,
                                 const QString &generi,
                                 const QString &autori,
                                 const unsigned int copie,
                                 const QString &canzoni,
                                 const unsigned int durata,
                                 const QString &casa);
    QList<QString> validateFilm(const QString &lingua,
                                const QString &titolo,
                                const QString &descrizione,
                                const QString &data,
                                const QString &generi,
                                const QString &autori,
                                const unsigned int copie,
                                const QString &registi,
                                const unsigned int durata,
                                const QString &caseProd,
                                const unsigned int numero);
    QList<QString> validateLibro(const QString &lingua,
                                 const QString &titolo,
                                 const QString &descrizione,
                                 const QString &data,
                                 const QString &generi,
                                 const QString &autori,
                                 const unsigned int copie,
                                 const QString &isbn,
                                 const unsigned int numPag,
                                 const QString &casa);
    QList<QString> validateRivista(const QString &lingua,
                                   const QString &titolo,
                                   const QString &descrizione,
                                   const QString &data,
                                   const QString &generi,
                                   const QString &autori,
                                   const unsigned int copie,
                                   const unsigned int numPag,
                                   const QString &casa);
};
}

#endif // MEDIALIBRARY_H
