#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "properties.h"
#include "graphicRendering.h"

int initWindow(SDL_Window **window, SDL_Renderer **renderer, int w, int h, SDL_Color background){
    if(0 != SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return -1;
    }
    if(0 != SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_SHOWN, window, renderer)){
        fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
        return -1;
    }
    if(0 != setWindowColor(*renderer, background)) {
        fprintf(stderr, "Erreur setWindowColor : %s", SDL_GetError());
        return -1;
    }

    return 0;
}

int drawGame(SDL_Renderer *renderer, struct piece whitePieces[], struct piece blackPieces[], int nbPieces, int boardSize, SDL_Texture *whitePieceImage, SDL_Texture *blackPieceImage) {
    if(0 != drawboard(renderer, boardSize)) {
        fprintf(stderr, "Erreur drawboard : %s", SDL_GetError());
        return -1;
    }

    if(0 != drawPieces(renderer, whitePieces, nbPieces, whitePieceImage)) {
        fprintf(stderr, "Erreur drawPieces : %s", SDL_GetError());
        return -1;
    }
    if(0 != drawPieces(renderer, blackPieces, nbPieces, blackPieceImage)) {
        fprintf(stderr, "Erreur drawPieces : %s", SDL_GetError());
        return -1;
    }
    SDL_RenderPresent(renderer);
    return 0;
}

int setWindowColor(SDL_Renderer *renderer, SDL_Color color){
    if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1;
    if(SDL_RenderClear(renderer) < 0)
        return -1;
    return 0;
}

SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer){
    SDL_Surface *tmp = NULL;
    SDL_Texture *texture = NULL;
    tmp = SDL_LoadBMP(path);
    SDL_SetColorKey(tmp, 1, SDL_MapRGB(tmp->format, 0, 0, 255));

    if(NULL == tmp)
    {
        fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
        return NULL;
    }
    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    if(NULL == texture)
    {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }
    return texture;
}

int drawboard(SDL_Renderer *renderer, int size){
    int nbSquare = (size * size);
    SDL_Rect damierWhite[nbSquare/2];
    SDL_Rect damierBlack[nbSquare/2];
    int posX=0; int posY=0; int idxW = 0; int idxB = 0;

    for(int i = 0; i<nbSquare; i++){
        if(((i + posY) % 2) == 0){
            damierBlack[idxB].w = 60;
            damierBlack[idxB].h = 60;
            damierBlack[idxB].x = 10 + posX*60;
            damierBlack[idxB].y = 10 + posY*60;
            idxB++;
        }
        else{
            damierWhite[idxW].w = 60;
            damierWhite[idxW].h = 60;
            damierWhite[idxW].x = 10 + posX*60;
            damierWhite[idxW].y = 10 + posY*60;
            idxW++;
        }
        if(posX == size-1){
            posX = 0;
            posY++;
        }
        else{
            posX++;
        }
    };

    if(0 != SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255)){
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        return -1;
    }
    if(0 != SDL_RenderFillRects(renderer, damierBlack, nbSquare/2)){
        fprintf(stderr, "Erreur SDL_RenderFillRects : %s", SDL_GetError());
        return -1;
    }
    if(0 != SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255)){
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        return -1;
    }
    if(0 != SDL_RenderFillRects(renderer, damierWhite, nbSquare/2)){
        fprintf(stderr, "Erreur SDL_RenderFillRects : %s", SDL_GetError());
        return -1;
    }
    return 0;
}

int drawPieces(SDL_Renderer *renderer, struct piece pieces[], int nbPieces, SDL_Texture *pieceImage){
    SDL_Rect imageParams = {0, 0, 0, 0};
    SDL_Rect pieceParams = {0, 0, 0, 0};

    if(0 != SDL_QueryTexture(pieceImage, NULL, NULL, &imageParams.w, &imageParams.h)){
        fprintf(stderr, "Erreur SDL_QueryTexture : %s", SDL_GetError());
        return -1;
    }

    for(int i=0; i<nbPieces; i++) {

        pieceParams.w = imageParams.w/10 + pieces[i].width;
        pieceParams.h = imageParams.h/15 + pieces[i].height;
        pieceParams.x = (((pieceParams.w + SPACING_WIDTH) * pieces[i].posX) - OFFSET_WIDTH);
        pieceParams.y = (((pieceParams.h + SPACING_HEIGHT) * pieces[i].posY) - OFFSET_HEIGHT);

        if(0 != SDL_RenderCopy(renderer, pieceImage, NULL, &pieceParams)){
            fprintf(stderr, "Erreur SDL_RenderCopy : %s", SDL_GetError());
            return -1;
        }
    }
    return 0;
}