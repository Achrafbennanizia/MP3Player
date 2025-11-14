# 🎵 MP3 Player

A modern, feature-rich MP3 player built with Qt 6 and C++. This cross-platform application provides an intuitive interface for managing and playing your audio library.

---

## 📌 Description

MP3 Player is a lightweight desktop audio player that supports multiple audio formats including **MP3, WAV, OGG, FLAC, and M4A**.
Built using **Qt 6 Multimedia (FFmpeg backend)**, it offers seamless playback control, playlist management, and an elegant user interface.

---

## ✨ Features

- **Multi-Format Support**: MP3, WAV, OGG, FLAC, M4A
- **Playlist Management**: drag-and-drop reorder, sorting, multi-select
- **Playback Controls**:

  - Play, Pause, Stop
  - Next / Previous

- **Volume & Mute**: realtime volume slider + mute toggle
- **Position Slider**: seek anywhere in the track
- **Auto-Play**: next track automatically plays
- **Keyboard Shortcuts**:

  - **Space**: Play/Pause
  - **Ctrl+O / Cmd+O**: Open files
  - **Ctrl+Q / Cmd+Q**: Quit

- **Status Display**: current playback state
- **Time Display**: mm:ss format (current / total)
- **Playlist Table View**:

  - sortable columns
  - double-click to play

---

## 🖥️ Requirements

### **System Requirements**

- macOS 10.15+, Windows 10+, Linux
- 512 MB RAM
- 50 MB disk space

### **Build Requirements**

- **Qt 6.2+**
- **CMake 3.16+**
- C++17-capable compiler (GCC 7+, Clang 5+, MSVC 2019+)

### **Qt Modules**

- `Qt6::Core`
- `Qt6::Widgets`
- `Qt6::Multimedia`

---

## 🔧 Installation

### **Building from Source**

#### 1. Clone the repository

```bash
git clone https://github.com/yourusername/mp3player.git
cd mp3player
```

#### 2. Create build directory

```bash
mkdir build
cd build
```

#### 3. Configure with CMake

```bash
cmake ..
```

#### 4. Build the project

```bash
cmake --build .
```

#### 5. Run the application

**macOS:**

```bash
./Mp3Player.app/Contents/MacOS/Mp3Player
```

**Linux:**

```bash
./Mp3Player
```

**Windows:**

```bash
Mp3Player.exe
```

---

## 🚀 Using Qt Creator

1. Open **CMakeLists.txt** in Qt Creator
2. Configure using your **Qt 6 Kit**
3. Build (`Ctrl+B` / `Cmd+B`)
4. Run (`Ctrl+R` / `Cmd+R`)

---

## 🎚 Usage

### **Opening Files**

- File → Open
- Or press **Ctrl+O / Cmd+O**
- Choose one or multiple audio files
- Playlist loads and playback starts automatically

### **Playback Controls**

| Action      | Description           |
| ----------- | --------------------- |
| **Play**    | Start/resume playback |
| **Pause**   | Pause track           |
| **Stop**    | Stop and reset        |
| **Next >>** | Next track            |
| **<< Prev** | Previous track        |
| **Mute**    | Toggle mute           |

### **Playlist Navigation**

- Double-click a track to play immediately
- Click column headers to sort
- Drag and drop to reorder

### **Volume**

- Adjust with slider (0–100%)

### **Seek**

- Drag position slider
- Time display shows current/total duration

---

## 🗂 Project Structure

```
Mp3Player/
├── CMakeLists.txt
├── main.cpp
├── mainwindow.h
├── mainwindow.cpp
├── mainwindow.ui
├── playercontroller.h
├── playercontroller.cpp
├── playlistmodel.h
├── playlistmodel.cpp
├── track.h
├── track.cpp
└── README.md
```

---

## 🧱 Architecture

### **MainWindow**

- UI management, menus, shortcuts
- Connects user actions to PlayerController

### **PlayerController**

- Wraps QMediaPlayer + QAudioOutput
- Handles playback, mute, navigation
- Emits track change & playback state signals

### **PlaylistModel**

- Custom `QAbstractTableModel`
- Stores and displays `Track` objects

### **Track**

- Stores:

  - path
  - title
  - artist
  - album
  - duration

---

## ⚙️ Technical Details

### **Qt 6 Migration**

Qt 6 changed multimedia API heavily.
Older Qt5 classes like `QMediaPlaylist` work differently.

Internals use:

- `QList<QUrl>` for track storage
- Manual index management
- `QMediaPlayer::mediaStatusChanged` for auto-play

### **FFmpeg Backend**

- Supports many audio formats
- Hardware acceleration
- Stable cross-platform behavior

---

## 🩺 Troubleshooting

### App won't start

- Check Qt 6 is installed correctly
- Verify FFmpeg support
- Ensure PATH contains Qt libs

### No sound

- Check system volume
- Ensure app volume slider > 0
- Verify audio output device

### Files won't load

- File must be readable
- Format must be supported
- File not corrupted

### Build errors

- Install Qt 6.2+
- Update CMake to 3.16+
- Use a C++17 compiler
- Clean build:

  ```bash
  cmake --build . --clean-first
  ```

---

## ⚠️ Known Limitations

- No ID3 tag extraction
- No equalizer
- No visualizations
- No playlist save/load
- No media library

---

## 📌 Future Enhancements

- ID3 tag reader
- Save/load playlists
- Equalizer
- Visualizers (spectrum, waveform)
- Media database
- Themes / skins
- Lyrics
- Internet radio
- Podcast support

---

## 🤝 Contributing

1. Fork repository
2. Create branch

   ```bash
   git checkout -b feature/amazing-feature
   ```

3. Commit changes

   ```bash
   git commit -m "Add amazing feature"
   ```

4. Push

   ```bash
   git push origin feature/amazing-feature
   ```

5. Open Pull Request

---

## 📄 License

MIT License – see `LICENSE`.

---

## 🙏 Acknowledgments

- Built with Qt Framework
- Audio decoding via FFmpeg
- Inspired by classic desktop audio players

---

## 📬 Contact

**Developer:** Achraf
**Project Link:** [https://github.com/yourusername/mp3player](https://github.com/yourusername/mp3player)

---

## 🏁 Version History

### **v1.0.0 (Current)**

- Initial release
- Basic playback
- Playlist management
- Volume + seek
- Keyboard shortcuts
- Qt 6 support
