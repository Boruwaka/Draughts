#include "properties.h"

int initTiles(struct tile blackTiles[], int size);
void getTilePositionFromClick(SDL_Event event, tile blackTiles[], int nbPieces);
int isSameTile(tile defaultTile, tile tile);
tile* getTileFromCoordonate(tile blackTiles[], int posX, int posY, int nbTiles);
int checkIfTileOccupated(tile blackTile, board board, piece whitePieces[], piece blackPieces[]);
int colorOfPieceOnTile(tile blackTile, board board, piece whitePieces[], piece blackPieces[]);
void resetValidMove(board board, tile blackTiles[]);
tile isValidDeplacement(board board, tile blackTiles[], SDL_Event event);
int setPossibleMoveOnCoordonate(tile blackTiles[], int posX, int posY, int nbTiles, board board, piece whitePieces[], piece blackPieces[]);
