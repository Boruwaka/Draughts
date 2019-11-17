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
            pieces[pieceIndex].isQueen = FALSE;
            pieces[pieceIndex].color = color;
            pieces[pieceIndex].isPicked = FALSE;
            pieces[pieceIndex].posY = posY;
            pieces[pieceIndex].posX = posX;
            pieces[pieceIndex].height = HEIGHT_PIECE;
            pieces[pieceIndex].width = WIDTH_PIECE;
            pieces[pieceIndex].renderPosX = 0;
            pieces[pieceIndex].renderPosY = 0;
            pieceIndex++;
        }

        posX++;
        if(posX > boardSize){
            posX = 1;
            posY++;
        }

    }while(pieceIndex != nbPieces);
}

void initMovement(struct piece pieces[], int nbPieces,SDL_Event event) {
    for(int i=0; i < nbPieces; i++){
        if(event.button.y < ((pieces[i].height + SPACING_HEIGHT) * pieces[i].posY)
        && event.button.y > ((pieces[i].height + SPACING_HEIGHT) * pieces[i].posY) - OFFSET_HEIGHT
        && event.button.x < ((pieces[i].width + SPACING_WIDTH) * pieces[i].posX)
        && event.button.x > ((pieces[i].width + SPACING_WIDTH) * pieces[i].posX) - OFFSET_WIDTH) {
            pieces[i].isPicked = TRUE;
        }
    }
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

void checkValidSimpleMove(struct piece piece, struct board board, struct tile blackTiles[]) {
    int nbTiles = ((board.size * board.size) / 2);
    if(piece.isQueen == FALSE) {
        if(piece.color == WHITE) {
            for(int i = 0; i<nbTiles; i++) {
                if(blackTiles[i].posY == piece.posY) {
                    if (blackTiles[i].posX == piece.posX || blackTiles[i].posX == piece.posX - 2) {
                        blackTiles[i].isPossibleMove = TRUE;

                    }
                }
            }
        }
        else {
            for(int i = 0; i<nbTiles; i++) {
                if(blackTiles[i].posY == piece.posY - 2) {
                    if (blackTiles[i].posX == piece.posX || blackTiles[i].posX == piece.posX - 2) {
                        blackTiles[i].isPossibleMove = TRUE;
                    }
                }
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

    initPieces(whitePieces, board.nbPieces, WHITE, board.size);
    initPieces(blackPieces, board.nbPieces, BLACK, board.size);

    drawGame(renderer, whitePieces, blackPieces, board, whitePieceImage, blackPieceImage, yellow, blackTiles);

    SDL_Event event;
    do {
        SDL_WaitEvent(&event);
        switch(event.type){
        case SDL_QUIT :
            goto Quit;
            break;

        case SDL_MOUSEBUTTONDOWN :
            if(event.button.button == SDL_BUTTON_LEFT){
                if(currentPlayer == WHITE) {
                    initMovement(whitePieces, board.nbPieces, event);
                }
                else {
                    initMovement(blackPieces, board.nbPieces, event);
                }
            }
            break;

        case SDL_MOUSEBUTTONUP :
            if(event.button.button == SDL_BUTTON_LEFT){
                if(currentPlayer == WHITE) {
                    endMovement(whitePieces, board.nbPieces);
                }
                else {
                    endMovement(blackPieces, board.nbPieces);
                }
            }
            break;
        }

        resetValidMove(board, blackTiles);
        for(int i=0; i < board.nbPieces; i++){
            if(whitePieces[i].isPicked == TRUE){
                whitePieces[i].renderPosX = event.button.x;
                whitePieces[i].renderPosY = event.button.y;
                checkValidSimpleMove(whitePieces[i], board, blackTiles);
            }
            if(blackPieces[i].isPicked == TRUE){
                blackPieces[i].renderPosX = event.button.x;
                blackPieces[i].renderPosY = event.button.y;
                checkValidSimpleMove(blackPieces[i], board, blackTiles);
            }
        }
        drawGame(renderer, whitePieces, blackPieces, board, whitePieceImage, blackPieceImage, yellow, blackTiles);
    }while(gameState == CONTINUE);

    Quit:
    if(NULL != whitePieceImage)
        SDL_DestroyTexture(whitePieceImage);
    if(NULL != blackPieceImage)
        SDL_DestroyTexture(blackPieceImage);
    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}

