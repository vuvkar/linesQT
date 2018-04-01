#include "GameLogic.h"


MainWindow* GameLogic::window = NULL;
GameLogic* GameLogic::m_pInstance = NULL;


bool isValid(int visited[][boardColumn], int row, int col)
{
    return (row >= 0) && (row < boardRow) && (col >= 0) && (col < boardColumn)
            && GameLogic::Instance()->gameBoard[row][col] == NULL && visited[row][col] == -1;
}

GameLogic::GameLogic()
{
    gameBoard = new Ghost**[boardRow];
    for(int i = 0; i < boardRow; i++) {
        gameBoard[i] = new Ghost*[boardColumn];
        for(int j = 0; j < boardColumn; j++)
        {
            gameBoard[i][j] = NULL;
            MatrixPoint temp(i,j);
            freeCells.push_back(temp);
        }
    }
}

GameLogic* GameLogic::Instance() {
    if(!m_pInstance)
        m_pInstance = new GameLogic();
    return m_pInstance;
}

GameLogic::~GameLogic() {
//    for(int i = 0; i != boardRow; ++i)
//    {
//        for(int j = 0; j != boardColumn; ++j)
//        {
//            delete[] gameBoard[i][j];
//        }
//        delete[] gameBoard[i];
//    }
//    delete[] gameBoard;
}

void GameLogic::generateGhosts() {
    if(freeCells.size() > ghostsAtMove ){
        srand(time(0));
        std::vector<Ghost*> deletableGhosts;
        for(int i = 0; i < ghostsAtMove; i++){
            int cellNumber = rand() % freeCells.size();
            Ghosts type = Ghosts(rand() % ghostTypeCount);
            Ghost* temp = new Ghost();
            QObject::connect(temp, SIGNAL(wasSelected(Ghosts, MatrixPoint)), window, SLOT(ghostWasSelected(Ghosts, MatrixPoint)));
            QObject::connect(temp, SIGNAL(wasDeselected(Ghosts, MatrixPoint)), window, SLOT(ghostWasDeselected(Ghosts, MatrixPoint)));
            QObject::connect(temp, SIGNAL(wasCreated(Ghosts, MatrixPoint)), window, SLOT(ghostWasGenerated(Ghosts, MatrixPoint)));
            QObject::connect(temp, SIGNAL(wasMoved(std::vector<Node>,Ghosts)), window, SLOT(ghostWasMoved(std::vector<Node>,Ghosts)));
            QObject::connect(temp, SIGNAL(deleteYourselfAziz(MatrixPoint)), window, SLOT(ghostWasDeleted(MatrixPoint)));
            temp->configure(type, freeCells[cellNumber]);
            gameBoard[freeCells[cellNumber].row][freeCells[cellNumber].column] = temp;
            freeCells.erase(freeCells.begin() + cellNumber);
            std::vector<Ghost*> ghosts = findCombinations(temp->currentPlace, type);
            for(unsigned int i = 0; i < ghosts.size(); i++)
            {
                bool wasFound = false;
                for(unsigned int j = 0; j < deletableGhosts.size(); j++)
                    if(deletableGhosts[j]->currentPlace.row == ghosts[i]->currentPlace.row
                            && deletableGhosts[j]->currentPlace.column == ghosts[i]->currentPlace.column)
                    {
                        wasFound = true;
                        break;
                    }

                if(!wasFound)
                    deletableGhosts.push_back(ghosts[i]);
            }
        }
        deleteGhosts(deletableGhosts);
    }
    else
        window->gameOver();
}

