#pragma once
#include <QString>

/**
 * @brief Repräsentiert einen Eintrag (Song) in der Wiedergabeliste.
 */
struct Track {
    QString filePath;   ///< Vollständiger Dateipfad
    QString title;      ///< Titel des Songs
    QString artist;     ///< Künstler
    QString album;      ///< Album
    qint64 durationMs;  ///< Dauer in Millisekunden

    /// Hilfsfunktion zur formatierten Dauer (mm:ss)
    QString formattedDuration() const;
};
