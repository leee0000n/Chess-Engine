#include "Chess.h"

#include "Constants.h"
#include "Globals.h"


#include <iostream>
#include <SDL.h>

std::array<const int, 8> knightOffsets = { -33, -31, // 2 rows above
										  -18, -14, // 1 row above
										  18, 14,   // 1 row below
										  33, 31 };  // 2 rows below
std::array<const int, 8> directionalOffsets = { -16, -15, 1, 17, 16, 15, -1, -17 };

bool Chess::verifyMove(int moveData) {

	if (m_colourToPlay == WHITE_AS_1) {
		// Check pawn checks
		if (m_board[m_whiteKingSquare - 17] == PAWN + BLACK) return false;
		else if (m_board[m_whiteKingSquare - 15] == PAWN + BLACK) return false;

		// Check for knight checks
		for (int offset : knightOffsets) {
			// If potential checking square is off the board
			if ((m_whiteKingSquare + offset) & 0x88) continue;

			// If checking square contains a horse, move is not valid
			if (m_board[m_whiteKingSquare + offset] == KNIGHT + BLACK) return false;

		}

		// Check for checks in up, down, left and right directions
		for (int i = 0; i < 8; i += 2) {
			for (int j = 1; j < 8; j++) {
				int offset = directionalOffsets[i];

				// If potential checking square is off the board
				if ((m_whiteKingSquare + offset * j) & 0x88) break;

				// If square not empty
				if (m_board[m_whiteKingSquare + offset * j] != 0) {
					// If checking square has rook on it
					if (m_board[(m_whiteKingSquare + offset * j)] == ROOK + BLACK) return false;

					// If checking square has a queen on it
					if (m_board[(m_whiteKingSquare + offset * j)] == QUEEN + BLACK) return false;

					// Go to next direction as piece on square cannot check king due to it being wrong colour or piec
					break;
				}
			}
		}

		// Check for checks in diagonal directions
		for (int i = 1; i < 8; i += 2) {
			for (int j = 1; j < 8; j++) {
				int offset = directionalOffsets[i];

				// If potential checking square is off the board
				if ((m_whiteKingSquare + offset * j) & 0x88) break;

				// If square not empty
				if (m_board[m_whiteKingSquare + offset * j] != 0) {
					// If checking square has rook on it
					if (m_board[(m_whiteKingSquare + offset * j)] == BISHOP + BLACK) return false;

					// If checking square has a queen on it
					if (m_board[(m_whiteKingSquare + offset * j)] == QUEEN + BLACK) return false;

					// Go to next direction as piece on square cannot check king due to it being wrong colour or piec
					break;
				}
			}
		}
	}


	else if (m_colourToPlay == BLACK_AS_2) {
		// Check pawn checks
		if (m_board[m_blackKingSquare + 17] == PAWN + WHITE) return false;
		else if (m_board[m_blackKingSquare + 15] == PAWN + WHITE) return false;

		// Check for knight checks
		for (int offset : knightOffsets) {
			// If potential checking square is off the board
			if ((m_blackKingSquare + offset) & 0x88) continue;

			// If checking square contains a horse, move is not valid
			if (m_board[m_blackKingSquare + offset] == KNIGHT + WHITE) return false;

		}

		// Check for checks in up, down, left and right directions
		for (int i = 0; i < 8; i += 2) {
			for (int j = 1; j < 8; j++) {
				int offset = directionalOffsets[i];

				// If potential checking square is off the board
				if ((m_blackKingSquare + offset * j) & 0x88) break;

				// If square not empty
				if (m_board[m_blackKingSquare + offset * j] != 0) {
					// If checking square has rook on it
					if (m_board[(m_blackKingSquare + offset * j)] == ROOK + WHITE) return false;

					// If checking square has a queen on it
					if (m_board[(m_blackKingSquare + offset * j)] == QUEEN + WHITE) return false;

					// Go to next direction as piece on square cannot check king due to it being wrong colour or piec
					break;
				}
			}
		}

		// Check for checks in diagonal directions
		for (int i = 1; i < 8; i += 2) {
			for (int j = 1; j < 8; j++) {
				int offset = directionalOffsets[i];

				// If potential checking square is off the board
				if ((m_blackKingSquare + offset * j) & 0x88) break;

				// If square not empty
				if (m_board[m_blackKingSquare + offset * j] != 0) {
					// If checking square has rook on it
					if (m_board[(m_blackKingSquare + offset * j)] == BISHOP + WHITE) return false;

					// If checking square has a queen on it
					if (m_board[(m_blackKingSquare + offset * j)] == QUEEN + WHITE) return false;

					// Go to next direction as piece on square cannot check king due to it being wrong colour or piec
					break;
				}
			}
		}
	}


	// Check if move is pseudo legal
	return true;
}

