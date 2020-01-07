#include "properties.h"

void initPieces(piece pieces[], int nbPieces, int color, int boardSize);
void getPiecePositionFromClick(SDL_Event event, piece pieces[], int nbPieces);
piece initMovement(piece pieces[], int nbPieces, SDL_Event event, piece defaultPiece);
void killPiece(piece *pieceToKill);
void endMovement(piece pieces[], int nbPieces);
int isSamePiece(piece defaultPiece, piece piece);
void checkValidSimpleMove(piece currentPiece, board board, tile blackTiles[], piece whitePieces[], piece blackPieces[]);
void upgradeToQueen(tile tileDrop, piece pieces[], board board);
void initKillPiece(tile dropTile, piece currentPieces[], piece opponentPieces[], tile blackTiles[], board board);
void killPieceOnPosition(piece pieces[], board board, int posX, int posY);
void movePieceInNewTile(tile dropTile, piece pieces[], tile blackTiles[], board board);
void checkValidTakeMove(piece currentPiece, board board, tile blackTiles[], piece whitePieces[], piece blackPieces[]);
