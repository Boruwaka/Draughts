#include "properties.h"

#ifndef GRAPHICRENDERING_H_INCLUDED
#define GRAPHICRENDERING_H_INCLUDED

int initWindow(SDL_Window **, SDL_Renderer **, int, int, SDL_Color);
int drawBorder(SDL_Renderer *, struct board);
int drawGame(SDL_Renderer *, struct piece[], struct piece[], struct board, SDL_Texture *, SDL_Texture *);
int setWindowColor(SDL_Renderer *, SDL_Color);
SDL_Texture* loadImage(const char[], SDL_Renderer *);
int drawBoard(SDL_Renderer *, struct board);
int drawPieces(SDL_Renderer *, struct piece[], struct board, SDL_Texture *);

#endif // GRAPHICRENDERING_H_INCLUDED
