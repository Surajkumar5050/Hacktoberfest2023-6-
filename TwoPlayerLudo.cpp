#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event e;

const int NUM_PLAYERS = 2;
const int NUM_PIECES_PER_PLAYER = 4;
const int BOARD_SIZE = 40;

struct Piece {
    int x, y;
    bool isAtHome;
};

std::vector<Piece> playerPieces[NUM_PLAYERS];
int currentPlayer = 0;
int roll = 0;

void initSDL() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Ludo Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void closeSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void renderBoard() {
    SDL_Rect rect;
    rect.w = SCREEN_WIDTH / 10;
    rect.h = SCREEN_HEIGHT / 10;

    // Draw the board
    for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 10; col++) {
            if ((row == 0 && col >= 4) || (row == 9 && col <= 5) || (row >= 4 && col == 0) || (row <= 5 && col == 9)) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Draw safe spots in black
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Draw the playable area in white
            }
            rect.x = col * rect.w;
            rect.y = row * rect.h;
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    // Render player pieces
    for (int player = 0; player < NUM_PLAYERS; player++) {
        for (const auto& piece : playerPieces[player]) {
            rect.x = piece.x * rect.w;
            rect.y = piece.y * rect.h;
            if (player == 0) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Player 1 pieces in red
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Player 2 pieces in blue
            }
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

int main(int argc, char* args[]) {
    initSDL();

    // Initialize player pieces
    for (int player = 0; player < NUM_PLAYERS; player++) {
        for (int piece = 0; piece < NUM_PIECES_PER_PLAYER; piece++) {
            if (player == 0) {
                playerPieces[player].push_back({4, 9 - piece, true});
            } else {
                playerPieces[player].push_back({5, piece, true});
            }
        }
    }

    bool quit = false;
    while (!quit) {
        SDL_RenderClear(renderer);
        renderBoard();
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    roll = rand() % 6 + 1;  // Roll a random number between 1 and 6
                }
            }
        }
    }

    closeSDL();

    return 0;
}
