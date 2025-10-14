#include "Chess.h"

#include "Constants.h"
#include "Globals.h"

#include <SDL.h>

void Chess::renderBoard() {
	// Render pieceSquareuares
	for (int row = 0; row < 8; row++) {
		for (int column = 0; column < 8; column++) {
			// DARK BROWN = 71, 39, 22
			// LIGHT BROWN = 219, 169, 143;
			int deciding = ((row + 1) % 2 + (column) % 2) % 2; // Alternate pieceSquareuare colours between light and dark brown
			SDL_Rect pieceSquareuare = { column * SCREEN_WIDTH / 8, row * SCREEN_HEIGHT / 8, SCREEN_WIDTH / 8, SCREEN_HEIGHT / 8 };
			SDL_SetRenderDrawColor(gRenderer, 71 + 148 * deciding, 39 + 130 * deciding, 22 + 121 * deciding, 255);
			SDL_RenderFillRect(gRenderer, &pieceSquareuare);
		}
	}

	// Render piece move indicators
	// Condition ensures that indicators are not rendered at the start, before any piece moves
	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
	if (m_movedPieceSource != m_movedPieceTarget) {
		SDL_SetRenderDrawColor(gRenderer, 3, 11, 252, 100);

		SDL_Rect sourceSquare = { (m_movedPieceSource % 16) * SCREEN_WIDTH / 8,
								 (m_movedPieceSource / 16) * SCREEN_HEIGHT / 8,
								 SCREEN_WIDTH / 8,
								 SCREEN_HEIGHT / 8 };
		SDL_RenderFillRect(gRenderer, &sourceSquare);

		SDL_Rect targetSquare = { (m_movedPieceTarget % 16) * SCREEN_WIDTH / 8,
								 (m_movedPieceTarget / 16) * SCREEN_HEIGHT / 8,
								 SCREEN_WIDTH / 8,
								 SCREEN_HEIGHT / 8 };
		SDL_RenderFillRect(gRenderer, &targetSquare);

	}
}

void Chess::renderPieces() {
	for (int square = 0; square < 120; square++) {
		// If index is not in board section of array
		if (square & 0x88) square += 8;

		int piece = m_board[square];

		if (piece == 0) continue;

		// Determine piece's sprite index in gSpriteClips

		int pieceIndex = EXTRACT_PIECE_TYPE(piece) + 6 * (EXTRACT_PIECE_COLOUR(piece) - 1) - 1;

		gSpriteSheetTexture.render(square % 8 * SCREEN_WIDTH / 8 + SCREEN_WIDTH * 0.0375, square / 16 * SCREEN_HEIGHT / 8 + SCREEN_HEIGHT * 0.0375, &gSpriteClips[pieceIndex]);
	}
}

void Chess::renderPossibleMoves() {
	for (int square = 0; square < 120; square++) {
		// If index is not in board section of array
		if (square & 0x88) square += 8;
		if (m_generatedMoves[square] > 0) {
			int column = square % 8;
			int row = square / 16;


			SDL_Rect pieceSquareuare = { column * SCREEN_WIDTH / 8, row * SCREEN_HEIGHT / 8, SCREEN_WIDTH / 8, SCREEN_HEIGHT / 8 };
			SDL_SetRenderDrawColor(gRenderer, 0, 100, 0, 100);
			SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
			SDL_RenderFillRect(gRenderer, &pieceSquareuare);
		}
	}
}