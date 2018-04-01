#ifndef GHOST_H
#define GHOST_H

#include <QtCore>
#include "Definitions.h"

enum Ghosts { red, green, white, yellow };

class Ghost: public QObject
{
    Q_OBJECT

signals:
    void wasSelected(Ghosts type, MatrixPoint place);
    void wasDeselected(Ghosts type, MatrixPoint place);
    void wasCreated(Ghosts type, MatrixPoint place);
    void wasMoved(std::vector<Node> road, Ghosts type);
    void deleteYourselfAziz(MatrixPoint place);

public:
    Ghosts type;
    MatrixPoint currentPlace;
    bool isSet = false;

    Ghost();
    void configure(Ghosts type, MatrixPoint place);

};

#endif // GHOST_H
