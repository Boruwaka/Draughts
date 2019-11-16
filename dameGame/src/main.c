#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "properties.h"
#include "graphicRendering.h"

void initPieces(struct piece pieces[], int nbPieces, int color, int boardSize) {
    int widthIndex = 1;
    int heightIndex = 1;
    int pieceIndex = 0;

    if(color == WHITE){
        heightIndex = 1;
    }
    else {
        switch (nbPieces){
        case 12:
            heightIndex = 6;
            break;
        case 20:
            heightIndex = 7;
            break;
        case 30:
            heightIndex = 8;
            break;
        }
    }

    do{
        if(((widthIndex + heightIndex) % 2) != 0){
            pieces[pieceIndex].isQueen = FALSE;
            pieces[pieceIndex].color = color;
            pieces[pieceIndex].isPicked = FALSE;
            pieces[pieceIndex].posY = heightIndex;
            pieces[pieceIndex].posX = widthIndex;
            pieces[pieceIndex].height = HEIGHT_PIECE;
            pieces[pieceIndex].width = WIDTH_PIECE;
            pieces[pieceIndex].pickedPosX = 0;
            pieces[pieceIndex].pickedPosY = 0;
            pieceIndex++;
        }

        widthIndex++;
        if(widthIndex > boardSize){
            widthIndex = 1;
            heightIndex++;
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

int main(int argc, char *argv[]){
    freopen("stdout.txt", "w", stdout);
    freopen("stderr.txt", "w", stderr);
    int gameState = CONTINUE;
    int currentPlayer = WHITE;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int statut = 0;
    int nbPieces=0;

    int boardSize = 10;
    switch (boardSize) {
    case 8:
        nbPieces = 12;
        break;
    case 10:
        nbPieces = 20;
        break;
    case 12:
        nbPieces = 30;
        break;
    }
    struct piece whitePieces[nbPieces];
    struct piece blackPieces[nbPieces];

    SDL_Color yellow = {255, 240, 140, 0};
    SDL_Color green = {40, 150, 40, 0};
    if(0 != initWindow(&window, &renderer, 1000, 740, yellow))
        goto Quit;
    SDL_Texture *whitePieceImage = NULL;
    whitePieceImage = loadImage("assets/whitePiece.bmp", renderer);
    SDL_Texture *blackPieceImage = NULL;
    blackPieceImage = loadImage("assets/blackPiece.bmp", renderer);

    initPieces(whitePieces, nbPieces, WHITE, boardSize);
    initPieces(blackPieces, nbPieces, BLACK, boardSize);
    for(int i = 0; i<nbPieces; i++) {
        printf("%d : %d \t%d \t%d \t%d", i, whitePieces[i].isQueen, whitePieces[i].color, whitePieces[i].posY, whitePieces[i].posX);
        printf(" ||| %d \t%d \t%d \t%d", blackPieces[i].isQueen, blackPieces[i].color, blackPieces[i].posY, blackPieces[i].posX);
        printf("\n");
    }

    drawGame(renderer, whitePieces, blackPieces, nbPieces, boardSize, whitePieceImage, blackPieceImage);

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
                    initMovement(whitePieces, nbPieces, event);
                }
                else {
                    initMovement(blackPieces, nbPieces, event);
                }
            }
            break;

        case SDL_MOUSEBUTTONUP :
            if(event.button.button == SDL_BUTTON_LEFT){
                if(currentPlayer == WHITE) {
                    endMovement(whitePieces, nbPieces);
                }
                else {
                    endMovement(blackPieces, nbPieces);
                }
            }
            break;
        }
         for(int i=0; i < nbPieces; i++){
            if(whitePieces[i].isPicked == TRUE){
                whitePieces[i].pickedPosX = event.button.x;
                whitePieces[i].pickedPosY = event.button.y;
            }
        }
        drawGame(renderer, whitePieces, blackPieces, nbPieces, boardSize, whitePieceImage, blackPieceImage);
        SDL_Delay(10);
    }while(gameState == CONTINUE);

     do {
        SDL_WaitEvent(&event);
        switch(event.type){
        case SDL_QUIT :
            goto Quit;
            break;
            }
        }while(1);

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

