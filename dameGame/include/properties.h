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

struct piece {
    int posY;
    int posX;
    int renderPosX;
    int renderPosY;
    int height;
    int width;
    int isPicked;
    int isQueen;
    int color;
};

struct board {
    int posX;
    int posY;
    int height;
    int width;
    int size;
    int nbPieces;
    SDL_Color colorDefault;
    SDL_Color colorPicked;
};

struct tile {
    SDL_Rect params;
    int posX;
    int posY;
    int isPossibleMove;
};
#endif // PROPERTIES_H_INCLUDED
