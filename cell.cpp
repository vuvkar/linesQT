#include "cell.h"

Cell::Cell()
{
    place = MatrixPoint();
}

void Cell::idiotClick(bool) {
    emit(wasPressed(this->place));
}
