#include "track.h"
#include <QTime>

QString Track::formattedDuration() const
{
    QTime t(0, 0);
    t = t.addMSecs(durationMs);
    return t.toString("mm:ss");
}
