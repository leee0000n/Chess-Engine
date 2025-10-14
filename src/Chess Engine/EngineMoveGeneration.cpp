#include "ChessEngine.h"

int knightOffsets[8] = {-33, -31, // 2 rows above
										  -18, -14, // 1 row above
										  18, 14,   // 1 row below
										  33, 31 };  // 2 rows below
int directionalOffsets[8] = {-16, -15, 1, 17, 16, 15, -1, -17};

int ChessEngine::generatePseudoLegalMoves(int* pseudoLegalMoves, int*piecePositions, int*board, int*lookup, char castlingRights, int enpassantSquare, int colourToPlay, int blackKingSquare, int whiteKingSquare) {
	int index = 0;
	int start = colourToPlay == 1 ? 1 : 17;
	for (int i = start; i < start + 16; i++) {
		int sourceSquare = piecePositions[i];
		if (sourceSquare < 0) continue;

		int piece = EXTRACT_PIECE_TYPE(board[sourceSquare]);
		int pieceColour = EXTRACT_PIECE_COLOUR(board[sourceSquare]);

		if (colourToPlay == WHITE_AS_1) {
			switch (piece) {
			case PAWN:
				generatePawnWhite(pseudoLegalMoves, index, sourceSquare, colourToPlay, enpassantSquare, board, lookup);
				break;
			case KNIGHT:
				generateKnight(pseudoLegalMoves, index, sourceSquare, colourToPlay, board, lookup);
				break;
			case BISHOP:
				generateDiagonal(pseudoLegalMoves, index, 7, sourceSquare, colourToPlay, board, lookup);
				break;
			case ROOK:
				generateSraight(pseudoLegalMoves, index, 7, sourceSquare, colourToPlay, board, lookup);
				break;
			case QUEEN:
				generateDiagonal(pseudoLegalMoves, index, 7, sourceSquare, colourToPlay, board, lookup);
				generateSraight(pseudoLegalMoves, index, 7, sourceSquare, colourToPlay, board, lookup);
				break;
			case KING:
				generateKingWhite(pseudoLegalMoves, index, sourceSquare, board, lookup, castlingRights);
				break;
			default:
				break;
			}
		}
		else if (colourToPlay == BLACK_AS_2) {
			switch (piece) {
			case PAWN:
				generatePawnBlack(pseudoLegalMoves, index, sourceSquare, colourToPlay, enpassantSquare, board, lookup);
				break;
			case KNIGHT:
				generateKnight(pseudoLegalMoves, index, sourceSquare, colourToPlay, board, lookup);
				break;
			case BISHOP:
				generateDiagonal(pseudoLegalMoves, index, 7, sourceSquare, colourToPlay, board, lookup);
				break;
			case ROOK:
				generateSraight(pseudoLegalMoves, index, 7, sourceSquare, colourToPlay, board, lookup);
				break;
			case QUEEN:
				generateDiagonal(pseudoLegalMoves, index, 7, sourceSquare, colourToPlay, board, lookup);
				generateSraight(pseudoLegalMoves, index, 7, sourceSquare, colourToPlay, board, lookup);
				break;
			case KING:
				generateKingBlack(pseudoLegalMoves, index, sourceSquare, board, lookup, castlingRights);
				break;
			default:
				break;
			}
		}
		
		
	}

	return index;
}

