#pragma once
#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMediaMetaData>
#include <QList>
#include <QUrl>
#include "playlistmodel.h"

/**
 * @brief Kapselt QMediaPlayer und QAudioOutput.
 */
class PlayerController : public QObject {
    Q_OBJECT
public:
    explicit PlayerController(QObject* parent = nullptr);

    void setModel(PlaylistModel* model);

    void play();
    void pause();
    void stop();
    void next();
    void previous();
    void toggleMute();

    void setVolume(int volume);            // 0-100
    void setPosition(qint64 posMs);
    void playTrack(int index);

    QMediaPlayer* mediaPlayer() { return &m_player; }
    int currentIndex() const { return m_currentIndex; }
    int playlistSize() const { return m_playlist.size(); }

signals:
    void currentTrackChanged(const Track& track);
    void playbackStateChanged(QMediaPlayer::PlaybackState state);
    void currentIndexChanged(int index);

private slots:
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState state);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

private:
    void updateCurrentTrack();

    PlaylistModel* m_model = nullptr;
    QMediaPlayer m_player;
    QAudioOutput m_audioOutput;

    // Playlist management (replaces QMediaPlaylist)
    QList<QUrl> m_playlist;
    int m_currentIndex = -1;
    bool m_isMuted = false;
};
