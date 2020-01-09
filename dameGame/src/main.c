#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "properties.h"
#include "graphicRendering.h"
#include "board.h"
#include "piece.h"
#include "queen.h"
#include "tile.h"

int main(int argc, char *argv[]){
    freopen("stdout.txt", "w", stdout);
    freopen("stderr.txt", "w", stderr);
    int gameState = CONTINUE;
    int currentPlayer = WHITE;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int statut = 0;

    board board;
    initBoard(&board);
    piece whitePieces[board.nbPieces];
    piece blackPieces[board.nbPieces];
    int nbTile = ((board.size * board.size) / 2);
    tile blackTiles[nbTile];

    initTiles(blackTiles, board.size);

    SDL_Color yellow = {255, 240, 140, 0};
    SDL_Color green = {40, 150, 40, 0};
    SDL_Color black = {0, 0, 0, 0};
    if(0 != initWindow(&window, &renderer, 1000, 740))
        goto Quit;
    SDL_Texture *whitePieceImage = NULL;
    whitePieceImage = loadImage("assets/whitePiece.bmp", renderer);
    SDL_Texture *blackPieceImage = NULL;
    blackPieceImage = loadImage("assets/blackPiece.bmp", renderer);
    SDL_Texture *blackQueenImage = NULL;
    blackQueenImage = loadImage("assets/blackQueen.bmp", renderer);
    SDL_Texture *whiteQueenImage = NULL;
    whiteQueenImage = loadImage("assets/whiteQueen.bmp", renderer);

    SDL_Texture * txtAuTourDes = NULL;
    txtAuTourDes = loadText("fonts/Nunito-SemiBold.ttf", renderer, "Au tour des : ", black);

    initPieces(whitePieces, board.nbPieces, WHITE, board.size);
    initPieces(blackPieces, board.nbPieces, BLACK, board.size);

    drawGame(renderer, whitePieces, blackPieces, board, whitePieceImage, blackPieceImage, whiteQueenImage, blackQueenImage, yellow, blackTiles);
    const piece defaultPiece = {-1, -1, -1, -1, -1, -1, FALSE, FALSE, BLACK};
    const tile defaultTile = {-1, -1, -1, -1, -1, -1, FALSE};
    piece takenPiece = {-1, -1, -1, -1, -1, -1, FALSE, FALSE, BLACK};
    tile tileDrop = {-1, -1, -1, -1, -1, -1, FALSE};
    int direction = 0;

    SDL_Event event;
    do {

        SDL_WaitEvent(&event);
        switch(event.type){
        case SDL_QUIT :
            goto Quit;
            break;

        case SDL_MOUSEBUTTONDOWN :
            if(event.button.button == SDL_BUTTON_LEFT){
                if(currentPlayer == WHITE) {
                    takenPiece = initMovement(whitePieces, board.nbPieces, event, defaultPiece);
                    if(FALSE == isSamePiece(defaultPiece, takenPiece)) {
                        if(takenPiece.isQueen == FALSE) {
                            checkValidSimpleMove(takenPiece, board, blackTiles, whitePieces, blackPieces);
                            checkValidTakeMove(takenPiece, board, blackTiles, whitePieces, blackPieces);
                        }
                        else {
                            checkQueenValidSimpleMove(takenPiece, board, blackTiles, whitePieces, blackPieces);
                            checkQueenValidTakeMove(takenPiece, board, blackTiles, whitePieces, blackPieces);
                        }
                    }
                }
                else if (currentPlayer == BLACK) {
                    takenPiece = initMovement(blackPieces, board.nbPieces, event, defaultPiece);
                    if(FALSE == isSamePiece(defaultPiece, takenPiece)) {
                        if(takenPiece.isQueen == FALSE) {
                            checkValidSimpleMove(takenPiece, board, blackTiles, whitePieces, blackPieces);
                            checkValidTakeMove(takenPiece, board, blackTiles, whitePieces, blackPieces);
                        }
                        else{
                            checkQueenValidSimpleMove(takenPiece, board, blackTiles, blackPieces, whitePieces);
                            checkQueenValidTakeMove(takenPiece, board, blackTiles, whitePieces, blackPieces);
                        }
                    }
                }
            }
            break;

        case SDL_MOUSEBUTTONUP :
            if(event.button.button == SDL_BUTTON_LEFT){
                tileDrop = isValidDeplacement(board, blackTiles, event);
                if(FALSE == isSameTile(defaultTile, tileDrop)) {
                    if(currentPlayer == WHITE) {
                        if(takenPiece.isQueen == TRUE) {
                            direction = getDirection(takenPiece, tileDrop, board);
                            killWithQueen(tileDrop, board, blackTiles, whitePieces, blackPieces, direction);
                        }
                        else {
                            initKillPiece(tileDrop, whitePieces, blackPieces, blackTiles, board);
                            upgradeToQueen(tileDrop, whitePieces, board);
                        }
                        movePieceInNewTile(tileDrop, whitePieces, blackTiles, board);
                        currentPlayer = BLACK;
                    }
                    else {
                        if(takenPiece.isQueen == TRUE) {
                            direction = getDirection(takenPiece, tileDrop, board);
                            killWithQueen(tileDrop, board, blackTiles, blackPieces, whitePieces, direction);
                        }
                        else {
                            initKillPiece(tileDrop, blackPieces, whitePieces, blackTiles, board);
                            upgradeToQueen(tileDrop, blackPieces, board);
                        }
                        movePieceInNewTile(tileDrop, blackPieces, blackTiles, board);
                        currentPlayer = WHITE;
                    }
                }

                resetValidMove(board, blackTiles);
                endMovement(whitePieces, board.nbPieces);
                endMovement(blackPieces, board.nbPieces);
            }
            break;
        }

        for(int i=0; i < board.nbPieces; i++){
            if(whitePieces[i].isPicked == TRUE){
                whitePieces[i].renderPosX = event.button.x;
                whitePieces[i].renderPosY = event.button.y;
            }
            if(blackPieces[i].isPicked == TRUE){
                blackPieces[i].renderPosX = event.button.x;
                blackPieces[i].renderPosY = event.button.y;
            }
        }
        drawGame(renderer, whitePieces, blackPieces, board, whitePieceImage, blackPieceImage, whiteQueenImage, blackQueenImage, yellow, blackTiles);
        drawInfos(renderer, board, txtAuTourDes, currentPlayer, blackPieceImage, whitePieceImage);

        updateView(renderer);
    }while(gameState == CONTINUE);

    Quit:
    /// Destroy Texture
    if(NULL != whitePieceImage)
        SDL_DestroyTexture(whitePieceImage);
    if(NULL != blackPieceImage)
        SDL_DestroyTexture(blackPieceImage);
    if(NULL != whiteQueenImage)
        SDL_DestroyTexture(whiteQueenImage);
    if(NULL != blackQueenImage)
        SDL_DestroyTexture(blackQueenImage);
    if(NULL != txtAuTourDes)
        SDL_DestroyTexture(txtAuTourDes);

    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return statut;
}
