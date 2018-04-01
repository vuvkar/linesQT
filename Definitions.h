#ifndef DEFINITIONS_H
#define DEFINITIONS_H

struct MatrixPoint {
    int column;
    int row;
    MatrixPoint(int row, int column) : column(column), row(row) {}
    MatrixPoint():column(0), row(0) {}
};

struct Node
{
    int x, y, distance;
};

const int boardRow  = 10;
const int boardColumn = 10;
const int ghostsAtMove = 5;
const int ghostTypeCount = 4;
const int ghostBoomCount = 4;
const int sWidth = 600;
const int cellSize = 50;

#endif // DEFINITIONS_H