std::vector<Node> GameLogic::shortestRoad(MatrixPoint begin, MatrixPoint end) {
    int moveByRow[] = { -1, 0, 0, 1 };
    int moveByColumn[] = { 0, -1, 1, 0 };
    bool visited[boardRow][boardColumn];
    memset(visited, false, sizeof(visited));
    std::queue<Node> queue;
    visited[begin.row][begin.column] = true;
    Node initialNode({begin.column, begin.row, 0});
    queue.push(initialNode);
    int minimumDistance = INT_MAX;
    std::vector<Node> road; //here we take all the nodes we have visited
    road.push_back(initialNode);
    while(queue.size() != 0)
    {
        Node node = queue.front();
        queue.pop();
        int currentColumn = node.x;
        int currentRow = node.y;
        int distance = node.distance;
        if (currentRow == end.row && currentColumn == end.column)
        {
            minimumDistance = distance; //so if we have found the road we should start cleaning our road array keeping cells which will be used for constructing road
            for(unsigned int i = 0; i < road.size(); i++) //this cleans the "headlike" cells :D
                if(road[i].distance >= minimumDistance && (road[i].x != end.column || road[i].y != end.row)){
                    road.erase(road.begin() + i);
                    i--;
                }
            Node head = node;
            while(head.x != begin.column || head.y != begin.row)
            {
                int tempDistance = head.distance;
                for(unsigned int i = 0; i < road.size(); i++){ //backtracking
                    bool isNeighbour = false;
                    if((road[i].y == head.y && road[i].x == (head.x + 1)) || (road[i].y == head.y && road[i].x == (head.x - 1)) || (road[i].y == (head.y - 1) && road[i].x == head.x) || (road[i].y == (head.y + 1) && road[i].x == head.x))
                        isNeighbour = true;
                    if(road[i].distance == (tempDistance - 1) && !isNeighbour){
                        road.erase(road.begin() + i);
                        i--;
                    }
                }
                for(unsigned int i = 0; i < road.size(); i++)
                    if(road[i].distance == tempDistance - 1){
                        head = road[i];
                        break;
                    }
            }
            break;
        }
        for (int k = 0; k < 4; k++) //checking for each move
        {
            bool freeCell = false;
            bool notVisitedCell = false;
            bool validCell = ((currentRow + moveByRow[k] >= 0) && (currentRow + moveByRow[k] < boardRow) && (currentColumn + moveByColumn[k] >= 0) && (currentColumn + moveByColumn[k] < boardColumn));
            if(validCell)
            {
                freeCell = (gameBoard[currentRow + moveByRow[k]][currentColumn + moveByColumn[k]] == NULL);
                notVisitedCell = (!visited[currentRow + moveByRow[k]][currentColumn + moveByColumn[k]]);

            }
            if (freeCell && validCell && notVisitedCell)
            {
                visited[currentRow + moveByRow[k]][currentColumn + moveByColumn[k]] = true;
                Node temp = {currentColumn + moveByColumn[k], currentRow + moveByRow[k], distance +1};
                queue.push(temp);
                road.push_back(temp);
            }
        }
    }
    if (minimumDistance != INT_MAX){
        qDebug() << "shortest road's length " << minimumDistance;
    }
    else{
        qDebug() << "unavailable" << minimumDistance;
        return std::vector<Node>();
    }

    return road;
}

void GameLogic::cellWasPressed(MatrixPoint point) {
    if(clickedGhost != NULL) {
        if(gameBoard[point.row][point.column] == NULL)
        {
            moveGhost(clickedGhost->currentPlace, point);
            clickedGhost = NULL;
        }
        else if(point.row == clickedGhost->currentPlace.row && point.column == clickedGhost->currentPlace.column){
            emit(clickedGhost->wasDeselected(clickedGhost->type, point));
            clickedGhost = NULL;
        }
        else
        {
            emit(clickedGhost->wasDeselected(clickedGhost->type, clickedGhost->currentPlace));
            clickedGhost = gameBoard[point.row][point.column];
            emit(clickedGhost->wasSelected(clickedGhost->type, point));
        }
    }
    else {
        if(gameBoard[point.row][point.column] != NULL) {
            clickedGhost = gameBoard[point.row][point.column];
            emit(clickedGhost->wasSelected(clickedGhost->type, point));
        }
    }
}