inline void ChessEngine::generatePawnWhite(int* pseudoLegalMoves, int& index, int sourceSquare, int colourToPlay, int enpassantSquare, int*board, int* lookup) {
	// // Regular piece move and pawn promotion
	// if square in front of pawn is empty
	if (board[sourceSquare - 16] == 0) {
		
		// If pawn on back rank, generate promotion moves
		if ((sourceSquare - 16) / 16 == 0) {
			for (int piece = KNIGHT_PROMO; piece <= QUEEN_PROMO; piece++) {
				pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare - 16, piece, 0, 127);
				index++;
			}
		}
		else {
			pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare - 16, QUIET_MOVE, 0, 127);
			index++;

			// If pawn is on starting square
			if (sourceSquare / 16 == 6) {

				// If square two squares away from pawn is empty
				if (board[sourceSquare - 32] == 0) {
					pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare - 32, DOUBLE_PAWN_MOVE, 0, 127);
					index++;
				}
			}
		}
	}

	// // Diagonal capture and pawn promotion
	if (!(sourceSquare - 17 & 0x88) && board[sourceSquare - 17] > 0) {
		// If piece to capture is not same colour as piece being moved
		if (EXTRACT_PIECE_COLOUR(board[sourceSquare - 17]) != colourToPlay) {
			
			// If target square is on back rank, generate promotion moves
			if ((sourceSquare - 17) / 16 == 0) {
				for (int piece = KNIGHT_CAPTURE_PROMO; piece <= QUEEN_CAPTURE_PROMO; piece++) {
					pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare - 17, piece, board[sourceSquare - 17], lookup[sourceSquare - 17]);
					index++;
				}
			}
			else {
				pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare - 17, CAPTURE, board[sourceSquare - 17], lookup[sourceSquare - 17]);
				index++;
			}
		}
	}

	if (!(sourceSquare - 15 & 0x88) && board[sourceSquare - 15] != 0) {
		// If piece to capture is not same colour as piece being moved
		if (EXTRACT_PIECE_COLOUR(board[sourceSquare - 15]) != colourToPlay) {
			
			// If target square is on back rank, generate promotion moves
			if ((sourceSquare - 15) / 16 == 0) {
				for (int piece = KNIGHT_CAPTURE_PROMO; piece <= QUEEN_CAPTURE_PROMO; piece++) {
					pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare - 15, piece, board[sourceSquare - 15], lookup[sourceSquare - 15]);
					index++;
				}
			}
			else {
				pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare - 15, CAPTURE, board[sourceSquare - 15], lookup[sourceSquare - 15]);
				index++;
			}
		}
	}

	// // Enpassant capture
	// If square to left is enpassant square
	if (enpassantSquare == sourceSquare - 1) {
		pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare - 17, ENPASSANT_CAPTURE, PAWN + BLACK, lookup[sourceSquare - 1]);
		index++;
	}
	
	// If square to right is enpassant square
	if (enpassantSquare == sourceSquare + 1) {
		pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare - 15, ENPASSANT_CAPTURE, PAWN + BLACK, lookup[sourceSquare + 1]);
		index++;
	}
}

inline void ChessEngine::generatePawnBlack(int* pseudoLegalMoves, int& index, int sourceSquare, int colourToPlay, int enpassantSquare, int*board, int* lookup) {
	// // Regular piece move and pawn promotion
	// if square in front of pawn is empty
	if (board[sourceSquare + 16] == 0) {

		// If pawn on back rank, generate promotion moves
		if ((sourceSquare + 16) / 16 == 7) {
			for (int piece = KNIGHT_PROMO; piece <= QUEEN_PROMO; piece++) {
				pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare + 16, piece, 0, 127);
				index++;
			}
		}
		else {
			pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare + 16, QUIET_MOVE, 0, 127);
			index++;

			// If pawn is on starting square
			if (sourceSquare / 16 == 1) {

				// If square two squares away from pawn is empty
				if (board[sourceSquare + 32] == 0) {
					pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare + 32, DOUBLE_PAWN_MOVE, 0, 127);
					index++;
				}
			}
		}
	}

	// // Diagonal capture and pawn promotion
	if (!(sourceSquare + 17 & 0x88) && board[sourceSquare + 17] != 0) {
		// If piece to capture is not same colour as piece being moved
		if (EXTRACT_PIECE_COLOUR(board[sourceSquare + 17]) != colourToPlay) {

			// If target square is on back rank, generate promotion moves
			if ((sourceSquare + 17) / 16 == 7) {
				for (int piece = KNIGHT_CAPTURE_PROMO; piece <= QUEEN_CAPTURE_PROMO; piece++) {
					pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare + 17, piece, board[sourceSquare + 17], lookup[sourceSquare + 17]);
					index++;
				}
			}
			else {
				pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare + 17, CAPTURE, board[sourceSquare + 17], lookup[sourceSquare + 17]);
				index++;
			}
		}
	}

	if (!(sourceSquare + 15 & 0x88) && board[sourceSquare + 15] != 0) {
		// If piece to capture is not same colour as piece being moved
		if (EXTRACT_PIECE_COLOUR(board[sourceSquare + 15]) != colourToPlay) {

			// If target square is on back rank, generate promotion moves
			if ((sourceSquare + 15) / 16 == 7) {
				for (int piece = KNIGHT_CAPTURE_PROMO; piece <= QUEEN_CAPTURE_PROMO; piece++) {
					pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare + 15, piece, board[sourceSquare + 15], lookup[sourceSquare + 15]);
					index++;
				}
			}
			else {
				pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare + 15, CAPTURE, board[sourceSquare + 15], lookup[sourceSquare + 15]);
				index++;
			}
		}
	}

	// // Enpassant capture
	// If square to left is enpassant square
	if (enpassantSquare == sourceSquare - 1) {
		pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare + 15, ENPASSANT_CAPTURE, PAWN + WHITE, lookup[sourceSquare - 1]);
		index++;
	}

	// If square to right is enpassant square
	if (enpassantSquare == sourceSquare + 1) {
		pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare + 17, ENPASSANT_CAPTURE, PAWN + WHITE, lookup[sourceSquare + 1]);
		index++;
	}
}

