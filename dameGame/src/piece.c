#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "properties.h"
#include "graphicRendering.h"

void initPieces(piece pieces[], int nbPieces, int color, int boardSize) {
    int posX = 1;
    int posY = 1;
    int pieceIndex = 0;

    if(color == WHITE){
        posY = 1;
    }
    else {
        switch (nbPieces){
        case 12:
            posY = 6;
            break;
        case 20:
            posY = 7;
            break;
        case 30:
            posY = 8;
            break;
        }
    }

    do{
        if(((posX + posY) % 2) != 0){
            pieces[pieceIndex].isQueen = FALSE;
            pieces[pieceIndex].isDeath = FALSE;
            pieces[pieceIndex].color = color;
            pieces[pieceIndex].isPicked = FALSE;
            pieces[pieceIndex].posY = posY;
            pieces[pieceIndex].posX = posX;
            pieces[pieceIndex].height = HEIGHT_PIECE;
            pieces[pieceIndex].width = WIDTH_PIECE;
            pieces[pieceIndex].renderPosX = 0;
            pieces[pieceIndex].renderPosY = 0;
            pieces[pieceIndex].canMove = FALSE;
            pieceIndex++;
        }
        posX++;
        if(posX > boardSize){
            posX = 1;
            posY++;
        }

    }while(pieceIndex != nbPieces);
}

void getPiecePositionFromClick(SDL_Event event, piece pieces[], int nbPieces) {
    for(int i=0; i < nbPieces; i++){
        if(event.button.y < ((pieces[i].height + SPACING_HEIGHT) * pieces[i].posY)
        && event.button.y > ((pieces[i].height + SPACING_HEIGHT) * pieces[i].posY) - OFFSET_HEIGHT
        && event.button.x < ((pieces[i].width + SPACING_WIDTH) * pieces[i].posX)
        && event.button.x > ((pieces[i].width + SPACING_WIDTH) * pieces[i].posX) - OFFSET_WIDTH) {
            printf("\nClicked Piece position : %d - %d", pieces[i].posX, pieces[i].posY);
        }
    }
}

piece initMovement(piece pieces[], int nbPieces, SDL_Event event, piece defaultPiece) {
    for(int i=0; i < nbPieces; i++){
        if(event.button.y < ((pieces[i].height + SPACING_HEIGHT) * pieces[i].posY)
        && event.button.y > ((pieces[i].height + SPACING_HEIGHT) * pieces[i].posY) - OFFSET_HEIGHT
        && event.button.x < ((pieces[i].width + SPACING_WIDTH) * pieces[i].posX)
        && event.button.x > ((pieces[i].width + SPACING_WIDTH) * pieces[i].posX) - OFFSET_WIDTH) {
            pieces[i].isPicked = TRUE;
            return pieces[i];
        }
    }
    return defaultPiece;
}

void endMovement(piece pieces[], int nbPieces) {
    for(int i=0; i < nbPieces; i++){
        pieces[i].isPicked = FALSE;
    }
}

int isSamePiece(piece defaultPiece, piece piece) {
    if(
    defaultPiece.posY == piece.posY
        && defaultPiece.posX == piece.posX
        && defaultPiece.renderPosX == piece.renderPosX
        && defaultPiece.renderPosY == piece.renderPosY
        && defaultPiece.height == piece.height
        && defaultPiece.width == piece.width
        && defaultPiece.isPicked == piece.isPicked
        && defaultPiece.isQueen == piece.isQueen
        && defaultPiece.color == piece.color
    ) {
        return TRUE;
    }
    return FALSE;
}