int Chess::makeMove(int moveData) {
	int source = EXTRACT_SOURCE_SQUARE(moveData);
	int target = EXTRACT_TARGET_SQUARE(moveData);

	int piece = EXTRACT_PIECE_TYPE(m_board[source]);

	switch (EXTRACT_MOVE_CODE(moveData)) {
	case QUIET_MOVE:
		// Update m_board
		m_board[target] = m_board[source];
		m_board[source] = 0;
		break;
	case DOUBLE_PAWN_MOVE:
		// Update m_board
		m_board[target] = m_board[source];
		m_board[source] = 0;

		m_enpassantSquare = target;
		break;
	case KING_CASTLE:
		// // Move king
		// Update m_board
		m_board[target] = m_board[source];
		m_board[source] = 0;

		// Move rook
		// Update m_board
		m_board[source + 1] = m_board[source + 3];
		m_board[source + 3] = 0;
		break;
	case QUEEN_CASTLE:
		// // Move king
		// Update m_board
		m_board[target] = m_board[source];
		m_board[source] = 0;

		// Move rook
		// Update m_board
		m_board[source - 1] = m_board[source - 4];
		m_board[source - 4] = 0;
		break;
	case CAPTURE:
		// Captured piece
		m_lastPieceTaken = m_board[target];

		// // Move piece
		// Update m_board
		m_board[target] = m_board[source];
		m_board[source] = 0;
		break;
	case ENPASSANT_CAPTURE:

		// // Erase captured piece
		if (m_colourToPlay == WHITE_AS_1) {
			m_lastPieceTaken = BLACK_AS_2;
			m_board[target + 16] = 0;
		}
		else if (m_colourToPlay == BLACK_AS_2) {
			m_lastPieceTaken = WHITE_AS_1;
			m_board[target - 16] = 0;
		}

		// // Move piece
		// Update m_board
		m_board[target] = m_board[source];
		m_board[source] = 0;
		break;
	case KNIGHT_PROMO:
		// Update m_board
		m_board[target] = KNIGHT + m_colourToPlay * 16;
		m_board[source] = 0;
		break;
	case BISHOP_PROMO:
		// Update m_board
		m_board[target] = BISHOP + m_colourToPlay * 16;
		m_board[source] = 0;
		break;
	case ROOK_PROMO:
		// Update m_board
		m_board[target] = ROOK + m_colourToPlay * 16;
		m_board[source] = 0;
		break;
	case QUEEN_PROMO:
		// Update m_board
		m_board[target] = QUEEN + m_colourToPlay * 16;
		m_board[source] = 0;
		break;
	case KNIGHT_CAPTURE_PROMO:
		// // Erase captured piece
		m_lastPieceTaken = m_board[target];

		// Update m_board
		m_board[target] = KNIGHT + m_colourToPlay * 16;
		m_board[source] = 0;
		break;
	case BISHOP_CAPTURE_PROMO:
		// // Erase captured piece
		m_lastPieceTaken = m_board[target];

		// Update m_board
		m_board[target] = BISHOP + m_colourToPlay * 16;
		m_board[source] = 0;
		break;
	case ROOK_CAPTURE_PROMO:
		// // Erase captured piece
		m_lastPieceTaken = m_board[target];

		// Update m_board
		m_board[target] = ROOK + m_colourToPlay * 16;
		m_board[source] = 0;
		break;
	case QUEEN_CAPTURE_PROMO:
		// // Erase captured piece
		m_lastPieceTaken = m_board[target];

		// Update m_board
		m_board[target] = QUEEN + m_colourToPlay * 16;
		m_board[source] = 0;
		break;
	default:
		throw;
		break;
	}

	// Change king square
	if (piece == KING) {
		if (m_colourToPlay == WHITE_AS_1) {
			m_whiteKingSquare = target;
			m_canWhiteKingsideCastle = false;
			m_canWhiteQueensideCastle = false;
		}
		else if (m_colourToPlay == BLACK_AS_2) {
			m_blackKingSquare = target;
			m_canBlackKingsideCastle = false;
			m_canBlackQueensideCastle = false;
		}


	}
	else if (piece == ROOK) {
		if (m_colourToPlay == WHITE_AS_1) {
			// White queenside rook
			if (source % 16 == 0) {
				m_canWhiteQueensideCastle = false;
			}
			// White kingside rook
			else if (source % 16 == 7) {
				m_canWhiteKingsideCastle = false;
			}
		}
		else if (m_colourToPlay == BLACK_AS_2) {
			// Black queenside rook
			if (source % 16 == 0) {
				m_canBlackQueensideCastle = false;
			}
			// Black kingside rook
			else if (source % 16 == 7) {
				m_canBlackKingsideCastle = false;
			}
		}
		
	}

}

