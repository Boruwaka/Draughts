#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "properties.h"
#include "graphicRendering.h"

int checkQueenValidSimpleMove(piece currentPiece, board board, tile blackTiles[], piece whitePieces[], piece blackPieces[]) {
    int nbTiles = ((board.size * board.size) / 2);
    int isAtLeastOneAvailable = FALSE;
    struct tile *currentTile;

    // Diagonale bottom left
    for(int i = 1; i<nbTiles; i++) {
        currentTile = getTileFromCoordonate(blackTiles, currentPiece.posX - i-1, currentPiece.posY + i, nbTiles);
        if (currentTile != NULL) {
            if( checkIfTileOccupated(*currentTile, board, whitePieces, blackPieces) == FALSE) {
                isAtLeastOneAvailable = TRUE;
                currentTile->isPossibleMove = TRUE;
            }
            else {
                break;
            }
        }
    }

    // Diagonale bottom right
    for(int i = 1; i<nbTiles; i++) {
        currentTile = getTileFromCoordonate(blackTiles, currentPiece.posX + i-1, currentPiece.posY + i, nbTiles);
        if (currentTile != NULL) {
            if( checkIfTileOccupated(*currentTile, board, whitePieces, blackPieces) == FALSE) {
                isAtLeastOneAvailable = TRUE;
                currentTile->isPossibleMove = TRUE;
            }
            else {
                break;
            }
        }
    }

    // Diagonale top left
    for(int i = 1; i<nbTiles; i++) {
        currentTile = getTileFromCoordonate(blackTiles, currentPiece.posX - i-1, currentPiece.posY - i, nbTiles);
        if (currentTile != NULL) {
            if( checkIfTileOccupated(*currentTile, board, whitePieces, blackPieces) == FALSE) {
                isAtLeastOneAvailable = TRUE;
                currentTile->isPossibleMove = TRUE;
            }
            else {
                break;
            }
        }
    }

    // Diagonale top right
    for(int i = 1; i<nbTiles; i++) {
        currentTile = getTileFromCoordonate(blackTiles, currentPiece.posX + i-1, currentPiece.posY - i, nbTiles);
        if (currentTile != NULL) {
            if( checkIfTileOccupated(*currentTile, board, whitePieces, blackPieces) == FALSE) {
                isAtLeastOneAvailable = TRUE;
                currentTile->isPossibleMove = TRUE;
            }
            else {
                break;
            }
        }
    }
    return isAtLeastOneAvailable;
}

