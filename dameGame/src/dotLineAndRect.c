#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    int statut = EXIT_FAILURE;
    SDL_Color orange = {255, 100, 100, 100};

    /* Initialisation, création de la fenêtre et du renderer. */
    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    window = SDL_CreateWindow("Jeu de dames RIOC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              420, 420, SDL_WINDOW_SHOWN);
    if(NULL == window)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == renderer)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_TARGET, 200, 200);
    if(NULL == texture)
    {
        fprintf(stderr, "Erreur SDL_CreateTexture : %s", SDL_GetError());
        goto Quit;
    }

    /* C’est à partir de maintenant que ça se passe. */
    if(0 != SDL_SetRenderDrawColor(renderer, orange.r, orange.g, orange.b, orange.a))
    {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        goto Quit;
    }

    if(0 != SDL_RenderClear(renderer))
    {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        goto Quit;
    }

    SDL_Delay(500);

    // Dessine 3 points indépendants
    /*SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawPoint(renderer, 50, 50);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawPoint(renderer, 100, 100);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderDrawPoint(renderer, 150, 150);*/

    // Déssine une ligne = tableau de point
    /*SDL_Point point[640];
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    size_t i = 0;
    for(i = 0; i < 640; i++)
    {
        point[i].x = i;
        point[i].y = 200;
    }
    SDL_RenderDrawPoints(renderer, point, 640);*/

    // Dessine en liant différents points
    /*SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Point point[6];
    point[0].x = 100;
    point[0].y = 400;
    point[1].x = 300;
    point[1].y = 400;
    point[2].x = 350;
    point[2].y = 300;
    point[3].x = 200;
    point[3].y = 200;
    point[4].x = 50;
    point[4].y = 300;
    point[5].x = 100;
    point[5].y = 400;
    SDL_RenderDrawLines(renderer, point, 6);*/

    // Draw le contour du rectangle en bleu et le rempli en rouge
    /*SDL_Rect rect = {100, 100, 100, 100};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect2 = {101, 101, 101, 101};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect2);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &rect);*/

    // Dessine un damier

    /*SDL_Rect damierWhite[50];
    SDL_Rect damierBlack[50];
    int posX=0; int posY=0; int idxW = 0; int idxB = 0;

    for(int i = 0; i<100; i++){
        if(((i + posY) % 2) != 0){
            damierBlack[idxB].w = 40;
            damierBlack[idxB].h = 40;
            damierBlack[idxB].x = 10 + posX*40;
            damierBlack[idxB].y = 10 + posY*40;
            idxB++;
        }
        else{
            damierWhite[idxW].w = 40;
            damierWhite[idxW].h = 40;
            damierWhite[idxW].x = 10 + posX*40;
            damierWhite[idxW].y = 10 + posY*40;
            idxW++;
        }
        if(posX == 9){
            posX = 0;
            posY++;
        }
        else{
            posX++;
        }
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRects(renderer, damierBlack, 50);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRects(renderer, damierWhite, 50);*/

    // Autre méthode
    /*SDL_Rect rect[50];
    size_t i = 0;
    for(i = 0; i < 50; i++)
    {
        rect[i].w = 40;
        rect[i].h = 40;
        rect[i].x = 10 + 80 * (i % 5) + 40 * ((i / 5) % 2);
        rect[i].y = 10 + 40 * (i / 5);
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRects(renderer, rect, 50);*/

    SDL_RenderPresent(renderer);
    SDL_Delay(5000);

    statut = EXIT_SUCCESS;

Quit:
    if(NULL != texture)
        SDL_DestroyTexture(texture);
    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}
