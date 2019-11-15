#include "properties.h"

#ifndef GRAPHICRENDERING_H_INCLUDED
#define GRAPHICRENDERING_H_INCLUDED

int initWindow(SDL_Window **, SDL_Renderer **, int, int, SDL_Color);
int drawGame(SDL_Renderer *, struct piece[], struct piece[], int, int, SDL_Texture *, SDL_Texture *);
int setWindowColor(SDL_Renderer *, SDL_Color);
SDL_Texture* loadImage(const char[], SDL_Renderer *);
int drawboard(SDL_Renderer *, int);
int drawPieces(SDL_Renderer *, struct piece[], int, SDL_Texture *);

#endif // GRAPHICRENDERING_H_INCLUDED
