#ifndef PROPERTIES_H_INCLUDED
#define PROPERTIES_H_INCLUDED

#define CONTINUE 1
#define EXIT 0

#define TRUE 1
#define FALSE 0

#define BLACK 1
#define WHITE 2

#define SPACING_HEIGHT 12
#define SPACING_WIDTH 6
#define OFFSET_HEIGHT 45
#define OFFSET_WIDTH 47

#define WIDTH_PIECE 54
#define HEIGHT_PIECE 48

struct piece{
    int posY;
    int posX;
    int pickedPosX;
    int pickedPosY;
    int height;
    int width;
    int isPicked;
    int isQueen;
    int color;
};

struct board{
    int posY;
    int posX;
    int height;
    int width;
    int size;
    int nbPieces;
};
#endif // PROPERTIES_H_INCLUDED
