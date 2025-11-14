#pragma once

#include <QAbstractTableModel>
#include <QVector>
#include "track.h"

/**
 * @brief Model für die Wiedergabeliste, basierend auf QAbstractTableModel.
 */
class PlaylistModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit PlaylistModel(QObject* parent = nullptr);

    // Grundlegende Model-Funktionen
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Sortierung
    void sort(int column, Qt::SortOrder order) override;

    // Songs verwalten
    void addTrack(const Track& track);
    void clear();
    const Track& trackAt(int row) const;

    int findByTitle(const QString& title) const;

private:
    QVector<Track> m_tracks;
};
