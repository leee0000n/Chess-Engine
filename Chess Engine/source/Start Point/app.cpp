#include "app.h"

#include "Globals.h"
#include "Util.h"
#include "Constants.h"
#include "EventHandler.h"
#include "Chess.h"
#include "ChessEngine.h"
#include "LTimer.h"


#include <SDL.h>
#include <iostream>

//extern int counter;

std::array<std::string, 128> b = { "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", "0", "0", "0", "0", "0", "0", "0", "0",
									 "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", "0", "0", "0", "0", "0", "0", "0", "0",
									 "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", "0", "0", "0", "0", "0", "0", "0", "0",
									 "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "0", "0", "0", "0", "0", "0", "0", "0",
									 "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", "0", "0", "0", "0", "0", "0", "0", "0",
									 "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", "0", "0", "0", "0", "0", "0", "0", "0",
									 "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", "0", "0", "0", "0", "0", "0", "0", "0",
									 "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "0", "0", "0", "0", "0", "0", "0", "0" };

void chessGame() {
	std::string FEN = "K3k2r/3ppp2/8/n7/8/8/8/6b1 b k - 0 1";
	const int depth = 5;
	const int seed = 33;

	Chess game;
	game.loadFromFEN(FEN);
	game.initEngine(depth, seed, BLACK_AS_2, FEN);

	EventHandler eventHandler({ &game });

	while (!quit) {
		// Event handling 
		if (!game.isEnginesTurnToPlay()) eventHandler.pollEvent(); // 1. d4 e6 2. e4 Qf6 3. Qg4 Nc6  ENPASSANT PROBLEM
		else {
			/*LTimer timer;
			timer.start();*/
			game.playEngineMove();
			/*int time = timer.getTicks();
			std::cout << counter / time * 1000 << '\n';
			std::cout << time << "\n";*/
		}

		// Rendering
		SDL_RenderClear(gRenderer);
		game.renderBoard();
		game.renderPossibleMoves();
		game.renderPieces();

		SDL_RenderPresent(gRenderer);
	}
}

void app::run() {
	if (!init()) {
		printf("Failed to initialize");
	}
	else {
		if (!(loadMedia("resource/chess.png"))) {
			printf("Failed to load media");
		}
		else
		{
			chessGame();
		}
	}

	close();
}