void checkQueenValidTakeMove(piece currentPiece, board board, tile blackTiles[], piece whitePieces[], piece blackPieces[]) {
    int nbTiles = ((board.size * board.size) / 2);
    struct tile *currentTile;
    int canTakeBotLeft = FALSE;
    int canTakeBotRight = FALSE;
    int canTakeTopLeft = FALSE;
    int canTakeTopRight = FALSE;
    int colorOfPiece = 10;

    // Diagonale bottom left
    for(int i = 1; i<nbTiles; i++) {
        currentTile = getTileFromCoordonate(blackTiles, currentPiece.posX - i-1, currentPiece.posY + i, nbTiles);
        if (currentTile != NULL) {
            if(checkIfTileOccupated(*currentTile, board, whitePieces, blackPieces) == TRUE) {
                colorOfPiece = colorOfPieceOnTile(*currentTile, board, whitePieces, blackPieces);
                if((colorOfPiece != currentPiece.color) && (canTakeBotLeft == FALSE)) {
                    canTakeBotLeft = TRUE;
                }
                else {
                    break;
                }
            }
            else {
                if(canTakeBotLeft == TRUE) {
                    currentTile->isPossibleMove = TRUE;
                }
            }
        }
    }

    // Diagonale bottom right
    for(int i = 1; i<nbTiles; i++) {
        currentTile = getTileFromCoordonate(blackTiles, currentPiece.posX + i-1, currentPiece.posY + i, nbTiles);
        if (currentTile != NULL) {
            if(checkIfTileOccupated(*currentTile, board, whitePieces, blackPieces) == TRUE) {
                colorOfPiece = colorOfPieceOnTile(*currentTile, board, whitePieces, blackPieces);
                if((colorOfPiece != currentPiece.color) && (canTakeBotRight == FALSE)) {
                    canTakeBotRight = TRUE;
                }
                else {
                    break;
                }
            }
            else {
                if(canTakeBotRight == TRUE) {
                    currentTile->isPossibleMove = TRUE;
                }
            }
        }
    }

    // Diagonale top left
    for(int i = 1; i<nbTiles; i++) {
        currentTile = getTileFromCoordonate(blackTiles, currentPiece.posX - i-1, currentPiece.posY - i, nbTiles);
        if (currentTile != NULL) {
            if(checkIfTileOccupated(*currentTile, board, whitePieces, blackPieces) == TRUE) {
                colorOfPiece = colorOfPieceOnTile(*currentTile, board, whitePieces, blackPieces);
                if((colorOfPiece != currentPiece.color) && (canTakeTopLeft == FALSE)) {
                    canTakeTopLeft = TRUE;
                }
                else {
                    break;
                }
            }
            else {
                if(canTakeTopLeft == TRUE) {
                    currentTile->isPossibleMove = TRUE;
                }
            }
        }
    }

    // Diagonale top right
    for(int i = 1; i<nbTiles; i++) {
        currentTile = getTileFromCoordonate(blackTiles, currentPiece.posX + i-1, currentPiece.posY - i, nbTiles);
        if (currentTile != NULL) {
            if(checkIfTileOccupated(*currentTile, board, whitePieces, blackPieces) == TRUE) {
                colorOfPiece = colorOfPieceOnTile(*currentTile, board, whitePieces, blackPieces);
                if((colorOfPiece != currentPiece.color) && (canTakeTopRight == FALSE)) {
                    canTakeTopRight = TRUE;
                }
                else {
                    break;
                }
            }
            else {
                if(canTakeTopRight == TRUE) {
                    currentTile->isPossibleMove = TRUE;
                }
            }
        }
    }
}

void killWithQueen(tile dropTile, board board, tile blackTiles[], piece currentPieces[], piece opponentPieces[], int direction) {
    int nbTiles = ((board.size * board.size) / 2);
    tile *currentTile;
    int foundIt = FALSE;
    int offsetX = 0;
    int offsetY = 0;

    for(int i = 1; i<board.size; i++) {
        offsetX = (direction % RIGHT) ? i : -i ;
        offsetY = (direction % DOWN) ? i : -i ;
        currentTile = getTileFromCoordonate(blackTiles, dropTile.posX + offsetX, dropTile.posY + offsetY, nbTiles);
        printf("%d = %d-%d", direction, dropTile.posX + offsetX, dropTile.posY + offsetY);
        if(foundIt == TRUE) {
            break;
        }
        if (currentTile == NULL) {
            continue;
        }
        if(checkIfTileOccupated(*currentTile, board, currentPieces, opponentPieces) == FALSE) {
            continue;
        }

        for(int i=0; i<board.nbPieces; i++) {
            if((opponentPieces[i].posX == currentTile->posX + 1) && (opponentPieces[i].posY == currentTile->posY)) {
                killPiece(&opponentPieces[i]);
                foundIt = TRUE;
                break;
            }
        }
    }
}

int getDirection(piece takenPiece, tile tileDrop, board board) {
    int dirUpDown = 0;
    int dirRightLeft = 0;

    if(takenPiece.posY > tileDrop.posY) { // TO TOP
        dirUpDown = UP;
    }
    else {
        dirUpDown = DOWN;
    }

    if(takenPiece.posX > tileDrop.posX) { // TO LEFT
        dirRightLeft = LEFT;
    }
    else {
        dirRightLeft = RIGHT;
    }

    return dirUpDown * dirRightLeft;
}
