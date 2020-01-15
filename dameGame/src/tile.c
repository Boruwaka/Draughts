#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "properties.h"
#include "graphicRendering.h"

int initTiles(struct tile blackTiles[], int size) {
  //  int nbTile = ((size*size) / 2);
    int posX=1; int posY=1; int indexTab=0;
    for(int i = 0; i<(size * size); i++){
        if(((i + posY) % 2) != 0){
            blackTiles[indexTab].params.x = 10 + posX*60;
            blackTiles[indexTab].params.y = 10 + posY*60 - 60;
            blackTiles[indexTab].params.h = 60;
            blackTiles[indexTab].params.w = 60;
            blackTiles[indexTab].posX = posX;
            blackTiles[indexTab].posY = posY;
            blackTiles[indexTab].isPossibleMove = FALSE;
            indexTab++;
        }

        if(posX == (size-1)){
            posX = 0;
            posY++;
        }
        else{
            posX++;
        }
    }
    return 0;
}

void getTilePositionFromClick(SDL_Event event, tile blackTiles[], int nbPieces) {
    for(int i=0; i < (nbPieces/2); i++){
        if(event.button.y < ((blackTiles[i].params.h + SPACING_HEIGHT) * blackTiles[i].posY)
        && event.button.y > ((blackTiles[i].params.h + SPACING_HEIGHT) * blackTiles[i].posY) - OFFSET_HEIGHT
        && event.button.x < ((blackTiles[i].params.w + SPACING_WIDTH) * blackTiles[i].posX+1)
        && event.button.x > ((blackTiles[i].params.w + SPACING_WIDTH) * blackTiles[i].posX+1) - OFFSET_WIDTH) {
            printf("\nClicked Tile position : %d - %d", blackTiles[i].posX, blackTiles[i].posY);
        }
    }
}

int isSameTile(tile defaultTile, tile tile) {
    if(
       defaultTile.isPossibleMove == tile.isPossibleMove
       && defaultTile.posX == tile.posX
       && defaultTile.posY == tile.posY
       && defaultTile.params.h == tile.params.h
       && defaultTile.params.w == tile.params.w
       && defaultTile.params.x == tile.params.x
       && defaultTile.params.y == tile.params.y
    ) {
        return TRUE;
    }
    return FALSE;
}

tile* getTileFromCoordonate(tile blackTiles[], int posX, int posY, int nbTiles) {
    for(int i=0; i< nbTiles; i++) {
        tile *test = &blackTiles[i];
        //printf("\nTile pointer : %d - %d", test->posX, test->posY);
            //printf("FOUND TILE : %d - %d", blackTiles[i].posX, blackTiles[i].posY);
        if(blackTiles[i].posX == posX && blackTiles[i].posY == posY) {
            return &blackTiles[i];
        }
    }
    return NULL;
}

int checkIfTileOccupated(tile blackTile, board board, piece whitePieces[], piece blackPieces[]) {
    if((blackTile.posX > board.size)
       || (blackTile.posX < 0)
       || (blackTile.posY > board.size)
       || (blackTile.posY < 0)) {
        return -1;
       }
    for(int i=0; i<board.nbPieces; i++) {
        if((whitePieces[i].posX == blackTile.posX+1) && (whitePieces[i].posY == blackTile.posY)) {
            return TRUE;
        }
        if((blackPieces[i].posX == blackTile.posX+1) && (blackPieces[i].posY == blackTile.posY)) {
            return TRUE;
        }
    }
    return FALSE;
}

int colorOfPieceOnTile(tile blackTile, board board, piece whitePieces[], piece blackPieces[]) {
    for(int i=0; i<board.nbPieces; i++) {
        if((whitePieces[i].posX == blackTile.posX+1) && (whitePieces[i].posY == blackTile.posY)) {
            return WHITE;
        }
        if((blackPieces[i].posX == blackTile.posX+1) && (blackPieces[i].posY == blackTile.posY)) {
            return BLACK;
        }
    }
    return -1;
}

void resetValidMove(board board, tile blackTiles[]) {
    int nbTiles = ((board.size * board.size) / 2);
    for(int i=0; i<nbTiles; i++) {
        blackTiles[i].isPossibleMove = FALSE;
    }
}

tile isValidDeplacement(board board, tile blackTiles[], SDL_Event event) {
    tile defaultTile = {-1, -1, -1, -1, -1, -1, FALSE};
    int size = ((board.size * board.size) / 2);
    //printf("\n\nEvent : x: %d - y: %d\n", event.button.x, event.button.y);
    for(int i=0; i < size; i++){
        if(blackTiles[i].isPossibleMove == TRUE) {
            if(event.button.x < (blackTiles[i].params.x + blackTiles[i].params.w) &&
            (event.button.x > (blackTiles[i].params.x )) &&
            (event.button.y < (blackTiles[i].params.y + blackTiles[i].params.h)) &&
            (event.button.y > (blackTiles[i].params.y ))) {
               /* printf("Tile : x-top: %d - x-bot: %d   ///   y-top: %d - y-bot: %d\n",
                   (blackTiles[i].params.x + blackTiles[i].params.w),
                   (blackTiles[i].params.x ),
                   (blackTiles[i].params.y + blackTiles[i].params.h),
                   (blackTiles[i].params.y ));*/
                return blackTiles[i];
            }
        }
    }
    return defaultTile;
}

int setPossibleMoveOnCoordonate(tile blackTiles[], int posX, int posY, int nbTiles, board board, piece whitePieces[], piece blackPieces[]) {
    for(int i=0; i < nbTiles; i++) {
        if( blackTiles[i].posX == posX && blackTiles[i].posY == posY ) {
            if( checkIfTileOccupated(blackTiles[i], board, whitePieces, blackPieces) == FALSE){
                blackTiles[i].isPossibleMove = TRUE;
                return TRUE;
            }
        }
    }
    return FALSE;
}


