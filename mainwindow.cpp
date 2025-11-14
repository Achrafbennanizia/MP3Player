#include "mainwindow.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTableView>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QShortcut>
#include <QKeySequence>
#include <QHeaderView>
#include <QFileInfo>
#include <QTime>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // Medienobjekte
    m_player      = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);

    // Model
    m_model = new PlaylistModel(this);

    setupUi();
    setupMenu();
    setupConnections();
    updateStatusLabel();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi()
{
    setWindowTitle("MP3-Player");
    auto* central = new QWidget(this);
    auto* mainLayout = new QVBoxLayout(central);

    // Playlist-View
    m_view = new QTableView(central);
    m_view->setModel(m_model);
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_view->setSelectionMode(QAbstractItemView::SingleSelection);
    m_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_view->setSortingEnabled(true);
    m_view->horizontalHeader()->setStretchLastSection(true);
    mainLayout->addWidget(m_view);

    // Position + Zeit
    auto* posLayout = new QHBoxLayout();
    m_positionSlider = new QSlider(Qt::Horizontal, central);
    m_positionSlider->setRange(0, 0);
    m_timeLabel = new QLabel("00:00 / 00:00", central);
    posLayout->addWidget(m_positionSlider);
    posLayout->addWidget(m_timeLabel);
    mainLayout->addLayout(posLayout);

    // Steuerleiste
    auto* controlLayout = new QHBoxLayout();
    m_prevButton  = new QPushButton("<<", central);
    m_playButton  = new QPushButton("Play", central);
    m_pauseButton = new QPushButton("Pause", central);
    m_stopButton  = new QPushButton("Stop", central);
    m_nextButton  = new QPushButton(">>", central);
    m_muteButton  = new QPushButton("Mute", central);

    controlLayout->addWidget(m_prevButton);
    controlLayout->addWidget(m_playButton);
    controlLayout->addWidget(m_pauseButton);
    controlLayout->addWidget(m_stopButton);
    controlLayout->addWidget(m_nextButton);
    controlLayout->addWidget(m_muteButton);

    // Lautstärke
    m_volumeSlider = new QSlider(Qt::Horizontal, central);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(50); // Standard
    controlLayout->addWidget(new QLabel("Vol:", central));
    controlLayout->addWidget(m_volumeSlider);

    mainLayout->addLayout(controlLayout);
    setCentralWidget(central);

    // Statusleiste
    m_statusLabel = new QLabel("Bereit", this);
    statusBar()->addPermanentWidget(m_statusLabel, 1);
}

void MainWindow::setupMenu()
{
    auto* fileMenu = menuBar()->addMenu("Datei");

    auto* openAction = new QAction("Öffnen...", this);
    openAction->setShortcut(QKeySequence::Open); // Ctrl+O
    connect(openAction, &QAction::triggered, this, &MainWindow::openFiles);

    auto* quitAction = new QAction("Beenden", this);
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, this, &MainWindow::close);

    fileMenu->addAction(openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);
}

void MainWindow::setupConnections()
{
    // Buttons
    connect(m_playButton,  &QPushButton::clicked, this, &MainWindow::play);
    connect(m_pauseButton, &QPushButton::clicked, this, &MainWindow::pause);
    connect(m_stopButton,  &QPushButton::clicked, this, &MainWindow::stop);
    connect(m_nextButton,  &QPushButton::clicked, this, &MainWindow::next);
    connect(m_prevButton,  &QPushButton::clicked, this, &MainWindow::previous);
    connect(m_muteButton,  &QPushButton::clicked, this, &MainWindow::toggleMute);

    // Slider
    connect(m_volumeSlider, &QSlider::valueChanged,
            this, &MainWindow::onVolumeChanged);
    connect(m_positionSlider, &QSlider::sliderMoved,
            this, &MainWindow::onSliderMoved);

    // Player-Signale
    connect(m_player, &QMediaPlayer::positionChanged,
            this, &MainWindow::onPositionChanged);
    connect(m_player, &QMediaPlayer::durationChanged,
            this, &MainWindow::onDurationChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged,
            this, &MainWindow::onMediaStatusChanged);

    // Playlist-Doppelklick
    connect(m_view, &QTableView::doubleClicked,
            this, &MainWindow::onPlaylistRowDoubleClicked);

    // Tastenkürzel: Space → Play/Pause
    auto* spaceShortcut = new QShortcut(QKeySequence(Qt::Key_Space), this);
    connect(spaceShortcut, &QShortcut::activated, [this]() {
        if (m_player->playbackState() == QMediaPlayer::PlayingState)
            pause();
        else
            play();
    });
}

