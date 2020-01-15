#include "properties.h"

void checkQueenValidSimpleMove(piece currentPiece, board board, tile blackTiles[], piece whitePieces[], piece blackPieces[]);
int checkQueenValidTakeMove(piece currentPiece, board board, tile blackTiles[], piece whitePieces[], piece blackPieces[]);
void killWithQueen(tile dropTile, board board, tile blackTiles[], piece currentPieces[], piece opponentPieces[], int direction);
