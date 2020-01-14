#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "properties.h"
#include "graphicRendering.h"

int initWindow(SDL_Window **window, SDL_Renderer **renderer, int w, int h){
    if(0 != SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return -1;
    }
    if(0 != TTF_Init()){
        fprintf(stderr, "Erreur TTF_Init : %s", SDL_GetError());
        return -1;
    }
    if(0 != SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED, window, renderer)){
        fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
        return -1;
    }
    return 0;
}

int renderPickedPiece(SDL_Renderer *renderer, struct piece pieces[], struct board board, SDL_Texture *pieceImage, SDL_Texture *queenImage) {
    SDL_Rect imageParams = {0, 0, 0, 0};
    SDL_Rect pieceParams = {0, 0, 0, 0};
    if(0 != SDL_QueryTexture(pieceImage, NULL, NULL, &imageParams.w, &imageParams.h)){
        fprintf(stderr, "Erreur SDL_QueryTexture : %s", SDL_GetError());
        return -1;
    }
    if(0 != SDL_QueryTexture(queenImage, NULL, NULL, &imageParams.w, &imageParams.h)){
        fprintf(stderr, "Erreur SDL_QueryTexture : %s", SDL_GetError());
        return -1;
    }

    for(int i=0; i<board.nbPieces; i++) {

        if(pieces[i].isPicked == TRUE) {
            pieceParams.h = pieces[i].height * 1.2;
            pieceParams.w = pieces[i].width * 1.2;
            if(pieces[i].renderPosX >= board.width - 15) {
                pieceParams.x = board.width - OFFSET_WIDTH - 15;
            }
            else if(pieces[i].renderPosX <= 55) {
                pieceParams.x = 10;
            }
            else {
                pieceParams.x = pieces[i].renderPosX - OFFSET_WIDTH;
            }

            if(pieces[i].renderPosY >= board.height - 10) {
                pieceParams.y = board.width - OFFSET_HEIGHT - 10;
            }
            else if(pieces[i].renderPosY <= 55) {
                pieceParams.y = 10;
            }
            else {
                pieceParams.y = pieces[i].renderPosY - OFFSET_HEIGHT;
            }
            if(pieces[i].isQueen == TRUE) {
                if(0 != SDL_RenderCopy(renderer, queenImage, NULL, &pieceParams)){
                    fprintf(stderr, "Erreur SDL_RenderCopy : %s", SDL_GetError());
                    return -1;
                }
            }
            else {
                if(0 != SDL_RenderCopy(renderer, pieceImage, NULL, &pieceParams)){
                    fprintf(stderr, "Erreur SDL_RenderCopy : %s", SDL_GetError());
                    return -1;
                }
            }
        }
    }
    return 0;
}

int drawGame(SDL_Renderer *renderer, struct piece whitePieces[],
             struct piece blackPieces[], struct board board,
             SDL_Texture *whitePieceImage, SDL_Texture *blackPieceImage,
             SDL_Texture *whiteQueenImage, SDL_Texture *blackQueenImage,
             SDL_Color background, struct tile blackTiles[]) {
    SDL_RenderClear(renderer);
    if(0 != setWindowColor(renderer, background)) {
        fprintf(stderr, "Erreur setWindowColor : %s", SDL_GetError());
        return -1;
    }

    if(0 != drawBoard(renderer, board, blackTiles)) {
        fprintf(stderr, "Erreur drawboard : %s", SDL_GetError());
        return -1;
    }

    if(0 != drawPieces(renderer, whitePieces, board, whitePieceImage, whiteQueenImage)) {
        fprintf(stderr, "Erreur drawPieces : %s", SDL_GetError());
        return -1;
    }
    if(0 != drawPieces(renderer, blackPieces, board, blackPieceImage, blackQueenImage)) {
        fprintf(stderr, "Erreur drawPieces : %s", SDL_GetError());
        return -1;
    }

     if(0 != renderPickedPiece(renderer, blackPieces, board, blackPieceImage, blackQueenImage)) {
        fprintf(stderr, "Erreur renderPickedPiece : %s", SDL_GetError());
        return -1;
    }
     if(0 != renderPickedPiece(renderer, whitePieces, board, whitePieceImage, whiteQueenImage)) {
        fprintf(stderr, "Erreur renderPickedPiece : %s", SDL_GetError());
        return -1;
    }

    return 0;
}

int updateView(SDL_Renderer *renderer) {
    SDL_RenderPresent(renderer);
    return 0;
}

