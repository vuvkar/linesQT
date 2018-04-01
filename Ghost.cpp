#include "Ghost.h"

Ghost::Ghost()
{
    currentPlace = MatrixPoint(0, 0);
    type = yellow;
}

void Ghost::configure(Ghosts type, MatrixPoint place)
{
    this->type = type;
    this->currentPlace = place;
    emit(this->wasCreated(type, place));
}
