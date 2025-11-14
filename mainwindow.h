#pragma once
#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QList>
#include <QUrl>

class QTableView;
class QSlider;
class QLabel;
class QPushButton;

#include "playlistmodel.h"   // dein Model mit Track-Liste

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void openFiles();
    void play();
    void pause();
    void stop();
    void next();
    void previous();
    void toggleMute();
    void onPositionChanged(qint64 pos);
    void onDurationChanged(qint64 dur);
    void onVolumeChanged(int value);
    void onSliderMoved(int value);
    void onPlaylistRowDoubleClicked(const QModelIndex& index);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

private:
    void setupUi();
    void setupMenu();
    void setupConnections();
    void updateStatusLabel();
    void playTrack(int index);
    QString formatTime(qint64 ms) const;

    // Audio / Medien
    QMediaPlayer*   m_player = nullptr;
    QAudioOutput*   m_audioOutput = nullptr;

    // Playlist management (replaces QMediaPlaylist)
    QList<QUrl> m_playlist;
    int m_currentIndex = -1;

    // Model + View
    PlaylistModel* m_model = nullptr;
    QTableView*    m_view  = nullptr;

    // Steuer-Widgets
    QPushButton* m_playButton = nullptr;
    QPushButton* m_pauseButton = nullptr;
    QPushButton* m_stopButton = nullptr;
    QPushButton* m_nextButton = nullptr;
    QPushButton* m_prevButton = nullptr;
    QPushButton* m_muteButton = nullptr;

    QSlider* m_positionSlider = nullptr;
    QSlider* m_volumeSlider   = nullptr;

    QLabel* m_statusLabel = nullptr;
    QLabel* m_timeLabel   = nullptr;

    bool m_isMuted = false;
};