int drawInfos(SDL_Renderer *renderer
              , struct board board
              , SDL_Texture *textureTxt
              , int currentPlayer
              , SDL_Texture *blackPieceImage
              , SDL_Texture *whitePieceImage) {
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(textureTxt, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = { board.size * 60 + OFFSET_WIDTH, 10, texW, texH };
    SDL_RenderCopy(renderer, textureTxt, NULL, &dstrect);

    SDL_Rect pieceParams = {0, 0, 0, 0};
    SDL_Texture *pieceToRender = NULL;
    pieceParams.w = WIDTH_PIECE;
    pieceParams.h = HEIGHT_PIECE;
    pieceParams.x = board.size * 60 + OFFSET_WIDTH + texW;
    pieceParams.y = 20;
    if(currentPlayer == WHITE) {
        pieceToRender = whitePieceImage;
    }
    else {
        pieceToRender = blackPieceImage;
    }
    if(0 != SDL_RenderCopy(renderer, pieceToRender, NULL, &pieceParams)){
        fprintf(stderr, "Erreur SDL_RenderCopy : %s", SDL_GetError());
        return -1;
    }

    return 0;
}

int setWindowColor(SDL_Renderer *renderer, SDL_Color color){
    if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1;
    if(SDL_RenderClear(renderer) < 0)
        return -1;
    return 0;
}

SDL_Texture *loadText(const char fontPath[], SDL_Renderer *renderer, char sentence[], SDL_Color color){
    TTF_Font * font = TTF_OpenFont(fontPath, 50);
    SDL_Surface *tmp = NULL;
    SDL_Texture *texture = NULL;
    tmp = TTF_RenderText_Solid(font, sentence, color);

    if(NULL == tmp)
    {
        fprintf(stderr, "Erreur TTF_RenderText_Solid : %s", SDL_GetError());
        TTF_CloseFont(font);
        return NULL;
    }
    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    if(NULL == texture)
    {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        TTF_CloseFont(font);
        return NULL;
    }
    TTF_CloseFont(font);
    return texture;
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

int drawBorder(SDL_Renderer *renderer, struct board board) {
    SDL_Rect rect = {board.posX, board.posY, board.height, board.width};
    if(0 != SDL_SetRenderDrawColor(renderer, 170, 100, 50, 255)){
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        return -1;
    };
    SDL_RenderFillRect(renderer, &rect);
    return 0;
}

int drawBoard(SDL_Renderer *renderer, struct board board, struct tile blackTiles[]){
    if(0 != drawBorder(renderer, board)){
        fprintf(stderr, "Erreur drawBorder : %s", SDL_GetError());
        return -1;
    }
    SDL_Rect rect = {board.posX + 5, board.posY + 5, board.height - 10, board.width - 10};
    if(0 != SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255)){
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        return -1;
    };
    SDL_RenderFillRect(renderer, &rect);
    int nbTiles = ((board.size * board.size) / 2);

    for(int i = 0; i<nbTiles; i++){
        if(blackTiles[i].isPossibleMove == TRUE) {
            if(0 != SDL_SetRenderDrawColor(renderer, board.colorPicked.r, board.colorPicked.g, board.colorPicked.b, board.colorPicked.a)){
                fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
                return -1;
            }
        }
        else {
            if(0 != SDL_SetRenderDrawColor(renderer, board.colorDefault.r, board.colorDefault.g, board.colorDefault.b, board.colorDefault.a)){
                fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
                return -1;
            }
        }

        if(0 != SDL_RenderFillRect(renderer, &blackTiles[i].params)){
            fprintf(stderr, "Erreur SDL_RenderFillRects : %s", SDL_GetError());
            return -1;
        }
    }

    return 0;
}

int drawPieces(SDL_Renderer *renderer, struct piece pieces[], struct board board, SDL_Texture *pieceImage, SDL_Texture *queenImage){
    SDL_Rect imageParams = {0, 0, 0, 0};
    SDL_Rect pieceParams = {0, 0, 0, 0};

    for(int i=0; i<board.nbPieces; i++) {

        if(pieces[i].isPicked != TRUE && pieces[i].isDeath != TRUE) {
            pieceParams.w = pieces[i].width;
            pieceParams.h = pieces[i].height;
            pieceParams.x = (((pieces[i].width + SPACING_WIDTH) * pieces[i].posX) - OFFSET_WIDTH);
            pieceParams.y = (((pieces[i].height + SPACING_HEIGHT) * pieces[i].posY) - OFFSET_HEIGHT);

            if(pieces[i].isQueen == TRUE) {
                if(0 != SDL_RenderCopy(renderer, queenImage, NULL, &pieceParams)){
                    fprintf(stderr, "Erreur SDL_RenderCopy : %s", SDL_GetError());
                    return -1;
                }
            }
            else {
                if(pieces[i].canMove == FALSE) {
                    SDL_SetTextureAlphaMod(pieceImage, 150);
                }
                else {
                    SDL_SetTextureAlphaMod(pieceImage, 255);
                }
                if(0 != SDL_RenderCopy(renderer, pieceImage, NULL, &pieceParams)){
                    fprintf(stderr, "Erreur SDL_RenderCopy : %s", SDL_GetError());
                    return -1;
                }
            }
        }
    }
    return 0;
}
