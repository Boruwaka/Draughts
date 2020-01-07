#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "properties.h"
#include "graphicRendering.h"

void initBoard(board *newBoard) {
    newBoard->posX = 5;
    newBoard->posY = 5;
    newBoard->size = 8;
    newBoard->colorDefault.r = 100;
    newBoard->colorDefault.g = 100;
    newBoard->colorDefault.b = 100;
    newBoard->colorDefault.a = 255;
    newBoard->colorPicked.r = 130;
    newBoard->colorPicked.g = 100;
    newBoard->colorPicked.b = 220;
    newBoard->colorPicked.a = 255;

    switch (newBoard->size) {
    case 8:
        newBoard->nbPieces = 12;
        newBoard->height = 490;
        newBoard->width = 490;
        break;
    case 10:
        newBoard->nbPieces = 20;
        newBoard->height = 610;
        newBoard->width = 610;
        break;
    case 12:
        newBoard->nbPieces = 30;
        newBoard->height = 730;
        newBoard->width = 730;
        break;
    }
}
