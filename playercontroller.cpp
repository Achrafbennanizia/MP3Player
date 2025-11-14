#include "playercontroller.h"
#include <QUrl>
#include <QFileInfo>

/**
 * @brief Konstruktor. Initialisiert Player und AudioOutput
 *        und verbindet interne Signale.
 * @param parent Elternobjekt
 */
PlayerController::PlayerController(QObject* parent)
    : QObject(parent)
    , m_player(this)
    , m_audioOutput(this)
{
    m_player.setAudioOutput(&m_audioOutput);

    // Wenn sich der Wiedergabestatus ändert (Playing, Paused, Stopped)
    connect(&m_player, &QMediaPlayer::playbackStateChanged,
            this, &PlayerController::onPlaybackStateChanged);

    // Wenn ein Medium zu Ende ist
    connect(&m_player, &QMediaPlayer::mediaStatusChanged,
            this, &PlayerController::onMediaStatusChanged);
}

/**
 * @brief Verknüpft das PlaylistModel mit dem PlayerController
 *        und baut die Playlist basierend auf den Tracks im Model.
 * @param model Zeiger auf das PlaylistModel
 */
void PlayerController::setModel(PlaylistModel* model)
{
    m_model = model;

    // Playlist leeren und neu aufbauen
    m_playlist.clear();
    m_currentIndex = -1;

    if (!m_model)
        return;

    int rows = m_model->rowCount();
    for (int row = 0; row < rows; ++row) {
        const Track& t = m_model->trackAt(row);
        QUrl url = QUrl::fromLocalFile(t.filePath);
        m_playlist.append(url);
    }

    // Standard: bei Index 0 beginnen
    if (rows > 0) {
        playTrack(0);
    }
}

/**
 * @brief Spielt einen bestimmten Track ab.
 * @param index Index des Tracks in der Playlist
 */
void PlayerController::playTrack(int index)
{
    if (index < 0 || index >= m_playlist.size())
        return;

    m_currentIndex = index;
    m_player.setSource(m_playlist[index]);
    m_player.play();

    emit currentIndexChanged(index);
    updateCurrentTrack();
}

/**
 * @brief Startet bzw. setzt die Wiedergabe fort.
 */
void PlayerController::play()
{
    if (m_currentIndex == -1 && !m_playlist.isEmpty()) {
        playTrack(0);
    } else {
        m_player.play();
    }
}

/**
 * @brief Pausiert die Wiedergabe.
 */
void PlayerController::pause()
{
    m_player.pause();
}

/**
 * @brief Stoppt die Wiedergabe.
 */
void PlayerController::stop()
{
    m_player.stop();
}

/**
 * @brief Spielt den nächsten Track in der Playlist ab.
 */
void PlayerController::next()
{
    if (m_currentIndex < m_playlist.size() - 1) {
        playTrack(m_currentIndex + 1);
    }
}

/**
 * @brief Spielt den vorherigen Track in der Playlist ab.
 */
void PlayerController::previous()
{
    if (m_currentIndex > 0) {
        playTrack(m_currentIndex - 1);
    }
}

/**
 * @brief Schaltet Stummschaltung um (toggle).
 */
void PlayerController::toggleMute()
{
    m_isMuted = !m_isMuted;
    m_audioOutput.setMuted(m_isMuted);
}

/**
 * @brief Setzt die Lautstärke.
 * @param volume Lautstärke in Prozent [0..100]
 */
void PlayerController::setVolume(int volume)
{
    // QAudioOutput erwartet 0.0 bis 1.0
    m_audioOutput.setVolume(volume / 100.0);
}

/**
 * @brief Setzt die Wiedergabeposition.
 * @param posMs Position in Millisekunden
 */
void PlayerController::setPosition(qint64 posMs)
{
    m_player.setPosition(posMs);
}

/**
 * @brief Aktualisiert das currentTrackChanged-Signal basierend auf dem aktuellen Index.
 */
void PlayerController::updateCurrentTrack()
{
    if (!m_model)
        return;
    if (m_currentIndex < 0 || m_currentIndex >= m_model->rowCount())
        return;

    const Track& t = m_model->trackAt(m_currentIndex);
    emit currentTrackChanged(t);
}

/**
 * @brief Slot: wird aufgerufen, wenn sich der Wiedergabestatus ändert.
 *        Leitet den Status als Signal playbackStateChanged weiter.
 * @param state neuer Wiedergabestatus
 */
void PlayerController::onPlaybackStateChanged(QMediaPlayer::PlaybackState state)
{
    emit playbackStateChanged(state);
}

/**
 * @brief Slot: wird aufgerufen, wenn sich der Media-Status ändert.
 *        Spielt automatisch den nächsten Track, wenn das aktuelle Medium endet.
 * @param status neuer Media-Status
 */
void PlayerController::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        // Automatisch nächsten Track abspielen
        next();
    }
}
