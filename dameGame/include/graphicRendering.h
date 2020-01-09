#include "properties.h"

#ifndef GRAPHICRENDERING_H_INCLUDED
#define GRAPHICRENDERING_H_INCLUDED

int initWindow(SDL_Window **, SDL_Renderer **, int, int);
int drawBorder(SDL_Renderer *, struct board);
int drawGame(SDL_Renderer *, struct piece[], struct piece[], struct board, SDL_Texture *, SDL_Texture *, SDL_Texture *, SDL_Texture *, SDL_Color, struct tile[]);
int setWindowColor(SDL_Renderer *, SDL_Color);
SDL_Texture* loadImage(const char[], SDL_Renderer *);
SDL_Texture *loadText(const char[], SDL_Renderer *, char[], SDL_Color);
int drawInfos(SDL_Renderer *, struct board, SDL_Texture *, int, SDL_Texture *, SDL_Texture *);
int updateView(SDL_Renderer *);
int drawBoard(SDL_Renderer *, struct board, struct tile[]);
int drawPieces(SDL_Renderer *, struct piece[], struct board, SDL_Texture *, SDL_Texture *);
int drawPickedPiece(SDL_Renderer *, struct piece[], struct board, SDL_Texture *, SDL_Texture *);

#endif // GRAPHICRENDERING_H_INCLUDED
