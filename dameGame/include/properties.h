#ifndef PROPERTIES_H_INCLUDED
#define PROPERTIES_H_INCLUDED

#define CONTINUE 1
#define EXIT 0

#define TRUE 1
#define FALSE 0

#define BLACK 1
#define WHITE 2

#define UP 2
#define DOWN 3
#define RIGHT 5
#define LEFT 7

#define UP_RIGHT 10
#define UP_LEFT 17
#define DOWN_RIGHT 15
#define DOWN_LEFT 21

#define SPACING_HEIGHT 12
#define SPACING_WIDTH 6
#define OFFSET_HEIGHT 45
#define OFFSET_WIDTH 47

#define WIDTH_PIECE 54
#define HEIGHT_PIECE 48

typedef struct piece {
    int posY;
    int posX;
    int renderPosX;
    int renderPosY;
    int height;
    int width;
    int isPicked;
    int isDeath;
    int isQueen;
    int color;
    int canMove;
} piece;

typedef struct board {
    int posX;
    int posY;
    int height;
    int width;
    int size;
    int nbPieces;
    SDL_Color colorDefault;
    SDL_Color colorPicked;
} board;

typedef struct tile {
    SDL_Rect params;
    int posX;
    int posY;
    int isPossibleMove;
} tile;
#endif // PROPERTIES_H_INCLUDED