void MainWindow::openFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(
        this,
        "MP3-Dateien öffnen",
        QString(),
        "Audio Files (*.mp3 *.wav *.ogg *.flac *.m4a)"
        );

    if (files.isEmpty())
        return;

    // Playlist + Model leeren
    m_playlist.clear();
    m_model->clear();
    m_currentIndex = -1;

    for (const QString& path : files) {
        QUrl url = QUrl::fromLocalFile(path);
        m_playlist.append(url);

        Track t;
        t.filePath = path;
        t.title = QFileInfo(path).baseName();
        t.artist = "";
        t.album = "";
        t.durationMs = 0;
        m_model->addTrack(t);
    }

    if (!m_playlist.isEmpty()) {
        playTrack(0);
    }
}

void MainWindow::play()
{
    if (m_currentIndex == -1 && !m_playlist.isEmpty()) {
        playTrack(0);
    } else {
        m_player->play();
    }
    updateStatusLabel();
}

void MainWindow::pause()
{
    m_player->pause();
    updateStatusLabel();
}

void MainWindow::stop()
{
    m_player->stop();
    updateStatusLabel();
}

void MainWindow::next()
{
    if (m_currentIndex < m_playlist.size() - 1) {
        playTrack(m_currentIndex + 1);
    }
}

void MainWindow::previous()
{
    if (m_currentIndex > 0) {
        playTrack(m_currentIndex - 1);
    }
}

void MainWindow::playTrack(int index)
{
    if (index < 0 || index >= m_playlist.size())
        return;

    m_currentIndex = index;
    m_player->setSource(m_playlist[index]);
    m_player->play();

    // Highlight current track in table
    m_view->selectRow(index);

    updateStatusLabel();
}

void MainWindow::toggleMute()
{
    m_isMuted = !m_isMuted;
    m_audioOutput->setMuted(m_isMuted);
    m_muteButton->setText(m_isMuted ? "Unmute" : "Mute");
}

void MainWindow::onPositionChanged(qint64 pos)
{
    if (!m_positionSlider->isSliderDown()) {
        m_positionSlider->setValue(static_cast<int>(pos));
    }
    m_timeLabel->setText(
        QString("%1 / %2")
            .arg(formatTime(pos))
            .arg(formatTime(m_player->duration()))
        );
}

void MainWindow::onDurationChanged(qint64 dur)
{
    m_positionSlider->setRange(0, static_cast<int>(dur));
    m_timeLabel->setText(
        QString("%1 / %2")
            .arg(formatTime(m_player->position()))
            .arg(formatTime(dur))
        );
}

void MainWindow::onVolumeChanged(int value)
{
    m_audioOutput->setVolume(value / 100.0); // 0..1
}

void MainWindow::onSliderMoved(int value)
{
    m_player->setPosition(static_cast<qint64>(value));
}

void MainWindow::onPlaylistRowDoubleClicked(const QModelIndex& index)
{
    if (!index.isValid())
        return;

    int row = index.row();
    playTrack(row);
}

void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    // Auto-play next track when current finishes
    if (status == QMediaPlayer::EndOfMedia) {
        next();
    }
}

void MainWindow::updateStatusLabel()
{
    QString state;
    switch (m_player->playbackState()) {
    case QMediaPlayer::PlayingState: state = "Wiedergabe"; break;
    case QMediaPlayer::PausedState:  state = "Pause"; break;
    case QMediaPlayer::StoppedState: state = "Stopp"; break;
    }

    QString trackInfo;
    if (m_currentIndex >= 0 && m_currentIndex < m_playlist.size()) {
        trackInfo = QString(" - Track %1/%2").arg(m_currentIndex + 1).arg(m_playlist.size());
    }

    m_statusLabel->setText(QString("Status: %1%2").arg(state).arg(trackInfo));
}

QString MainWindow::formatTime(qint64 ms) const
{
    QTime t(0, 0, 0);
    t = t.addMSecs(ms);
    return t.toString("mm:ss");
}
