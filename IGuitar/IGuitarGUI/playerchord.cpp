#include "playerchord.h"

PlayerChord::PlayerChord()
{
}

QString PlayerChord::getName() const
{
    return name;
}

int PlayerChord::getTime() const
{
    return time;
}

void PlayerChord::setName(const QString n) {
    name = n;
}

void PlayerChord::setTime(const int t) {
    time = t;
}