void checkValidSimpleMove(piece currentPiece, board board, tile blackTiles[], piece whitePieces[], piece blackPieces[]) {
    int nbTiles = ((board.size * board.size) / 2);
    int offsetY = (currentPiece.color == WHITE) ? 1 : -1 ;
    for(int i = 0; i<nbTiles; i++) {
        if(blackTiles[i].posY == currentPiece.posY + offsetY) {
            if (blackTiles[i].posX == currentPiece.posX || blackTiles[i].posX == currentPiece.posX - 2) {
                if(FALSE == checkIfTileOccupated(blackTiles[i], board, whitePieces, blackPieces)) {
                    blackTiles[i].isPossibleMove = TRUE;
                }
            }
        }
    }
}

void upgradeToQueen(tile tileDrop, piece pieces[], board board) {
    for(int i=0; i<board.nbPieces; i++) {
        if(pieces[i].isPicked == TRUE) {
            if((pieces[i].color == WHITE) && (tileDrop.posY == board.size)) {
                pieces[i].isQueen = TRUE;
            }
            else if((pieces[i].color == BLACK) && (tileDrop.posY == 1)) {
                pieces[i].isQueen = TRUE;
            }
        }
    }
}

void initKillPiece(tile dropTile, piece currentPieces[], piece opponentPieces[], tile blackTiles[], board board) {
    for(int i=0; i < board.nbPieces; i++){
        if(currentPieces[i].isPicked == TRUE) {
            if((dropTile.posY == currentPieces[i].posY + 2) // Bottom left
            && (dropTile.posX == currentPieces[i].posX - 3)) {
                killPieceOnPosition(opponentPieces, board, currentPieces[i].posX-1, currentPieces[i].posY+1);
            }
            else if((dropTile.posY == currentPieces[i].posY + 2) // Bottom right
            && (dropTile.posX == currentPieces[i].posX + 1)) {
                killPieceOnPosition(opponentPieces, board, currentPieces[i].posX+1, currentPieces[i].posY+1);
            }
            else if((dropTile.posY == currentPieces[i].posY - 2) // Top left
            && (dropTile.posX == currentPieces[i].posX - 3)) {
                killPieceOnPosition(opponentPieces, board, currentPieces[i].posX-1, currentPieces[i].posY-1);
            }
            else if((dropTile.posY == currentPieces[i].posY - 2) // Top right
            && (dropTile.posX == currentPieces[i].posX + 1)) {
                killPieceOnPosition(opponentPieces, board, currentPieces[i].posX+1, currentPieces[i].posY-1);
            }
        }
    }
}

void killPiece(piece *pieceToKill) {
    pieceToKill->isDeath = TRUE;
    pieceToKill->posX = -50;
    pieceToKill->posY = -50;
}

void killPieceOnPosition(piece pieces[], board board, int posX, int posY) {
    for(int i=0; i< board.nbPieces; i++) {
            printf("\nSeek piece : %d - %d (current : %d - %d)", posX, posY, pieces[i].posX, pieces[i].posY);
        if(pieces[i].posX == posX && pieces[i].posY == posY) {
            pieces[i].isDeath = TRUE;
            pieces[i].posX = -50;
            pieces[i].posY = -50;
        }
    }
}

void movePieceInNewTile(tile dropTile, piece pieces[], tile blackTiles[], board board) {
    for(int i=0; i < board.nbPieces; i++){
        if(pieces[i].isPicked == TRUE) {
            pieces[i].posY = dropTile.posY;
            pieces[i].posX = dropTile.posX+1;
        }
    }
}

