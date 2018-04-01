#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "Ghost.h"
#include <iostream>
#include "Definitions.h"
#include "mainwindow.h"
#include <cstdlib>
#include <time.h>
#include <vector>
#include <QObject>
#include <queue>
#include <QDebug>

class MainWindow;

class GameLogic: public QObject
{
    Q_OBJECT
public:
    static GameLogic* m_pInstance;
    static MainWindow* window;
    Ghost* **gameBoard;
    std::vector<MatrixPoint> freeCells;
    MatrixPoint* ghostDestination = NULL;
    int score = 0;
    void nextMove();
private:
    Ghost* clickedGhost = NULL;
    GameLogic();
    ~GameLogic();
    void moveGhost(MatrixPoint, MatrixPoint);
    std::vector<Ghost*> findCombinations(MatrixPoint newlyAddedPlace, Ghosts type);
    std::vector<Node> shortestRoad(MatrixPoint from, MatrixPoint to);
    void deleteGhosts(std::vector<Ghost*> ghosts);

public:
    static GameLogic* Instance();
    void generateGhosts();
public slots:
    void cellWasPressed(MatrixPoint point);
};

#endif // GAMELOGIC_H
