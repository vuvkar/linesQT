#ifndef CELL_H
#define CELL_H

#include <QPushButton>
#include <Definitions.h>

class Cell: public QPushButton
{
    Q_OBJECT
public:
    Cell();
    MatrixPoint place;
public slots:
    void idiotClick(bool zrtik);
signals:
    void wasPressed(MatrixPoint place);

};

#endif // CELL_H