void checkValidTakeMove(piece currentPiece, board board, tile blackTiles[], piece whitePieces[], piece blackPieces[]) {
    int nbTiles = ((board.size * board.size) / 2);
    int offsetBeforeY = -1;
    int offsetAfterY = 1;
    for(int i = 0; i<nbTiles; i++) {
        int colorOfPiece = colorOfPieceOnTile(blackTiles[i], board, whitePieces, blackPieces);
        switch (colorOfPiece) {
            blackTiles[i].isPossibleMove = TRUE;
        case FALSE :
            return;
            break;
        case WHITE :
            if(currentPiece.color == BLACK) {
                if((blackTiles[i].posY == currentPiece.posY + offsetAfterY) // Bottom left
                && (blackTiles[i].posX == currentPiece.posX - 2)) {
                    setPossibleMoveOnCoordonate(blackTiles, currentPiece.posX - 3, currentPiece.posY + 2, nbTiles, board, whitePieces, blackPieces);
                }
                else if((blackTiles[i].posY == currentPiece.posY + offsetAfterY) // Bottom right
                && (blackTiles[i].posX == currentPiece.posX)) {
                    setPossibleMoveOnCoordonate(blackTiles, currentPiece.posX + 1, currentPiece.posY + 2, nbTiles, board, whitePieces, blackPieces);
                }
                else if((blackTiles[i].posY == currentPiece.posY + offsetBeforeY) // Top left
                && (blackTiles[i].posX == currentPiece.posX - 2)) {
                    setPossibleMoveOnCoordonate(blackTiles, currentPiece.posX - 3, currentPiece.posY - 2, nbTiles, board, whitePieces, blackPieces);
                }
                else if((blackTiles[i].posY == currentPiece.posY + offsetBeforeY) // Top right
                && (blackTiles[i].posX == currentPiece.posX)) {
                    setPossibleMoveOnCoordonate(blackTiles, currentPiece.posX + 1, currentPiece.posY - 2, nbTiles, board, whitePieces, blackPieces);
                }
            }
            break;
        case BLACK :
            if(currentPiece.color == WHITE) {
                if((blackTiles[i].posY == currentPiece.posY + offsetAfterY) // Bottom left
                && (blackTiles[i].posX == currentPiece.posX - 2)) {
                    setPossibleMoveOnCoordonate(blackTiles, currentPiece.posX - 3, currentPiece.posY + 2, nbTiles, board, whitePieces, blackPieces);
                }
                else if((blackTiles[i].posY == currentPiece.posY + offsetAfterY) // Bottom right
                && (blackTiles[i].posX == currentPiece.posX)) {
                    setPossibleMoveOnCoordonate(blackTiles, currentPiece.posX + 1, currentPiece.posY + 2, nbTiles, board, whitePieces, blackPieces);
                }
                else if((blackTiles[i].posY == currentPiece.posY + offsetBeforeY) // Top left
                && (blackTiles[i].posX == currentPiece.posX - 2)) {
                    setPossibleMoveOnCoordonate(blackTiles, currentPiece.posX - 3, currentPiece.posY - 2, nbTiles, board, whitePieces, blackPieces);
                }
                else if((blackTiles[i].posY == currentPiece.posY + offsetBeforeY) // Top right
                && (blackTiles[i].posX == currentPiece.posX)) {
                    setPossibleMoveOnCoordonate(blackTiles, currentPiece.posX + 1, currentPiece.posY - 2, nbTiles, board, whitePieces, blackPieces);
                }
            }
            break;
        default :
            break;
        }
    }
}

