#include "ChessEngine.h"

int knightoffsets[8] = {-33, -31, // 2 rows above
										  -18, -14, // 1 row above
										  18, 14,   // 1 row below
										  33, 31 };  // 2 rows below
int directionaloffsets[8] = {-16, -15, 1, 17, 16, 15, -1, -17};

int ChessEngine::makeMove(int encodedMove, int encodedInfo, int* board, int* piecePositions, int* lookupPiece) {
	int source = EXTRACT_SOURCE_SQUARE(encodedMove);
	int target = EXTRACT_TARGET_SQUARE(encodedMove);
	int colourToPlay = EXTRACT_COLOUR_TO_PLAY(encodedInfo);

	int piece = EXTRACT_PIECE_TYPE(board[source]);

	encodedInfo = UPDATE_ENPASSANT_SQUARE(encodedInfo, 127);

	switch (EXTRACT_MOVE_CODE(encodedMove)) {
	case QUIET_MOVE:
		// Update board
		board[target] = board[source];
		board[source] = 0;

		// Update lookup 
		lookupPiece[target] = lookupPiece[source];
		lookupPiece[source] = 0;

		// Update piece
		piecePositions[lookupPiece[target]] = target;
		break;
	case DOUBLE_PAWN_MOVE:
		// Update board
		board[target] = board[source];
		board[source] = 0;

		// Update lookup 
		lookupPiece[target] = lookupPiece[source];
		lookupPiece[source] = 0;

		// Update piece
		piecePositions[lookupPiece[target]] = target;

		encodedInfo = UPDATE_ENPASSANT_SQUARE(encodedInfo, target);

		break;
	case KING_CASTLE:
		// // Move king
		// Update board
		board[target] = board[source];
		board[source] = 0;

		// Update lookup 
		lookupPiece[target] = lookupPiece[source];
		lookupPiece[source] = 0;

		// Update piece
		piecePositions[lookupPiece[target]] = target;

		// Move rook
		// Update board
		board[source + 1] = board[source + 3];
		board[source + 3] = 0;

		// Update lookup 
		lookupPiece[source + 1] = lookupPiece[source + 3];
		lookupPiece[source + 3] = 0;

		// Update piece
		piecePositions[lookupPiece[source + 1]] = source + 1;
		break;
	case QUEEN_CASTLE:
		// // Move king
		// Update board
		board[target] = board[source];
		board[source] = 0;

		// Update lookup 
		lookupPiece[target] = lookupPiece[source];
		lookupPiece[source] = 0;

		// Update piece
		piecePositions[lookupPiece[target]] = target;

		// Move rook
		// Update board
		board[source - 1] = board[source - 4];
		board[source - 4] = 0;

		// Update lookup 
		lookupPiece[source - 1] = lookupPiece[source - 4];
		lookupPiece[source - 4] = 0;

		// Update piece
		piecePositions[lookupPiece[source - 1]] = source - 1;
		break;
	case CAPTURE:
		// // Erase captured piece
		piecePositions[lookupPiece[target]] = -1;

		// // Move piece
		// Update board
		board[target] = board[source];
		board[source] = 0;

		// Update lookup 
		lookupPiece[target] = lookupPiece[source];
		lookupPiece[source] = 0;

		// Update piece
		piecePositions[lookupPiece[target]] = target;
		break;
	case ENPASSANT_CAPTURE:

		// // Erase captured piece
		if (colourToPlay == WHITE_AS_1) {
			piecePositions[lookupPiece[target + 16]] = -1;
			lookupPiece[target + 16] = 0;
			board[target + 16] = 0;
		}
		else if (colourToPlay == BLACK_AS_2) {
			piecePositions[lookupPiece[target - 16]] = -1;
			lookupPiece[target - 16] = 0;
			board[target - 16] = 0;
		}
		
		// // Move piece
		// Update board
		board[target] = board[source];
		board[source] = 0;

		// Update lookup 
		lookupPiece[target] = lookupPiece[source];
		lookupPiece[source] = 0;

		// Update piece
		piecePositions[lookupPiece[target]] = target;
		break;
	case KNIGHT_PROMO:
		// Update board
		board[target] = KNIGHT + colourToPlay * 16;
		board[source] = 0;

		// Update lookup 
		lookupPiece[target] = lookupPiece[source];
		lookupPiece[source] = 0;

		// Update piece
		piecePositions[lookupPiece[target]] = target;
		break;
	case BISHOP_PROMO:
		// Update board
		board[target] = BISHOP + colourToPlay * 16;
		board[source] = 0;

		// Update lookup 
		lookupPiece[target] = lookupPiece[source];
		lookupPiece[source] = 0;

		// Update piece
		piecePositions[lookupPiece[target]] = target;
		break;
	case ROOK_PROMO:
		// Update board
		board[target] = ROOK + colourToPlay * 16;
		board[source] = 0;

		// Update lookup 
		lookupPiece[target] = lookupPiece[source];
		lookupPiece[source] = 0;

		// Update piece
		piecePositions[lookupPiece[target]] = target;
		break;
	case QUEEN_PROMO:
		// Update board
		board[target] = QUEEN + colourToPlay * 16;
		board[source] = 0;

		// Update lookup 
		lookupPiece[target] = lookupPiece[source];
		lookupPiece[source] = 0;

		// Update piece
		piecePositions[lookupPiece[target]] = target;
		break;
	case KNIGHT_CAPTURE_PROMO:
		// // Erase captured piece
		piecePositions[lookupPiece[target]] = -1;

		// Update board
		board[target] = KNIGHT + colourToPlay * 16;
		board[source] = 0;

		// Update lookup 
		lookupPiece[target] = lookupPiece[source];
		lookupPiece[source] = 0;

		// Update piece
		piecePositions[lookupPiece[target]] = target;
		break;
	case BISHOP_CAPTURE_PROMO:
		// // Erase captured piece
		piecePositions[lookupPiece[target]] = -1;

		// Update board
		board[target] = BISHOP + colourToPlay * 16;
		board[source] = 0;

		// Update lookup 
		lookupPiece[target] = lookupPiece[source];
		lookupPiece[source] = 0;

		// Update piece
		piecePositions[lookupPiece[target]] = target;
		break;
	case ROOK_CAPTURE_PROMO:
		// // Erase captured piece
		piecePositions[lookupPiece[target]] = -1;

		// Update board
		board[target] = ROOK + colourToPlay * 16;
		board[source] = 0;

		// Update lookup 
		lookupPiece[target] = lookupPiece[source];
		lookupPiece[source] = 0;

		// Update piece
		piecePositions[lookupPiece[target]] = target;
		break;
	case QUEEN_CAPTURE_PROMO:
		// // Erase captured piece
		piecePositions[lookupPiece[target]] = -1;

		// Update board
		board[target] = QUEEN + colourToPlay * 16;
		board[source] = 0;

		// Update lookup 
		lookupPiece[target] = lookupPiece[source];
		lookupPiece[source] = 0;

		// Update piece
		piecePositions[lookupPiece[target]] = target;
		break;
	default:		
		throw;
		break;
	}


	// Change king square
	if (piece == KING) {
		if (colourToPlay == WHITE_AS_1) {
			encodedInfo = UPDATE_WHITE_KING_SQUARE(encodedInfo, target);
			encodedInfo = UPDATE_CASTLING_RIGHTS(encodedInfo, (0b1100 & EXTRACT_CASTLING_RIGHTS(encodedInfo)));
		}
		else if (colourToPlay == BLACK_AS_2) {
			encodedInfo = UPDATE_BLACK_KING_SQUARE(encodedInfo, target);
			encodedInfo = UPDATE_CASTLING_RIGHTS(encodedInfo, (0b0011 & EXTRACT_CASTLING_RIGHTS(encodedInfo)));
		}

		
	}
	else if (piece == ROOK) {
		if (source % 16 == 0) {
			const int castleRights = EXTRACT_CASTLING_RIGHTS(encodedInfo) & (colourToPlay == 1 ? 0b1101 : 0b0111);
			encodedInfo = UPDATE_CASTLING_RIGHTS(encodedInfo, castleRights);
		}
		else if (source % 16 == 7) {
			const int castleRights = EXTRACT_CASTLING_RIGHTS(encodedInfo) & (colourToPlay == 1 ? 0b1110 : 0b1011);
			encodedInfo = UPDATE_CASTLING_RIGHTS(encodedInfo, castleRights);
		}
	}

	// Change turn 
	encodedInfo = UPDATE_COLOUR_TO_PLAY(encodedInfo, SWITCH_COLOUR(colourToPlay));


	return encodedInfo;
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ChessEngine::unmakeMove(int encodedMove, int encodedInfo, int* board, int* piecePositions, int* lookupPiece) {
	int source = EXTRACT_SOURCE_SQUARE(encodedMove);
	int target = EXTRACT_TARGET_SQUARE(encodedMove);
	int colourToPlay = EXTRACT_COLOUR_TO_PLAY(encodedInfo);

	int piece = EXTRACT_PIECE_TYPE(board[source]);

	switch (EXTRACT_MOVE_CODE(encodedMove)) {
	case QUIET_MOVE:
		// Update board
		board[source] = board[target];
		board[target] = 0;

		// Update lookup 
		lookupPiece[source] = lookupPiece[target];
		lookupPiece[target] = 0;

		// Update piece
		piecePositions[lookupPiece[source]] = source;
		break;
	case DOUBLE_PAWN_MOVE:
		// Update board
		board[source] = board[target];
		board[target] = 0;

		// Update lookup 
		lookupPiece[source] = lookupPiece[target];
		lookupPiece[target] = 0;

		// Update piece
		piecePositions[lookupPiece[source]] = source;
		break;
	case KING_CASTLE:
		// // Move king back
		// Update board
		board[source] = board[target];
		board[target] = 0;

		// Update lookup 
		lookupPiece[source] = lookupPiece[target];
		lookupPiece[target] = 0;

		// Update piece
		piecePositions[lookupPiece[source]] = source;
		 
		// Move rook back
		// Update board
		board[source + 3] = board[source + 1];
		board[source + 1] = 0;

		// Update lookup 
		lookupPiece[source + 3] = lookupPiece[source + 1];
		lookupPiece[source + 1] = 0;

		// Update piece
		piecePositions[lookupPiece[source + 3]] = source + 3;
		break;
	case QUEEN_CASTLE:
		// // Move king back
		// Update board
		board[source] = board[target];
		board[target] = 0;

		// Update lookup 
		lookupPiece[source] = lookupPiece[target];
		lookupPiece[target] = 0;

		// Update piece
		piecePositions[lookupPiece[source]] = source;

		// Move rook back
		// Update board
		board[source - 4] = board[source - 1];
		board[source - 1] = 0;

		// Update lookup 
		lookupPiece[source - 4] = lookupPiece[source - 1];
		lookupPiece[source - 1] = 0;

		// Update piece
		piecePositions[lookupPiece[source - 4]] = source - 4;
		break;
	case CAPTURE:
		// // Add captured piece back
		piecePositions[EXTRACT_CAPTURED_LOOKUP(encodedMove)] = target;

		// // Move piece back
		// Update board
		board[source] = board[target];
		board[target] = EXTRACT_PIECE_CAPTURED(encodedMove);

		// Update lookup 
		lookupPiece[source] = lookupPiece[target];
		lookupPiece[target] = EXTRACT_CAPTURED_LOOKUP(encodedMove);

		// Update piece
		piecePositions[lookupPiece[source]] = source;
		break;
	case ENPASSANT_CAPTURE:

		// // Add captured piece back
		if (colourToPlay == WHITE_AS_1) {
			piecePositions[EXTRACT_CAPTURED_LOOKUP(encodedMove)] = target + 16;
			lookupPiece[target + 16] = EXTRACT_CAPTURED_LOOKUP(encodedMove);
			board[target + 16] = EXTRACT_PIECE_CAPTURED(encodedMove);
		}
		else if (colourToPlay == BLACK_AS_2) {
			piecePositions[EXTRACT_CAPTURED_LOOKUP(encodedMove)] = target - 16;
			lookupPiece[target - 16] = EXTRACT_CAPTURED_LOOKUP(encodedMove);
			board[target - 16] = EXTRACT_PIECE_CAPTURED(encodedMove);
		}
		// // Move piece back
		// Update board
		board[source] = board[target];
		board[target] = 0;

		// Update lookup 
		lookupPiece[source] = lookupPiece[target];
		lookupPiece[target] = 0;

		// Update piece
		piecePositions[lookupPiece[source]] = source;
		break;
	case KNIGHT_PROMO:
		// Update board
		board[source] = PAWN + colourToPlay * 16;
		board[target] = 0;

		// Update lookup 
		lookupPiece[source] = lookupPiece[target];
		lookupPiece[target] = 0;

		// Update piece
		piecePositions[lookupPiece[source]] = source;
		break;
	case BISHOP_PROMO:
		// Update board
		board[source] = PAWN + colourToPlay * 16;
		board[target] = 0;

		// Update lookup 
		lookupPiece[source] = lookupPiece[target];
		lookupPiece[target] = 0;

		// Update piece
		piecePositions[lookupPiece[source]] = source;
		break;
	case ROOK_PROMO:
		// Update board
		board[source] = PAWN + colourToPlay * 16;
		board[target] = 0;

		// Update lookup 
		lookupPiece[source] = lookupPiece[target];
		lookupPiece[target] = 0;

		// Update piece
		piecePositions[lookupPiece[source]] = source;
		break;
	case QUEEN_PROMO:
		// Update board
		board[source] = PAWN + colourToPlay * 16;
		board[target] = 0;

		// Update lookup 
		lookupPiece[source] = lookupPiece[target];
		lookupPiece[target] = 0;

		// Update piece
		piecePositions[lookupPiece[source]] = source;
		break;
	case KNIGHT_CAPTURE_PROMO:
		// // Add captured piece back
		piecePositions[EXTRACT_CAPTURED_LOOKUP(encodedMove)] = target;

		// Update board
		board[source] = PAWN + colourToPlay * 16;
		board[target] = EXTRACT_PIECE_CAPTURED(encodedMove);

		// Update lookup 
		lookupPiece[source] = lookupPiece[target];
		lookupPiece[target] = EXTRACT_CAPTURED_LOOKUP(encodedMove);

		// Update piece
		piecePositions[lookupPiece[source]] = source;
		break;
	case BISHOP_CAPTURE_PROMO:
		// // Add captured piece back
		piecePositions[EXTRACT_CAPTURED_LOOKUP(encodedMove)] = target;

		// Update board
		board[source] = PAWN + colourToPlay * 16;
		board[target] = EXTRACT_PIECE_CAPTURED(encodedMove);

		// Update lookup 
		lookupPiece[source] = lookupPiece[target];
		lookupPiece[target] = EXTRACT_CAPTURED_LOOKUP(encodedMove);

		// Update piece
		piecePositions[lookupPiece[source]] = source;
		break;
	case ROOK_CAPTURE_PROMO:
		// // Add captured piece back
		piecePositions[EXTRACT_CAPTURED_LOOKUP(encodedMove)] = target;

		// Update board
		board[source] = PAWN + colourToPlay * 16;
		board[target] = EXTRACT_PIECE_CAPTURED(encodedMove);

		// Update lookup 
		lookupPiece[source] = lookupPiece[target];
		lookupPiece[target] = EXTRACT_CAPTURED_LOOKUP(encodedMove);

		// Update piece
		piecePositions[lookupPiece[source]] = source;
		break;
	case QUEEN_CAPTURE_PROMO:
		// // Add captured piece back
		piecePositions[EXTRACT_CAPTURED_LOOKUP(encodedMove)] = target;

		// Update board
		board[source] = PAWN + colourToPlay * 16;
		board[target] = EXTRACT_PIECE_CAPTURED(encodedMove);

		// Update lookup 
		lookupPiece[source] = lookupPiece[target];
		lookupPiece[target] = EXTRACT_CAPTURED_LOOKUP(encodedMove);

		// Update piece
		piecePositions[lookupPiece[source]] = source;
		break;
	default:
		throw;
		break;
	}
}

// Refactor to make cleaner
bool ChessEngine::verifyMove(int encodedMove, int encodedInfo, int* board, int* piecePositions, int* lookupPiece) {
	int colourToPlay = SWITCH_COLOUR(EXTRACT_COLOUR_TO_PLAY(encodedInfo));
	int kingSquare = -1;

	if (colourToPlay == WHITE_AS_1) {
		kingSquare = EXTRACT_WHITE_KING_SQUARE(encodedInfo);

		// Check if there is a pawn checking the king
		if (board[kingSquare - 15] == BLACK + PAWN) {
			return false;
		}
		if (board[kingSquare - 17] == BLACK + PAWN) {
			return false;
		}
	}
	else if (colourToPlay == BLACK_AS_2) {
		kingSquare = EXTRACT_BLACK_KING_SQUARE(encodedInfo);

		// Check if there is a pawn checking the king
		if (board[kingSquare + 15] == WHITE + PAWN) {
			return false;
		}
		if (board[kingSquare + 17] == WHITE + PAWN) {
			return false;
		}
	}

	// Up Down Left Right offset
	for (int i = 0; i < 8; i+=2) {
		for (int j = 1; j <= 7; j++) {
			int square = kingSquare + directionaloffsets[i] * j;

			if (square & 0x88) break;

			int piece = board[square];

			// If king is same colour as piece on square
			if (colourToPlay == EXTRACT_PIECE_COLOUR(piece)) break;

			// If piece is rook or queen
			if (EXTRACT_PIECE_TYPE(piece) == ROOK || EXTRACT_PIECE_TYPE(piece) == QUEEN) return false;
			else if (piece != 0) break;
		}
	}

	// Diagonal offset
	for (int i = 1; i < 8; i += 2) {
		for (int j = 1; j <= 7; j++) {
			int square = kingSquare + directionaloffsets[i] * j;

			if (square & 0x88) break;

			int piece = board[square];

			// If king is same colour as piece on square
			if (colourToPlay == EXTRACT_PIECE_COLOUR(piece)) break;

			// If piece is rook or queen
			if (EXTRACT_PIECE_TYPE(piece) == BISHOP || EXTRACT_PIECE_TYPE(piece) == QUEEN) return false;
			else if (piece != 0) break;
		}
	}

	for (int offset : knightoffsets) {
		int square = kingSquare + offset;

		if (square & 0x88) continue;

		int piece = board[square];

		// If king is same colour as piece on square
		if (colourToPlay == EXTRACT_PIECE_COLOUR(piece)) continue;

		// If piece is rook or queen
		if (EXTRACT_PIECE_TYPE(piece) == KNIGHT) return false;
	}

	if (EXTRACT_MOVE_CODE(encodedMove) == KING_CASTLE) {
		if (colourToPlay == WHITE_AS_1) {
			// Check if there is a pawn checking the king passing square
			if (board[kingSquare - 18] == BLACK + PAWN) {
				return false;
			}
			else if (board[kingSquare - 16] == BLACK + PAWN ) {
				return false;
			}
			else if (board[kingSquare - 17] == BLACK + PAWN) {
				return false;
			}
			else if (board[kingSquare - 19] == BLACK + PAWN) {
				return false;
			}
		}
		else if (colourToPlay == BLACK_AS_2) {
			// Check if there is a pawn checking the king passing square
			if (board[kingSquare + 14] == WHITE + PAWN) {
				return false;
			}
			if (board[kingSquare + 16] == WHITE + PAWN) {
				return false;
			}
			if (board[kingSquare + 15] == WHITE + PAWN) {
				return false;
			}
			if (board[kingSquare + 13] == WHITE + PAWN) {
				return false;
			}
		}

		// Up Down Left Right offset
		for (int i = 0; i < 8; i += 2) {
			bool check1 = true;
			bool check2 = true;
			for	(int j = 1; j <= 7; j++) {
				if (check1) {
					int square = kingSquare - 1 + directionaloffsets[i] * j;

					int piece = board[square];

					if (square & 0x88) check1 = false;

					// If king is same colour as piece on square
					if (colourToPlay == EXTRACT_PIECE_COLOUR(piece)) check1 = false;

					// If piece is rook or queen
					if ((EXTRACT_PIECE_TYPE(piece) == ROOK || EXTRACT_PIECE_TYPE(piece) == QUEEN) && check1) return false;
					else if (piece != 0) check1 = false;
				}
				if (check2) {
					int sourcesquare = kingSquare - 2 + directionaloffsets[i] * j;

					int piece = board[sourcesquare];

					if (sourcesquare & 0x88) check2 = false;

					// If king is same colour as piece on square
					if (colourToPlay == EXTRACT_PIECE_COLOUR(piece)) check2 = false;

					// If piece is rook or queen
					if ((EXTRACT_PIECE_TYPE(piece) == ROOK || EXTRACT_PIECE_TYPE(piece) == QUEEN) && check2) return false;
					else if (piece != 0) check2 = false;
				}


				if (!check1 && !check2) break;

			}
		}

		// Diagonal offset
		for (int i = 1; i < 8; i += 2) {
			bool check1 = true;
			bool check2 = true;
			for (int j = 1; j <= 7; j++) {
				if (check1) {
					int square = kingSquare - 1 + directionaloffsets[i] * j;

					if (square & 0x88) check1 = false;

					int piece = board[square];

					// If king is same colour as piece on square
					if (colourToPlay == EXTRACT_PIECE_COLOUR(piece)) check1 = false;

					// If piece is rook or queen
					if ((EXTRACT_PIECE_TYPE(piece) == BISHOP || EXTRACT_PIECE_TYPE(piece) == QUEEN) && check1) return false;
					else if (piece != 0) check1 = false;
				}

				if (check2) {
					int square = kingSquare - 2 + directionaloffsets[i] * j;

					if (square & 0x88) check2 = false;

					int piece = board[square];

					// If king is same colour as piece on square
					if (colourToPlay == EXTRACT_PIECE_COLOUR(piece)) check2 = false;

					// If piece is rook or queen
					if ((EXTRACT_PIECE_TYPE(piece) == BISHOP || EXTRACT_PIECE_TYPE(piece) == QUEEN) && check2) return false;
					else if (piece != 0) check2 = false;
				}

				if (!check1 && !check2) break;
			}
		}

		for (int offset : knightoffsets) {
			int square = kingSquare - 1 + offset;

			if (square & 0x88) continue;

			int piece = board[square];

			// If king is same colour as piece on square
			if (colourToPlay == EXTRACT_PIECE_COLOUR(piece)) goto NextCheck;

			// If piece is rook or queen
			if (EXTRACT_PIECE_TYPE(piece) == KNIGHT) return false;
			
			NextCheck:

			square = kingSquare - 2 + offset;

			if (square & 0x88) continue;

			piece = board[square];

			// If king is same colour as piece on square
			if (colourToPlay == EXTRACT_PIECE_COLOUR(piece)) continue;

			// If piece is rook or queen
			if (EXTRACT_PIECE_TYPE(piece) == KNIGHT) return false;
			
		}
	}
	else if (EXTRACT_MOVE_CODE(encodedMove) == QUEEN_CASTLE) {
		if (colourToPlay == WHITE_AS_1) {
			// Check if there is a pawn checking the king
			if (board[kingSquare - 14] == BLACK + PAWN) {
				return false;
			}
			if (board[kingSquare - 16] == BLACK + PAWN) {
				return false;
			}
			// Check if there is a pawn checking the king
			if (board[kingSquare - 13] == BLACK + PAWN) {
				return false;
			}
			if (board[kingSquare - 15] == BLACK + PAWN) {
				return false;
			}
		}
		else if (colourToPlay == BLACK_AS_2) {
			// Check if there is a pawn checking the king
			if (board[kingSquare + 16] == WHITE + PAWN ) {
				return false;
			}
			if (board[kingSquare + 18] == WHITE + PAWN) {
				return false;
			}
			// Check if there is a pawn checking the king
			if (board[kingSquare + 17] == WHITE + PAWN) {
				return false;
			}
			if (board[kingSquare + 19] == WHITE + PAWN) {
				return false;
			}
		}

		// Up Down Left Right offset
		for (int i = 0; i < 8; i += 2) {
			bool check1 = true;
			bool check2 = true;
			for (int j = 1; j <= 7; j++) {
				if (check1) {
					int square = kingSquare + 1 + directionaloffsets[i] * j;

					int piece = board[square];

					if (square & 0x88) check1 = false;

					// If king is same colour as piece on square
					if (colourToPlay == EXTRACT_PIECE_COLOUR(piece)) check1 = false;

					// If piece is rook or queen
					if ((EXTRACT_PIECE_TYPE(piece) == ROOK || EXTRACT_PIECE_TYPE(piece) == QUEEN) && check1) return false;
					else if (piece != 0) check1 = false;
				}
				if (check2) {
					int sourcesquare = kingSquare + 2 + directionaloffsets[i] * j;

					int piece = board[sourcesquare];

					if (sourcesquare & 0x88) check2 = false;

					// If king is same colour as piece on square
					if (colourToPlay == EXTRACT_PIECE_COLOUR(piece)) check2 = false;

					// If piece is rook or queen
					if ((EXTRACT_PIECE_TYPE(piece) == ROOK || EXTRACT_PIECE_TYPE(piece) == QUEEN) && check2) return false;
					else if (piece != 0) check2 = false;
				}

				if (!check1 && !check2) break;

			}
		}

		// Diagonal offset
		for (int i = 1; i < 8; i += 2) {
			bool check1 = true;
			bool check2 = true;
			for (int j = 1; j <= 7; j++) {
				if (check1) {
					int square = kingSquare + 1 + directionaloffsets[i] * j;

					if (square & 0x88) check1 = false;

					int piece = board[square];

					// If king is same colour as piece on square
					if (colourToPlay == EXTRACT_PIECE_COLOUR(piece)) check1 = false;

					// If piece is rook or queen
					if ((EXTRACT_PIECE_TYPE(piece) == BISHOP || EXTRACT_PIECE_TYPE(piece) == QUEEN) && check1) return false;
					else if (piece != 0) check1 = false;
				}

				if (check2) {
					int square = kingSquare + 2 + directionaloffsets[i] * j;

					if (square & 0x88) check2 = false;

					int piece = board[square];

					// If king is same colour as piece on square
					if (colourToPlay == EXTRACT_PIECE_COLOUR(piece)) check2 = false;

					// If piece is rook or queen
					if ((EXTRACT_PIECE_TYPE(piece) == BISHOP || EXTRACT_PIECE_TYPE(piece) == QUEEN) && check2) return false;
					else if (piece != 0) check2 = false;
				}

				if (!check1 && !check2) break;
			}
		}

		for (int offset : knightoffsets) {
			int square = kingSquare + 1 + offset;

			if (square & 0x88) continue;

			int piece = board[square];

			// If king is same colour as piece on square
			if (colourToPlay == EXTRACT_PIECE_COLOUR(piece)) goto NextCheck2;

			// If piece is rook or queen
			if (EXTRACT_PIECE_TYPE(piece) == KNIGHT) return false;

			NextCheck2:

			square = kingSquare + 2 + offset;

			if (square & 0x88) continue;

			piece = board[square];

			// If king is same colour as piece on square
			if (colourToPlay == EXTRACT_PIECE_COLOUR(piece)) continue;

			// If piece is rook or queen
			if (EXTRACT_PIECE_TYPE(piece) == KNIGHT) return false;
		}
	}

	return true;
}

bool ChessEngine::playPlayerMove(int encodedMove, int encodedInfo) {
	
	// If colour of move played doesn't match with colour to play in engine
	if (EXTRACT_COLOUR_TO_PLAY(encodedInfo) != m_colourToPlay) throw; 

	int newInfo = makeMove(encodedMove, encodedInfo, m_board, m_piecePositions, m_lookupPiece);

	if (verifyMove(encodedMove, newInfo, m_board, m_piecePositions, m_lookupPiece)) {
		m_colourToPlay = SWITCH_COLOUR(m_colourToPlay);
		m_castlingRights = EXTRACT_CASTLING_RIGHTS(newInfo);
		m_enpassantSquare = EXTRACT_ENPASSANT_SQUARE(newInfo);
		m_whiteKingSquare = EXTRACT_WHITE_KING_SQUARE(newInfo);
		m_blackKingSquare = EXTRACT_BLACK_KING_SQUARE(newInfo);
		return true;
	}

	unmakeMove(encodedMove, encodedInfo, m_board, m_piecePositions, m_lookupPiece);

	return false;
}