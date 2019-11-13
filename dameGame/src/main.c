#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

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

struct piece{
    int posY;
    int posX;
    int height;
    int width;
    int isQueen;
    int color;
};

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

int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h){
    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return -1;
    }
    if(0 != SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_SHOWN, window, renderer))
    {
        fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
        return -1;
    }
    return 0;
}

void drawboard(SDL_Renderer *renderer, int size){
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
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRects(renderer, damierBlack, nbSquare/2);
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRects(renderer, damierWhite, nbSquare/2);
    SDL_RenderPresent(renderer);
}

int setWindowColor(SDL_Renderer *renderer, SDL_Color color){
    if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1;
    if(SDL_RenderClear(renderer) < 0)
        return -1;
    return 0;
}

void drawPieces(SDL_Renderer *renderer, struct piece pieces[], int nbPieces, int color){
    SDL_Rect imageParams = {0, 0, 0, 0};
    SDL_Rect pieceParams = {0, 0, 0, 0};
    SDL_Texture *pieceImage = NULL;

    if(color == WHITE){
        pieceImage = loadImage("assets/whitePiece.bmp", renderer);
    }
    else {
        pieceImage = loadImage("assets/blackPiece.bmp", renderer);
    }

    SDL_QueryTexture(pieceImage, NULL, NULL, &imageParams.w, &imageParams.h);

    for(int i=0; i<nbPieces; i++) {

        pieceParams.w = imageParams.w/10 + pieces[i].width;
        pieceParams.h = imageParams.h/15 + pieces[i].height;
        pieceParams.x = (((pieceParams.w + SPACING_WIDTH) * pieces[i].posX) - OFFSET_WIDTH);
        pieceParams.y = (((pieceParams.h + SPACING_HEIGHT) * pieces[i].posY) - OFFSET_HEIGHT);

        pieces[i].posY = pieceParams.y;
        pieces[i].posX = pieceParams.x;

        SDL_RenderCopy(renderer, pieceImage, NULL, &pieceParams);
        SDL_SetRenderTarget(renderer, pieceImage);
    }


    if(NULL != pieceImage)
        SDL_DestroyTexture(pieceImage);
}

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
            pieces[pieceIndex].posY = heightIndex;
            pieces[pieceIndex].posX = widthIndex;
            pieces[pieceIndex].height = 0;
            pieces[pieceIndex].width = 0;
            pieceIndex++;
        }

        widthIndex++;
        if(widthIndex > boardSize){
            widthIndex = 1;
            heightIndex++;
        }

    }while(pieceIndex != nbPieces);
}

void drawGame(SDL_Renderer *renderer, struct piece whitePieces[], struct piece blackPieces[], int nbPieces, int boardSize) {
    SDL_Color yellow = {255, 240, 140, 0};

    setWindowColor(renderer, yellow);
    SDL_RenderPresent(renderer);
    drawboard(renderer, boardSize);

    drawPieces(renderer, whitePieces, nbPieces, WHITE);
    drawPieces(renderer, blackPieces, nbPieces, BLACK);

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]){
    freopen("printf.txt", "w", stdout);
    int gameState = CONTINUE;
    int currentPlayer = WHITE;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int statut = EXIT_FAILURE;
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

    SDL_Color green = {40, 150, 40, 0};
    if(0 != init(&window, &renderer, 1000, 740))
        goto Quit;

    initPieces(whitePieces, nbPieces, WHITE, boardSize);
    initPieces(blackPieces, nbPieces, BLACK, boardSize);
    for(int i = 0; i<nbPieces; i++) {
        printf("%d : %d \t%d \t%d \t%d", i, whitePieces[i].isQueen, whitePieces[i].color, whitePieces[i].posY, whitePieces[i].posX);
        printf(" ||| %d \t%d \t%d \t%d", blackPieces[i].isQueen, blackPieces[i].color, blackPieces[i].posY, blackPieces[i].posX);
        printf("\n");
    }
    drawGame(renderer, whitePieces, blackPieces, nbPieces, boardSize);
    SDL_Delay(3000);
    drawGame(renderer, whitePieces, blackPieces, nbPieces, boardSize);

    SDL_Event event;
    /*
    do {
        SDL_WaitEvent(&event);
        switch(event.type){
        case SDL_QUIT :
            goto Quit;
            break;

        case SDL_MOUSEBUTTONDOWN :
            if(event.button.button == SDL_BUTTON_LEFT){
                if(event.button.state == SDL_PRESSED ) {
                    if(currentPlayer == WHITE) {
                        for(int i=0; i < nbPieces; i++){
                            if(event.button.y < whitePieces[i].posY + 30
                            && event.button.y > whitePieces[i].posY - 30
                            && event.button.x < whitePieces[i].posX + 30
                            && event.button.x > whitePieces[i].posX - 30) {
                                printf("MATCHED PIECE : %d = \n y: %d  -  x: %d \n y: %d  -  x: %d", i, whitePieces[i].posY, whitePieces[i].posX, event.button.y, event.button.x);
                                drawGame(renderer, whitePieces, blackPieces, nbPieces, boardSize);
                            }
                        }
                    }
                }
                if(event.button.state == SDL_RELEASED) {

                }
            }
        }

    }while(gameState == CONTINUE);
*/
     do {
        SDL_WaitEvent(&event);
        switch(event.type){
        case SDL_QUIT :
            goto Quit;
            break;
            }
        }while(1);
    Quit:
    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}