int checkIfCanTake(board board, tile blackTiles[], piece currentPiece, piece currentPieces[], piece opponentPieces[]) {
    int nbTiles = ((board.size * board.size) / 2);
    int colorOfPiece = -1;
    int offsetBeforeY = -1;
    int offsetAfterY = 1;
    for(int i = 0; i<nbTiles; i++) {
        if(currentPiece.color == WHITE) {
            colorOfPiece = colorOfPieceOnTile(blackTiles[i], board, currentPieces, opponentPieces);
        }
        else {
            colorOfPiece = colorOfPieceOnTile(blackTiles[i], board, opponentPieces, currentPieces);
        }

        switch (colorOfPiece) {
        case FALSE :
            break;
        case WHITE:
        case BLACK:
            if(currentPiece.color != colorOfPiece) {
                for (int j=0; j<nbTiles; j++) {
                    if((blackTiles[i].posY == currentPiece.posY + offsetAfterY) // Bottom left
                    && (blackTiles[i].posX == currentPiece.posX - 2)
                    && (blackTiles[i].posY + 1 == blackTiles[j].posY) // Bottom left
                    && (blackTiles[i].posX - 1 == blackTiles[j].posX)) {
                        if(FALSE == checkIfTileOccupated(blackTiles[j], board, currentPieces, opponentPieces)) {
                            return TRUE;
                        }
                    }
                    else if((blackTiles[i].posY == currentPiece.posY + offsetAfterY) // Bottom right
                    && (blackTiles[i].posX == currentPiece.posX)
                    && (blackTiles[i].posY + 1 == blackTiles[j].posY) // Bottom right
                    && (blackTiles[i].posX + 1 == blackTiles[j].posX)) {
                        if(FALSE == checkIfTileOccupated(blackTiles[j], board, currentPieces, opponentPieces)) {
                            return TRUE;
                        }
                    }
                    else if((blackTiles[i].posY == currentPiece.posY + offsetBeforeY) // Top left
                    && (blackTiles[i].posX == currentPiece.posX - 2)
                    && (blackTiles[i].posY - 1 == blackTiles[j].posY) // Top left
                    && (blackTiles[i].posX - 1 == blackTiles[j].posX)) {
                        if(FALSE == checkIfTileOccupated(blackTiles[j], board, currentPieces, opponentPieces)) {
                            return TRUE;
                        }
                    }
                    else if((blackTiles[i].posY == currentPiece.posY + offsetBeforeY) // Top right
                    && (blackTiles[i].posX == currentPiece.posX)
                    && (blackTiles[i].posY - 1 == blackTiles[j].posY) // Top right
                    && (blackTiles[i].posX + 1 == blackTiles[j].posX)) {
                        if(FALSE == checkIfTileOccupated(blackTiles[j], board, currentPieces, opponentPieces)) {
                            return TRUE;
                        }
                    }
                }
            }
            break;
        }
    }
    return FALSE;
}

int checkIfCanMove(board board, tile blackTiles[], piece currentPiece, piece currentPieces[], piece opponentPieces[]) {
    int nbTiles = ((board.size * board.size) / 2);
    int offsetY = (currentPiece.color == WHITE) ? 1 : -1 ;
    for(int i = 0; i<nbTiles; i++) {
        if(blackTiles[i].posY == currentPiece.posY + offsetY) {
            if (blackTiles[i].posX == currentPiece.posX || blackTiles[i].posX == currentPiece.posX - 2) {
                if(FALSE == checkIfTileOccupated(blackTiles[i], board, currentPieces, opponentPieces)) {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

void checkAvailableMove(board board, tile blackTiles[], piece currentPieces[], piece opponentPieces[]) {
    int nbTiles = ((board.size * board.size) / 2);
    int oneCanTake = FALSE;
    int availableToTake[board.nbPieces];
    int availableToMove[board.nbPieces];

    for(int i = 0; i<board.nbPieces; i++) {
        availableToTake[i] = checkIfCanTake(board, blackTiles, currentPieces[i], currentPieces, opponentPieces);
        if(availableToTake[i] == TRUE) {
            oneCanTake = TRUE;
        }
    }

    if(oneCanTake == TRUE) {
        for(int i = 0; i<board.nbPieces; i++) {
            opponentPieces[i].canMove = TRUE;
            if(availableToTake[i] == TRUE) {
                currentPieces[i].canMove = TRUE;
            }
            else {
                currentPieces[i].canMove = FALSE;
            }
        }
        return;
    }

    for(int i = 0; i<board.nbPieces; i++) {
        availableToMove[i] = checkIfCanMove(board, blackTiles, currentPieces[i], currentPieces, opponentPieces);
    }
    for(int i = 0; i<board.nbPieces; i++) {
        opponentPieces[i].canMove = TRUE;
        if(availableToMove[i] == TRUE) {
            currentPieces[i].canMove = TRUE;
        }
        else {
            currentPieces[i].canMove = FALSE;
        }
    }
    return;
}
