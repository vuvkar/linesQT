#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GameLogic.h"
#include <QGridLayout>
#include "cell.h"
#include "Definitions.h"
#include <QPainter>
#include <QAnimationDriver>
#include <QAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QButtonGroup>
#include <QFontDatabase>
#include <QMessageBox>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QWidget* board = new QWidget();
    Cell* **cells;
    Ghosts lastCellType;
    MatrixPoint* lastCellPlace;
    void closeEvent(QCloseEvent *bar);

public slots:
    void ghostWasSelected(Ghosts type, MatrixPoint place);
    void ghostWasDeselected(Ghosts type, MatrixPoint place);
    void ghostWasGenerated(Ghosts type, MatrixPoint place);
    void ghostWasMoved(std::vector<Node> road, Ghosts type);
    void ghostWasDeleted(MatrixPoint place);
    void finishedAnimating();

    void playButtonWasClicked(bool zrtik);
    void settingsButtonWasClicked(bool zrtik);
    void homeButtonWasClicked(bool zrtik);

    void buttonWasPressed(QWidget* button);
    void buttonWasReleased(QWidget* button);

signals:
    wasPressed(QWidget* button);
    wasReleased(QPushButton* button);

public:
    explicit MainWindow(QWidget *parent = 0);
    QPushButton* score;

    void gameOver();
    void startScreen();
    void startGame();
    void nextMove();
    QIcon mergedIcon(Ghosts type, MatrixPoint place, bool select = true);
    Ui::MainWindow *ui;
    ~MainWindow();
};

#endif // MAINWINDOW_H
