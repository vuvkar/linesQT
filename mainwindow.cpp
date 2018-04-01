#include "mainwindow.h"
#include "ui_mainwindow.h"

QImage createImageWithOverlay(const QImage& baseImage, const QImage& overlayImage);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap bkgnd("://Assets/background.png");
    QIcon icon("://Assets/ghost_green.png");
    this->setWindowIcon(icon);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    this->setFixedHeight(sWidth + 100);
    this->setFixedWidth(sWidth);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Exit?",
                                                                tr("Are you sure to close the game?"),
                                                                QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::No);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}

MainWindow::~MainWindow()
{
//    delete board;
//    for(int i = 0; i != boardRow; ++i)
//    {
//        for(int j = 0; j != boardColumn; ++j)
//        {
//            delete[] cells[i][j];
//        }
//        delete[] cells[i];

//    }
//    delete[] cells;
//    delete lastCellPlace;
}

void MainWindow::playButtonWasClicked(bool) {
    startGame();
}

void MainWindow::settingsButtonWasClicked(bool) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit?", "Are you sure to close the game?",
                                  QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        qDebug() << "Yes was clicked";
        QApplication::exit();
    } else {
        qDebug() << "Yes was *not* clicked";
    }

}

void MainWindow::homeButtonWasClicked(bool) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Go back", "You are going to cancel the game. Are you sure?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        qDebug() << "Yes was clicked";
        startScreen();
    } else {
        qDebug() << "Yes was *not* clicked";
    }
}

void MainWindow::buttonWasPressed(QWidget* buttonW) {
    QPushButton* button = (QPushButton*)(buttonW);
    button->setIconSize(QSize(button->iconSize().width() / 2, button->iconSize().height() / 2));
}

void MainWindow::buttonWasReleased(QWidget* buttonW) {
    QPushButton* button = (QPushButton*)(buttonW);
    button->setIconSize(QSize(button->iconSize().width() * 2, button->iconSize().height() * 2));
}