void GameLogic::moveGhost(MatrixPoint from, MatrixPoint to) {
    ghostDestination = new MatrixPoint(to.row, to.column);
    std::vector<Node> road = shortestRoad(from, to);
    if(!road.empty()) {
        clickedGhost->currentPlace = to;
        gameBoard[from.row][from.column] = NULL;
        gameBoard[to.row][to.column] = clickedGhost;
        freeCells.push_back(from);
        for(unsigned int i = 0; i < freeCells.size(); i++)
        {
            if (freeCells[i].row == to.row && freeCells[i].column == to.column){
                freeCells.erase(freeCells.begin() + i);
                break;
            }
        }
        int zrtik = INT_MAX;
        for(unsigned int i = 0; i < road.size(); i++) {
            if(road[i].distance != zrtik)
                zrtik = road[i].distance;
            else {
                road.erase(road.begin() + i);
                i--;
            }
        }
        emit(clickedGhost->wasMoved(road, clickedGhost->type));

    }
    else {
        cellWasPressed(from);
    }
}

void GameLogic::nextMove() {
    if(ghostDestination){
        MatrixPoint to = *ghostDestination;
        std::vector<Ghost*> temp = findCombinations(to, gameBoard[to.row][to.column]->type);
        deleteGhosts(temp);
        if(temp.size() == 0)
            generateGhosts();
    }
}

void GameLogic::deleteGhosts(std::vector<Ghost*> temp) {
    for(unsigned int i = 0; i < temp.size(); i ++) {
        emit(temp[i]->deleteYourselfAziz(temp[i]->currentPlace));
        gameBoard[temp[i]->currentPlace.row][temp[i]->currentPlace.column] = NULL;
        freeCells.push_back(temp[i]->currentPlace);
    }
    score += temp.size();
    window->score->setText(QString::number(score));
}


std::vector<Ghost*> GameLogic::findCombinations(MatrixPoint newlyAddedPlace, Ghosts type) {
    std::vector<Ghost*> deletableGhosts;
    int rowRight = 0;
    for(int i = newlyAddedPlace.column + 1; i < boardColumn; i++){
        if(gameBoard[newlyAddedPlace.row][i] == NULL)
            break;
        if(gameBoard[newlyAddedPlace.row][i]->type == type)
            rowRight++;
        else
            break;
    }
    int rowLeft = 0;
    for(int i = newlyAddedPlace.column - 1; i >= 0 ; i--) {
        if(gameBoard[newlyAddedPlace.row][i] == NULL)
            break;
        if(gameBoard[newlyAddedPlace.row][i]->type == type)
            rowLeft++;
        else
            break;
    }
    int columnDown = 0;
    for(int i = newlyAddedPlace.row + 1; i < boardRow; i++) {
        if(gameBoard[i][newlyAddedPlace.column] == NULL)
            break;
        if(gameBoard[i][newlyAddedPlace.column]->type == type)
            columnDown++;
        else
            break;
    }
    int columnUp = 0;
    for(int i = newlyAddedPlace.row - 1; i >= 0; i--) {
        if(gameBoard[i][newlyAddedPlace.column] == NULL)
            break;
        if(gameBoard[i][newlyAddedPlace.column]->type == type)
            columnUp++;
        else
            break;
    }
    bool wasFound = false;
    if (columnDown + columnUp >= ghostBoomCount - 1) {
        for(int i = 0; i < columnUp; i++) {
            deletableGhosts.push_back(gameBoard[newlyAddedPlace.row - 1 - i][newlyAddedPlace.column]);
        }
        for(int i = 0; i < columnDown; i++) {
            deletableGhosts.push_back(gameBoard[newlyAddedPlace.row + 1 + i][newlyAddedPlace.column]);
        }
        wasFound = true;
    }
    if (rowRight + rowLeft >= ghostBoomCount - 1) {
        for(int i = 0; i < rowRight; i++) {
            deletableGhosts.push_back(gameBoard[newlyAddedPlace.row][newlyAddedPlace.column + 1 + i]);
        }
        for(int i = 0; i < rowLeft; i++) {
            deletableGhosts.push_back(gameBoard[newlyAddedPlace.row][newlyAddedPlace.column - 1 - i]);
        }
        wasFound = true;
    }
    if(wasFound)
        deletableGhosts.push_back(gameBoard[newlyAddedPlace.row][newlyAddedPlace.column]);
    return deletableGhosts;
}