inline void ChessEngine::generateKnight(int* pseudoLegalMoves, int& index, int sourceSquare, int colourToPlay, int*board, int* lookup) {
	for (int offset : knightOffsets) {
		int targetSquare = sourceSquare + offset;

		// If move is off the board then go to next offset
		if (targetSquare & 0x88) continue;

		// If there is a piece on target square
		if (board[targetSquare] != 0) {

			// If piece on the square is same colour
			if (EXTRACT_PIECE_COLOUR(board[targetSquare]) == colourToPlay) continue;

			pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, targetSquare, CAPTURE, board[targetSquare], lookup[targetSquare]);
			index++;
			continue;
		}
		// If move is not a capture
		pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, targetSquare, QUIET_MOVE, 0, 127);
		index++;
	}
}

inline void ChessEngine::generateDiagonal(int* pseudoLegalMoves, int& index, int max, int sourceSquare, int colourToPlay, int*board, int* lookup) {
	for (int i = 1; i < 8; i+=2) {
		for (int j = 1; j < max + 1; j++) {
			int targetSquare = sourceSquare + directionalOffsets[i] * j;

			// Check if square is off board
			if (targetSquare & 0x88) break;

			// Check if there is piece on target square
			if (board[targetSquare] != 0) {

				// Check if target square contains piece of same colour
				if (EXTRACT_PIECE_COLOUR(board[targetSquare]) == colourToPlay) break;

				pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, targetSquare, CAPTURE, board[targetSquare], lookup[targetSquare]);
				index++;
				break;
			}
			pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, targetSquare, QUIET_MOVE, 0, 127);
			index++;
		}
	}
}

inline void ChessEngine::generateSraight(int* pseudoLegalMoves, int& index, int max, int sourceSquare, int colourToPlay, int*board, int* lookup) {
	for (int i = 0; i < 8; i += 2) {
		for (int j = 1; j < max + 1; j++) {
			int targetSquare = sourceSquare + directionalOffsets[i] * j;

			// Check if square is off board
			if (targetSquare & 0x88) break;

			// Check if there is piece on target square
			if (board[targetSquare] != 0) {

				// Check if target square contains piece of same colour
				if (EXTRACT_PIECE_COLOUR(board[targetSquare]) == colourToPlay) break;

				pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, targetSquare, CAPTURE, board[targetSquare], lookup[targetSquare]);
				index++;
				break;
			}
			pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, targetSquare, QUIET_MOVE, 0, 127);
			index++;

		}
	}
}

inline void ChessEngine::generateKingWhite(int* pseudoLegalMoves, int& index, int sourceSquare, int*board, int* lookup, char castlingRights) {
	// Generate normal moves
	generateDiagonal(pseudoLegalMoves, index, 1, sourceSquare, WHITE_AS_1, board, lookup);
	generateSraight(pseudoLegalMoves, index, 1, sourceSquare, WHITE_AS_1, board, lookup);

	// Kingside castling
	if (castlingRights & 1) {
		// Check if no pieces inbetween
		if (board[sourceSquare + 1] == 0 && board[sourceSquare + 2] == 0 && board[sourceSquare + 3] == WHITE + ROOK) {
			pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare + 2, KING_CASTLE, 0, 127);
			index++;
		}
	}
	// Queenside castling
	if (castlingRights & 2) {
		if (board[sourceSquare - 1] == 0 && board[sourceSquare - 2] == 0 && board[sourceSquare - 3] == 0 && board[sourceSquare - 4] == WHITE + ROOK) {
			pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare - 2, QUEEN_CASTLE, 0, 127);
			index++;
		}
	}
}

inline void ChessEngine::generateKingBlack(int* pseudoLegalMoves, int& index, int sourceSquare, int*board, int* lookup, char castlingRights) {
	// Generate normal moves
	generateDiagonal(pseudoLegalMoves, index, 1, sourceSquare, BLACK_AS_2, board, lookup);
	generateSraight(pseudoLegalMoves, index, 1, sourceSquare, BLACK_AS_2, board, lookup);

	// Kingside castling
	if (castlingRights & 4) {
		// Check if no pieces inbetween
		if (board[sourceSquare + 1] == 0 && board[sourceSquare + 2] == 0 && board[sourceSquare + 3] == BLACK + ROOK) {
			pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare + 2, KING_CASTLE, 0, 127);
			index++;
		}
	}
	// Queenside castling
	if (castlingRights & 8) {
		if (board[sourceSquare - 1] == 0 && board[sourceSquare - 2] == 0 && board[sourceSquare - 3] == 0 && board[sourceSquare - 4] == BLACK + ROOK) {
			pseudoLegalMoves[index] = ENCODE_MOVE(sourceSquare, sourceSquare - 2, QUEEN_CASTLE, 0, 127);
			index++;
		}
	}
}