void Chess::unmakeMove(int moveData) {
	int source = EXTRACT_SOURCE_SQUARE(moveData);
	int target = EXTRACT_TARGET_SQUARE(moveData);

	int piece = EXTRACT_PIECE_TYPE(m_board[source]);

	switch (EXTRACT_MOVE_CODE(moveData)) {
	case QUIET_MOVE:
		// Update m_board
		m_board[source] = m_board[target];
		m_board[target] = 0;
		break;
	case DOUBLE_PAWN_MOVE:
		// Update m_board
		m_board[source] = m_board[target];
		m_board[target] = 0;
		break;
	case KING_CASTLE:
		// // Move king back
		// Update m_board
		m_board[source] = m_board[target];
		m_board[target] = 0;

		// Move rook back
		// Update m_board
		m_board[source + 3] = m_board[source + 1];
		m_board[source + 1] = 0;
		break;
	case QUEEN_CASTLE:
		// // Move king back
		// Update m_board
		m_board[source] = m_board[target];
		m_board[target] = 0;

		// Move rook back
		// Update m_board
		m_board[source - 4] = m_board[source - 1];
		m_board[source - 1] = 0;
		break;
	case CAPTURE:

		// // Move piece back
		// Update m_board
		m_board[source] = m_board[target];
		m_board[target] = m_lastPieceTaken;
		break;
	case ENPASSANT_CAPTURE:

		// // Add captured piece back
		if (m_colourToPlay == WHITE_AS_1) {
			m_board[target + 16] = m_lastPieceTaken;
		}
		else if (m_colourToPlay == BLACK_AS_2) {
			m_board[target - 16] = m_lastPieceTaken;
		}
		// // Move piece back
		// Update m_board
		m_board[source] = m_board[target];
		m_board[target] = 0;
		break;
	case KNIGHT_PROMO:
		// Update m_board
		m_board[source] = PAWN + m_colourToPlay * 16;
		m_board[target] = 0;
		break;
	case BISHOP_PROMO:
		// Update m_board
		m_board[source] = PAWN + m_colourToPlay * 16;
		m_board[target] = 0;
		break;
	case ROOK_PROMO:
		// Update m_board
		m_board[source] = PAWN + m_colourToPlay * 16;
		m_board[target] = 0;
		break;
	case QUEEN_PROMO:
		// Update m_board
		m_board[source] = PAWN + m_colourToPlay * 16;
		m_board[target] = 0;
		break;
	case KNIGHT_CAPTURE_PROMO:
		// Update m_board
		m_board[source] = PAWN + m_colourToPlay * 16;
		m_board[target] = m_lastPieceTaken;
		break;
	case BISHOP_CAPTURE_PROMO:
		// Update m_board
		m_board[source] = PAWN + m_colourToPlay * 16;
		m_board[target] = m_lastPieceTaken;
		break;
	case ROOK_CAPTURE_PROMO:
		// Update m_board
		m_board[source] = PAWN + m_colourToPlay * 16;
		m_board[target] = m_lastPieceTaken;
		break;
	case QUEEN_CAPTURE_PROMO:
		// Update m_board
		m_board[source] = PAWN + m_colourToPlay * 16;
		m_board[target] = m_lastPieceTaken;
		break;
	default:
		throw;
		break;
	}
}