void MainWindow::startScreen() {
    GameLogic::m_pInstance = NULL;
    GameLogic::window = this;

    int id = QFontDatabase::addApplicationFont("://Assets/ConcertOne-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont monospace(family);
    monospace.setPointSize(20);

    QWidget *parent = new QWidget();
    parent->resize(sWidth, sWidth);

    QPushButton* playGame = new QPushButton(QIcon("://Assets/button_play.png"), "Play", parent);
    playGame->move(100, 110);
    playGame->setFont(monospace);
    playGame->setStyleSheet("border: 0px; color: rgb(255, 255, 255)");
    playGame->setIconSize(QSize(200, 200));
    connect(playGame, SIGNAL(clicked(bool)), this, SLOT(playButtonWasClicked(bool)));

    //    QSignalMapper* mapper = new QSignalMapper(this);

    //    connect(playGame, SIGNAL(pressed()), mapper, SLOT(map())) ;
    //    mapper->setMapping(playGame, playGame);

    //    connect(mapper, SIGNAL(mapped(QWidget*)), this, SIGNAL(wasPressed(QWidget*)));

    QPushButton* settings = new QPushButton(QIcon("://Assets/button_home.png"), "Exit", parent);
    settings->move(200, 400);
    settings->setFont(monospace);
    settings->setStyleSheet("border: 0px; color: rgb(255, 255, 255)");
    settings->setIconSize(QSize(200, 200));

    connect(settings, SIGNAL(clicked(bool)), this, SLOT(settingsButtonWasClicked(bool)));

    this->setCentralWidget(parent);
    this->show();
}

void MainWindow::startGame() {
    board = new QWidget();
    cells = new Cell* *[boardRow];
    for(int i = 0; i < boardRow; i++) {
        cells[i] = new Cell*[boardColumn];
    }
    QGridLayout* layout = new QGridLayout();

    this->setCentralWidget(board);
    board->setLayout(layout);
    for(int i = 0; i < boardRow; i++)
        for(int j = 0; j < boardColumn; j++){
            Cell* temp = new Cell();
            temp->place = MatrixPoint(i,j);
            QImage cell;
            (i + j) % 2 == 0 ? cell = QImage("://Assets/cell_light.png") : cell = QImage("://Assets/cell_dark.png");
            temp->setIcon(QIcon(QPixmap::fromImage(cell)));
            temp->setStyleSheet("border: 0px");
            temp->setIconSize(QSize(cellSize, cellSize));


            connect(temp, SIGNAL(wasPressed(MatrixPoint)), GameLogic::Instance(), SLOT(cellWasPressed(MatrixPoint)));
            connect(temp, SIGNAL(clicked(bool)), temp, SLOT(idiotClick(bool)));

            temp->setMinimumSize(cellSize, cellSize);
            layout->addWidget(temp, i, j);
            cells[i][j] = temp;
        }

    int id = QFontDatabase::addApplicationFont("://Assets/ConcertOne-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont monospace(family);
    monospace.setPointSize(30);

    QPushButton* score = new QPushButton(QIcon("://Assets/icon_path.png"), "0", this);
    score->setFont(monospace);
    score->setStyleSheet("border: 0px; color: rgb(255, 255, 255)");
    score->setIconSize(QSize(100, 100));

    layout->addWidget(score, boardRow + 1, 0, 2, 5);
    this->score = score;

    QPushButton* home = new QPushButton(QIcon("://Assets/button_home.png"), "", this);
    home ->setFont(monospace);
    home->setStyleSheet("border: 0px; color: rgb(255, 255, 255)");
    home->setIconSize(QSize(100, 100));

    connect(home, SIGNAL(clicked(bool)), this, SLOT(homeButtonWasClicked(bool)));

    layout->addWidget(home, boardRow + 1, 5, 2, 2);



    GameLogic::Instance()->generateGhosts();

    this->show();
}

void MainWindow::ghostWasMoved(std::vector<Node> road, Ghosts type) {
    QSequentialAnimationGroup* animationManager = new QSequentialAnimationGroup();
    connect(animationManager, SIGNAL(finished()), this, SLOT(finishedAnimating()));
    lastCellPlace = new MatrixPoint(road.back().y, road.back().x);
    lastCellType = type;
    QImage cell;
    (road[0].y + road[0].x) % 2 == 0 ? cell = QImage("://Assets/cell_light.png") : cell = QImage("://Assets/cell_dark.png");
    cells[road[0].y][road[0].x]->setIcon(QIcon(QPixmap::fromImage(cell)));
    for(unsigned int i = 1; i < road.size(); i++) {
        Node nextCell = road[i];
        Cell* current = cells[nextCell.y][nextCell.x];
        QPropertyAnimation *animation = new QPropertyAnimation(current, "iconSize");
        animation->setDuration(50);
        animation->setStartValue(QSize(0, 0));
        animation->setEndValue(QSize(cellSize, cellSize));

        animationManager->addAnimation(animation);
    }
    animationManager->start();
}

void MainWindow::finishedAnimating() {
    if(lastCellPlace != NULL) {
        cells[lastCellPlace->row][lastCellPlace->column]->setIcon(mergedIcon(lastCellType, *lastCellPlace));
        lastCellPlace = NULL;
        GameLogic::Instance()->nextMove();
    }
}

QIcon MainWindow::mergedIcon(Ghosts type, MatrixPoint place, bool select) {
    QImage icon;
    switch (type) {
    case yellow:
        icon = QImage("://Assets/ghost_yellow.png");
        break;
    case white:
        icon = QImage("://Assets/ghost_white.png");
        break;
    case green:
        icon = QImage("://Assets/ghost_green.png");
        break;
    case red:
        icon = QImage("://Assets/ghost_red.png");
        break;
    }
    QImage cellIcon;
    if(select)
        cellIcon = (place.row + place.column) % 2 == 0 ? QImage("://Assets/cell_light.png") : QImage("://Assets/cell_dark.png");
    else
        cellIcon = QImage("://Assets/cell_selected.png");
    QImage merged = createImageWithOverlay(cellIcon, icon);
    return QIcon(QPixmap::fromImage(merged));

}

void MainWindow::ghostWasGenerated(Ghosts type, MatrixPoint place){
    Cell* current = cells[place.row][place.column];

    current->setIcon(mergedIcon(type, place));

    QPropertyAnimation *animation = new QPropertyAnimation(cells[place.row][place.column], "iconSize");
    animation->setDuration(100);
    animation->setStartValue(QSize(0, 0));
    animation->setEndValue(QSize(cellSize, cellSize));
    animation->start();
}

void MainWindow::ghostWasDeleted(MatrixPoint place) {
    Cell* temp = cells[place.row][place.column];
    if((place.row + place.column) % 2 == 0)
        temp->setIcon(QIcon("://Assets/cell_light.png"));
    else
        temp->setIcon(QIcon("://Assets/cell_dark.png"));
    temp->setIconSize(QSize(cellSize, cellSize));
}

void MainWindow::ghostWasSelected(Ghosts type, MatrixPoint place) {
    if(GameLogic::Instance()->gameBoard[place.row][place.column])
        cells[place.row][place.column]->setIcon(mergedIcon(type, place, false));
}

void MainWindow::ghostWasDeselected(Ghosts type, MatrixPoint place) {
    if(GameLogic::Instance()->gameBoard[place.row][place.column])
        cells[place.row][place.column]->setIcon(mergedIcon(type, place, true));
}

void MainWindow::gameOver() {
    GameLogic::m_pInstance  = NULL;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Game Over", "You have lost. Do you want to try again?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        startGame();
    } else {
        startScreen();
    }
}

QImage createImageWithOverlay(const QImage& baseImage, const QImage& overlayImage)
{
    QImage imageWithOverlay = QImage(overlayImage.size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&imageWithOverlay);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(imageWithOverlay.rect(), Qt::transparent);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, baseImage);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, overlayImage);

    painter.end();

    return imageWithOverlay;
}
