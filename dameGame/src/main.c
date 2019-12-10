#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "properties.h"
#include "graphicRendering.h"

void initPieces(struct piece pieces[], int nbPieces, int color, int boardSize) {
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
            pieces[pieceIndex].isQueen = TRUE;
            pieces[pieceIndex].isDeath = FALSE;
            pieces[pieceIndex].color = color;
            pieces[pieceIndex].isPicked = FALSE;
            pieces[pieceIndex].posY = posY;
            pieces[pieceIndex].posX = posX;
            pieces[pieceIndex].height = HEIGHT_PIECE;
            pieces[pieceIndex].width = WIDTH_PIECE;
            pieces[pieceIndex].renderPosX = 0;
            pieces[pieceIndex].renderPosY = 0;
            /*if(posX == 3 && posY == 4) {
                pieces[pieceIndex].isQueen = TRUE;
            }*/
            pieceIndex++;

        }



        posX++;
        if(posX > boardSize){
            posX = 1;
            posY++;
        }

    }while(pieceIndex != nbPieces);
}

struct piece initMovement(struct piece pieces[], int nbPieces, SDL_Event event, struct piece defaultPiece) {
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

void endMovement(struct piece pieces[], int nbPieces) {
    for(int i=0; i < nbPieces; i++){
        pieces[i].isPicked = FALSE;
    }
}

void resetValidMove(struct board board, struct tile blackTiles[]) {
    int nbTiles = ((board.size * board.size) / 2);
    for(int i=0; i<nbTiles; i++) {
        blackTiles[i].isPossibleMove = FALSE;
    }
}

int checkIfTileOccupated(struct tile blackTile, struct board board, struct piece whitePieces[], struct piece blackPieces[]) {
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

int colorOfPieceOnTile(struct tile blackTile, struct board board, struct piece whitePieces[], struct piece blackPieces[]) {
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

int isSameTile(struct tile defaultTile, struct tile tile) {
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

int isSamePiece(struct piece defaultPiece, struct piece piece) {
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

void checkValidSimpleMove(struct piece piece, struct board board, struct tile blackTiles[], struct piece whitePieces[], struct piece blackPieces[]) {
    int nbTiles = ((board.size * board.size) / 2);
    int offsetY = (piece.color == WHITE) ? 1 : -1 ;
    for(int i = 0; i<nbTiles; i++) {
        if(blackTiles[i].posY == piece.posY + offsetY) {
            if (blackTiles[i].posX == piece.posX || blackTiles[i].posX == piece.posX - 2) {
                if(FALSE == checkIfTileOccupated(blackTiles[i], board, whitePieces, blackPieces)) {
                    blackTiles[i].isPossibleMove = TRUE;
                }
            }
        }
    }
}

struct tile* getTileFromCoordonate(struct tile blackTiles[], int posX, int posY, int nbTiles) {
    for(int i=0; i< nbTiles; i++) {
        if(blackTiles[i].posX == posX && blackTiles[i].posY == posY) {
            return &blackTiles[i];
        }
    }
    return NULL;
}

void checkQueenValidSimpleMove(struct piece piece, struct board board, struct tile blackTiles[], struct piece whitePieces[], struct piece blackPieces[]) {
    int nbTiles = ((board.size * board.size) / 2);
    struct tile *currentTile;

    // Diagonale bottom left
    for(int i = 1; i<nbTiles; i++) {
        currentTile = getTileFromCoordonate(blackTiles, piece.posX - i-1, piece.posY + i, nbTiles);
        if (currentTile != NULL) {
            if( checkIfTileOccupated(*currentTile, board, whitePieces, blackPieces) == FALSE) {
                currentTile->isPossibleMove = TRUE;
            }
            else {
                break;
            }
        }
    }

    // Diagonale bottom right
    for(int i = 1; i<nbTiles; i++) {
        currentTile = getTileFromCoordonate(blackTiles, piece.posX + i-1, piece.posY + i, nbTiles);
        if (currentTile != NULL) {
            if( checkIfTileOccupated(*currentTile, board, whitePieces, blackPieces) == FALSE) {
                currentTile->isPossibleMove = TRUE;
            }
            else {
                break;
            }
        }
    }

    // Diagonale top left
    for(int i = 1; i<nbTiles; i++) {
        currentTile = getTileFromCoordonate(blackTiles, piece.posX - i-1, piece.posY - i, nbTiles);
        if (currentTile != NULL) {
            if( checkIfTileOccupated(*currentTile, board, whitePieces, blackPieces) == FALSE) {
                currentTile->isPossibleMove = TRUE;
            }
            else {
                break;
            }
        }
    }

    // Diagonale top right
    for(int i = 1; i<nbTiles; i++) {
        currentTile = getTileFromCoordonate(blackTiles, piece.posX + i-1, piece.posY - i, nbTiles);
        if (currentTile != NULL) {
            if( checkIfTileOccupated(*currentTile, board, whitePieces, blackPieces) == FALSE) {
                currentTile->isPossibleMove = TRUE;
            }
            else {
                break;
            }
        }
    }
}

void setPossibleMoveOnCoordonate(struct tile blackTiles[], int posX, int posY, int nbTiles, struct board board, struct piece whitePieces[], struct piece blackPieces[]) {
    for(int i=0; i < nbTiles; i++) {
        if( blackTiles[i].posX == posX && blackTiles[i].posY == posY ) {
            if( checkIfTileOccupated(blackTiles[i], board, whitePieces, blackPieces) == FALSE){
                blackTiles[i].isPossibleMove = TRUE;
            }
        }
    }
}

void checkValidTakeMove(struct piece piece, struct board board, struct tile blackTiles[], struct piece whitePieces[], struct piece blackPieces[]) {
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
            if(piece.color == BLACK) {
                if((blackTiles[i].posY == piece.posY + offsetAfterY) // Bottom left
                && (blackTiles[i].posX == piece.posX - 2)) {
                    setPossibleMoveOnCoordonate(blackTiles, piece.posX - 3, piece.posY + 2, nbTiles, board, whitePieces, blackPieces);
                }
                else if((blackTiles[i].posY == piece.posY + offsetAfterY) // Bottom right
                && (blackTiles[i].posX == piece.posX)) {
                    setPossibleMoveOnCoordonate(blackTiles, piece.posX + 1, piece.posY + 2, nbTiles, board, whitePieces, blackPieces);
                }
                else if((blackTiles[i].posY == piece.posY + offsetBeforeY) // Top left
                && (blackTiles[i].posX == piece.posX - 2)) {
                    setPossibleMoveOnCoordonate(blackTiles, piece.posX - 3, piece.posY - 2, nbTiles, board, whitePieces, blackPieces);
                }
                else if((blackTiles[i].posY == piece.posY + offsetBeforeY) // Top right
                && (blackTiles[i].posX == piece.posX)) {
                    setPossibleMoveOnCoordonate(blackTiles, piece.posX + 1, piece.posY - 2, nbTiles, board, whitePieces, blackPieces);
                }
            }
            break;
        case BLACK :
            if(piece.color == WHITE) {
                if((blackTiles[i].posY == piece.posY + offsetAfterY) // Bottom left
                && (blackTiles[i].posX == piece.posX - 2)) {
                    setPossibleMoveOnCoordonate(blackTiles, piece.posX - 3, piece.posY + 2, nbTiles, board, whitePieces, blackPieces);
                }
                else if((blackTiles[i].posY == piece.posY + offsetAfterY) // Bottom right
                && (blackTiles[i].posX == piece.posX)) {
                    setPossibleMoveOnCoordonate(blackTiles, piece.posX + 1, piece.posY + 2, nbTiles, board, whitePieces, blackPieces);
                }
                else if((blackTiles[i].posY == piece.posY + offsetBeforeY) // Top left
                && (blackTiles[i].posX == piece.posX - 2)) {
                    setPossibleMoveOnCoordonate(blackTiles, piece.posX - 3, piece.posY - 2, nbTiles, board, whitePieces, blackPieces);
                }
                else if((blackTiles[i].posY == piece.posY + offsetBeforeY) // Top right
                && (blackTiles[i].posX == piece.posX)) {
                    setPossibleMoveOnCoordonate(blackTiles, piece.posX + 1, piece.posY - 2, nbTiles, board, whitePieces, blackPieces);
                }
            }
            break;
        default :
            break;
        }
    }
}

struct tile isValidDeplacement(struct board board, struct tile blackTiles[], SDL_Event event) {
    struct tile defaultTile = {-1, -1, -1, -1, -1, -1, FALSE};
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

void movePieceInNewTile(struct tile tile, struct piece pieces[], struct tile blackTiles[], struct board board) {
    for(int i=0; i < board.nbPieces; i++){
        if(pieces[i].isPicked == TRUE) {
            pieces[i].posY = tile.posY;
            pieces[i].posX = tile.posX+1;
        }
    }
}

void killPieceOnPosition(struct piece pieces[], struct board board, int posX, int posY) {
    for(int i=0; i< board.nbPieces; i++) {
        if(pieces[i].posX == posX && pieces[i].posY == posY) {
            pieces[i].isDeath = TRUE;
            pieces[i].posX = -50;
            pieces[i].posY = -50;
        }
    }
}

void initKillPiece(struct tile dropTile, struct piece currentPieces[], struct piece opponentPieces[], struct tile blackTiles[], struct board board) {
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

void upgradeToQueen(struct tile tileDrop, struct piece pieces[], struct board board) {
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

int main(int argc, char *argv[]){
    freopen("stdout.txt", "w", stdout);
    freopen("stderr.txt", "w", stderr);
    int gameState = CONTINUE;
    int currentPlayer = WHITE;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int statut = 0;

    struct board board;
    board.posX = 5;
    board.posY = 5;
    board.size = 10;
    board.colorDefault.r = 100;
    board.colorDefault.g = 100;
    board.colorDefault.b = 100;
    board.colorDefault.a = 255;
    board.colorPicked.r = 130;
    board.colorPicked.g = 100;
    board.colorPicked.b = 220;
    board.colorPicked.a = 255;

    switch (board.size) {
    case 8:
        board.nbPieces = 12;
        board.height = 490;
        board.width = 490;
        break;
    case 10:
        board.nbPieces = 20;
        board.height = 610;
        board.width = 610;
        break;
    case 12:
        board.nbPieces = 30;
        board.height = 730;
        board.width = 730;
        break;
    }
    struct piece whitePieces[board.nbPieces];
    struct piece blackPieces[board.nbPieces];
    int nbTile = ((board.size * board.size) / 2);
    struct tile blackTiles[nbTile];

    initGame(blackTiles, board.size);

    SDL_Color yellow = {255, 240, 140, 0};
    SDL_Color green = {40, 150, 40, 0};
    if(0 != initWindow(&window, &renderer, 1000, 740))
        goto Quit;
    SDL_Texture *whitePieceImage = NULL;
    whitePieceImage = loadImage("assets/whitePiece.bmp", renderer);
    SDL_Texture *blackPieceImage = NULL;
    blackPieceImage = loadImage("assets/blackPiece.bmp", renderer);
    SDL_Texture *blackQueenImage = NULL;
    blackQueenImage = loadImage("assets/blackQueen.bmp", renderer);
    SDL_Texture *whiteQueenImage = NULL;
    whiteQueenImage = loadImage("assets/whiteQueen.bmp", renderer);

    initPieces(whitePieces, board.nbPieces, WHITE, board.size);
    initPieces(blackPieces, board.nbPieces, BLACK, board.size);

    drawGame(renderer, whitePieces, blackPieces, board, whitePieceImage, blackPieceImage, whiteQueenImage, blackQueenImage, yellow, blackTiles);

    SDL_Event event;
    do {
        SDL_WaitEvent(&event);
        switch(event.type){
        case SDL_QUIT :
            goto Quit;
            break;

        case SDL_MOUSEBUTTONDOWN :
            if(event.button.button == SDL_BUTTON_LEFT){
                struct piece takenPiece = {-1, -1, -1, -1, -1, -1, FALSE, FALSE, BLACK};
                struct piece defaultPiece = {-1, -1, -1, -1, -1, -1, FALSE, FALSE, BLACK};

                if(currentPlayer == WHITE) {
                    takenPiece = initMovement(whitePieces, board.nbPieces, event, defaultPiece);
                    if(FALSE == isSamePiece(defaultPiece, takenPiece)) {
                        if(takenPiece.isQueen == FALSE) {
                            checkValidSimpleMove(takenPiece, board, blackTiles, whitePieces, blackPieces);
                            checkValidTakeMove(takenPiece, board, blackTiles, whitePieces, blackPieces);
                        }
                        else {
                            checkQueenValidSimpleMove(takenPiece, board, blackTiles, whitePieces, blackPieces);
                        }
                    }
                }
                else if (currentPlayer == BLACK) {
                    takenPiece = initMovement(blackPieces, board.nbPieces, event, defaultPiece);
                    if(FALSE == isSamePiece(defaultPiece, takenPiece)) {
                        if(takenPiece.isQueen == FALSE) {
                            checkValidSimpleMove(takenPiece, board, blackTiles, whitePieces, blackPieces);
                            checkValidTakeMove(takenPiece, board, blackTiles, whitePieces, blackPieces);
                        }
                        else{
                            checkQueenValidSimpleMove(takenPiece, board, blackTiles, whitePieces, blackPieces);
                        }
                    }
                }
            }
            break;

        case SDL_MOUSEBUTTONUP :
            if(event.button.button == SDL_BUTTON_LEFT){
                struct tile tileDrop = {-1, -1, -1, -1, -1, -1, FALSE};
                struct tile defaultTile = {-1, -1, -1, -1, -1, -1, FALSE};
                tileDrop = isValidDeplacement(board, blackTiles, event);

                if(FALSE == isSameTile(defaultTile, tileDrop)) {
                    if(currentPlayer == WHITE) {
                        initKillPiece(tileDrop, whitePieces, blackPieces, blackTiles, board);
                        upgradeToQueen(tileDrop, whitePieces, board);
                        movePieceInNewTile(tileDrop, whitePieces, blackTiles, board);
                        currentPlayer = BLACK;
                    }
                    else {
                        initKillPiece(tileDrop, blackPieces, whitePieces, blackTiles, board);
                        upgradeToQueen(tileDrop, blackPieces, board);
                        movePieceInNewTile(tileDrop, blackPieces, blackTiles, board);
                        currentPlayer = WHITE;
                    }
                }

                resetValidMove(board, blackTiles);
                endMovement(whitePieces, board.nbPieces);
                endMovement(blackPieces, board.nbPieces);
            }
            break;
        }

        for(int i=0; i < board.nbPieces; i++){
            if(whitePieces[i].isPicked == TRUE){
                whitePieces[i].renderPosX = event.button.x;
                whitePieces[i].renderPosY = event.button.y;
            }
            if(blackPieces[i].isPicked == TRUE){
                blackPieces[i].renderPosX = event.button.x;
                blackPieces[i].renderPosY = event.button.y;
            }
        }
        drawGame(renderer, whitePieces, blackPieces, board, whitePieceImage, blackPieceImage, whiteQueenImage, blackQueenImage, yellow, blackTiles);
    }while(gameState == CONTINUE);

    Quit:
    if(NULL != whitePieceImage)
        SDL_DestroyTexture(whitePieceImage);
    if(NULL != blackPieceImage)
        SDL_DestroyTexture(blackPieceImage);
    if(NULL != whiteQueenImage)
        SDL_DestroyTexture(whiteQueenImage);
    if(NULL != blackQueenImage)
        SDL_DestroyTexture(blackQueenImage);
    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}

