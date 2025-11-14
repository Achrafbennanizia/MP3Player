// core/playlistmodel.cpp
#include "playlistmodel.h"

PlaylistModel::PlaylistModel(QObject* parent)
    : QAbstractTableModel(parent)
{}

int PlaylistModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_tracks.size();
}

int PlaylistModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return 4; // Titel, Künstler, Album, Dauer
}

QVariant PlaylistModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return {};

    const Track& t = m_tracks[index.row()];

    switch (index.column()) {
    case 0: return t.title;
    case 1: return t.artist;
    case 2: return t.album;
    case 3: return t.formattedDuration();
    default: return {};
    }
}

QVariant PlaylistModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return {};

    switch (section) {
    case 0: return "Titel";
    case 1: return "Künstler";
    case 2: return "Album";
    case 3: return "Dauer";
    default: return {};
    }
}

Qt::ItemFlags PlaylistModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;

    // Drag & Drop vorbereiten
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled |
           Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

void PlaylistModel::addTrack(const Track& track)
{
    beginInsertRows(QModelIndex(), m_tracks.size(), m_tracks.size());
    m_tracks.push_back(track);
    endInsertRows();
}

void PlaylistModel::clear()
{
    beginResetModel();
    m_tracks.clear();
    endResetModel();
}

const Track& PlaylistModel::trackAt(int row) const
{
    return m_tracks[row];
}

int PlaylistModel::findByTitle(const QString& title) const
{
    for (int i = 0; i < m_tracks.size(); ++i) {
        if (m_tracks[i].title.compare(title, Qt::CaseInsensitive) == 0)
            return i;
    }
    return -1;
}

void PlaylistModel::sort(int column, Qt::SortOrder order)
{
    // TODO: Implementiere Sortierung nach Spalte (Titel/Künstler/Album/Dauer)
    // mithilfe von std::sort und einem passenden Lambda.
}
