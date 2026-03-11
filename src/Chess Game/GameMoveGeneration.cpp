#include "Chess.h"

constexpr std::array<const int, 8> knightOffsetss = { -33, -31, // 2 rows above
										  -18, -14, // 1 row above
										  18, 14,   // 1 row below
										  33, 31 };  // 2 rows below
constexpr std::array<const int, 8> directionalOffsetss = { -16, -15, 1, 17, 16, 15, -1, -17 };

void Chess::generatePseudoLegalMoves() {
	m_generatedMoves.fill(0);
	

	int piece = EXTRACT_PIECE_TYPE(m_board[m_selectedSquare]);
	int pieceColour = EXTRACT_PIECE_COLOUR(m_board[m_selectedSquare]);

	if (m_colourToPlay == WHITE_AS_1) {
		switch (piece) {
		case PAWN:
			generatePawnWhite(m_selectedSquare);
			break;
		case KNIGHT:
			generateKnight(m_selectedSquare);
			break;
		case BISHOP:
			generateDiagonal(7, m_selectedSquare);
			break;
		case ROOK:
			generateSraight(7, m_selectedSquare);
			break;
		case QUEEN:
			generateDiagonal(7, m_selectedSquare);
			generateSraight(7, m_selectedSquare);
			break;
		case KING:
			generateKingWhite(m_selectedSquare);
			break;
		default:
			break;
		}
	}
	else if (m_colourToPlay == BLACK_AS_2) {
		switch (piece) {
		case PAWN:
			generatePawnBlack(m_selectedSquare);
			break;
		case KNIGHT:
			generateKnight(m_selectedSquare);
			break;
		case BISHOP:
			generateDiagonal(7, m_selectedSquare);
			break;
		case ROOK:
			generateSraight(7, m_selectedSquare);
			break;
		case QUEEN:
			generateDiagonal(7, m_selectedSquare);
			generateSraight(7, m_selectedSquare);
			break;
		case KING:
			generateKingBlack(m_selectedSquare);
			break;
		default:
			break;
		}
	}


	
}

inline void Chess::generatePawnWhite(int sourceSquare) {
	int move = 0;

	// // Regular piece move and pawn promotion
	// if square in front of pawn is empty
	if (m_board[sourceSquare - 16] == 0) {

		// If pawn on back rank, generate promotion moves
		if ((sourceSquare - 16) / 16 == 0) {
			for (int piece = KNIGHT_PROMO; piece <= QUEEN_PROMO; piece++) {
				move = ENCODE_MOVE(sourceSquare, sourceSquare - 16, piece, 0, 127);
				if (verifyMove(move)) m_generatedMoves[sourceSquare - 16] = move;
				
			}
		}
		else {
			move = ENCODE_MOVE(sourceSquare, sourceSquare - 16, QUIET_MOVE, 0, 127);
			if (verifyMove(move)) m_generatedMoves[sourceSquare - 16] = move;
			

			// If pawn is on starting square
			if (sourceSquare / 16 == 6) {

				// If square two squares away from pawn is empty
				if (m_board[sourceSquare - 32] == 0) {
					move = ENCODE_MOVE(sourceSquare, sourceSquare - 32, DOUBLE_PAWN_MOVE, 0, 127);
					if (verifyMove(move)) m_generatedMoves[sourceSquare - 32] = move;
					
				}
			}
		}
	}

	// // Diagonal capture and pawn promotion
	if (!(sourceSquare - 17 & 0x88) && m_board[sourceSquare - 17] > 0) {
		// If piece to capture is not same colour as piece being moved
		if (EXTRACT_PIECE_COLOUR(m_board[sourceSquare - 17]) != m_colourToPlay) {

			// If target square is on back rank, generate promotion moves
			if ((sourceSquare - 17) / 16 == 0) {
				for (int piece = KNIGHT_CAPTURE_PROMO; piece <= QUEEN_CAPTURE_PROMO; piece++) {
					move = ENCODE_MOVE(sourceSquare, sourceSquare - 17, piece, m_board[sourceSquare - 17], 127);
					if (verifyMove(move)) m_generatedMoves[sourceSquare - 17] = move;
					
				}
			}
			else {
				move = ENCODE_MOVE(sourceSquare, sourceSquare - 17, CAPTURE, m_board[sourceSquare - 17], 127);
				if (verifyMove(move)) m_generatedMoves[sourceSquare - 17] = move;
				
			}
		}
	}

	if (!(sourceSquare - 15 & 0x88) && m_board[sourceSquare - 15] != 0) {
		// If piece to capture is not same colour as piece being moved
		if (EXTRACT_PIECE_COLOUR(m_board[sourceSquare - 15]) != m_colourToPlay) {

			// If target square is on back rank, generate promotion moves
			if ((sourceSquare - 15) / 16 == 0) {
				for (int piece = KNIGHT_CAPTURE_PROMO; piece <= QUEEN_CAPTURE_PROMO; piece++) {
					move = ENCODE_MOVE(sourceSquare, sourceSquare - 15, piece, m_board[sourceSquare - 15], 127);
					if (verifyMove(move)) m_generatedMoves[sourceSquare - 15] = move;
					
				}
			}
			else {
				move = ENCODE_MOVE(sourceSquare, sourceSquare - 15, CAPTURE, m_board[sourceSquare - 15], 127);
				if (verifyMove(move)) m_generatedMoves[sourceSquare - 15] = move;
				
			}
		}
	}

	// // Enpassant capture
	// If square to left is enpassant square
	if (m_enpassantSquare == sourceSquare - 1 && EXTRACT_PIECE_COLOUR(m_board[m_enpassantSquare]) != m_colourToPlay) {
		move = ENCODE_MOVE(sourceSquare, sourceSquare - 17, ENPASSANT_CAPTURE, PAWN + BLACK, 127);
		if (verifyMove(move)) m_generatedMoves[sourceSquare - 17] = move;
		
	}

	// If square to right is enpassant square
	if (m_enpassantSquare == sourceSquare + 1 && EXTRACT_PIECE_COLOUR(m_board[m_enpassantSquare]) != m_colourToPlay) {
		move = ENCODE_MOVE(sourceSquare, sourceSquare - 15, ENPASSANT_CAPTURE, PAWN + BLACK, 127);
		if (verifyMove(move)) m_generatedMoves[sourceSquare - 15] = move;
		
	}
}

inline void Chess::generatePawnBlack(int sourceSquare) {
	int move = 0;

	// // Regular piece move and pawn promotion
	// if square in front of pawn is empty
	if (m_board[sourceSquare + 16] == 0) {

		// If pawn on back rank, generate promotion moves
		if ((sourceSquare + 16) / 16 == 7) {
			for (int piece = KNIGHT_PROMO; piece <= QUEEN_PROMO; piece++) {
				move = ENCODE_MOVE(sourceSquare, sourceSquare + 16, piece, 0, 127);
				if (verifyMove(move)) m_generatedMoves[sourceSquare + 16] = move;
				
			}
		}
		else {
			move = ENCODE_MOVE(sourceSquare, sourceSquare + 16, QUIET_MOVE, 0, 127);
			if (verifyMove(move)) m_generatedMoves[sourceSquare + 16] = move;
			

			// If pawn is on starting square
			if (sourceSquare / 16 == 1) {

				// If square two squares away from pawn is empty
				if (m_board[sourceSquare + 32] == 0) {
					move = ENCODE_MOVE(sourceSquare, sourceSquare + 32, DOUBLE_PAWN_MOVE, 0, 127);
					if (verifyMove(move)) m_generatedMoves[sourceSquare + 32] = move;
					
				}
			}
		}
	}

	// // Diagonal capture and pawn promotion
	if (!(sourceSquare + 17 & 0x88) && m_board[sourceSquare + 17] != 0) {
		// If piece to capture is not same colour as piece being moved
		if (EXTRACT_PIECE_COLOUR(m_board[sourceSquare + 17]) != m_colourToPlay) {

			// If target square is on back rank, generate promotion moves
			if ((sourceSquare + 17) / 16 == 7) {
				for (int piece = KNIGHT_CAPTURE_PROMO; piece <= QUEEN_CAPTURE_PROMO; piece++) {
					move = ENCODE_MOVE(sourceSquare, sourceSquare + 17, piece, m_board[sourceSquare + 17], 127);
					if (verifyMove(move)) m_generatedMoves[sourceSquare + 17] = move;
					
				}
			}
			else {
				move = ENCODE_MOVE(sourceSquare, sourceSquare + 17, CAPTURE, m_board[sourceSquare + 17], 127);
				if (verifyMove(move)) m_generatedMoves[sourceSquare + 17] = move;
				
			}
		}
	}

	if (!(sourceSquare + 15 & 0x88) && m_board[sourceSquare + 15] != 0) {
		// If piece to capture is not same colour as piece being moved
		if (EXTRACT_PIECE_COLOUR(m_board[sourceSquare + 15]) != m_colourToPlay) {

			// If target square is on back rank, generate promotion moves
			if ((sourceSquare + 15) / 16 == 7) {
				for (int piece = KNIGHT_CAPTURE_PROMO; piece <= QUEEN_CAPTURE_PROMO; piece++) {
					move = ENCODE_MOVE(sourceSquare, sourceSquare + 15, piece, m_board[sourceSquare + 15], 127);
					if (verifyMove(move)) m_generatedMoves[sourceSquare + 15] = move;
					
				}
			}
			else {
				move = ENCODE_MOVE(sourceSquare, sourceSquare + 15, CAPTURE, m_board[sourceSquare + 15], 127);
				if (verifyMove(move)) m_generatedMoves[sourceSquare + 15] = move;
				
			}
		}
	}

	// // Enpassant capture
	// If square to left is enpassant square
	if (m_enpassantSquare == sourceSquare - 1 && EXTRACT_PIECE_COLOUR(m_board[m_enpassantSquare]) != m_colourToPlay) {
		move = ENCODE_MOVE(sourceSquare, sourceSquare + 15, ENPASSANT_CAPTURE, PAWN + WHITE, 127);
		if (verifyMove(move)) m_generatedMoves[sourceSquare + 15] = move;
		
	}

	// If square to right is enpassant square
	if (m_enpassantSquare == sourceSquare + 1 && EXTRACT_PIECE_COLOUR(m_board[m_enpassantSquare]) != m_colourToPlay) {
		move = ENCODE_MOVE(sourceSquare, sourceSquare + 17, ENPASSANT_CAPTURE, PAWN + WHITE, 127);
		if (verifyMove(move)) m_generatedMoves[sourceSquare + 17] = move;
		
	}
}

inline void Chess::generateKnight(int sourceSquare) {
	int move = 0;

	for (int offset : knightOffsetss) {
		int targetSquare = sourceSquare + offset;

		// If move is off the m_board then go to next offset
		if (targetSquare & 0x88) continue;

		// If there is a piece on target square
		if (m_board[targetSquare] != 0) {

			// If piece on the square is same colour
			if (EXTRACT_PIECE_COLOUR(m_board[targetSquare]) == m_colourToPlay) continue;

			move = ENCODE_MOVE(sourceSquare, targetSquare, CAPTURE, m_board[targetSquare], 127);
			if (verifyMove(move)) m_generatedMoves[targetSquare] = move;
				
			break;
		}
		move = ENCODE_MOVE(sourceSquare, targetSquare, QUIET_MOVE, 0, 127);
		if (verifyMove(move)) m_generatedMoves[targetSquare] = move;
		
	}
}

inline void Chess::generateDiagonal(int max, int sourceSquare) {
	int move = 0;

	int neg17DiagDistToEdge = std::min(sourceSquare / 16, sourceSquare % 16);
	int neg15DiagDistToEdge = std::min(sourceSquare / 16, 7 - sourceSquare % 16);
	int pos15DiagDistToEdge = std::min(7 - sourceSquare / 16, sourceSquare % 16);
	int pos17DiagDistToEdge = std::min(7 - sourceSquare / 16, 7 - sourceSquare % 16);

	for (int j = 1; j <= neg17DiagDistToEdge; j++) {
		int targetSquare = sourceSquare - 17 * j;

		// Check if there is piece on target square
		if (m_board[targetSquare] != 0) {

			// Check if target square contains piece of same colour
			if (EXTRACT_PIECE_COLOUR(m_board[targetSquare]) == m_colourToPlay) break;

			move = ENCODE_MOVE(sourceSquare, targetSquare, CAPTURE, m_board[targetSquare], 127);
			if (verifyMove(move)) m_generatedMoves[targetSquare] = move;

			break;
		}
		move = ENCODE_MOVE(sourceSquare, targetSquare, QUIET_MOVE, 0, 127);
		if (verifyMove(move)) m_generatedMoves[targetSquare] = move;
	}

	for (int j = 1; j <= neg15DiagDistToEdge; j++) {
		int targetSquare = sourceSquare - 15 * j;

		// Check if there is piece on target square
		if (m_board[targetSquare] != 0) {

			// Check if target square contains piece of same colour
			if (EXTRACT_PIECE_COLOUR(m_board[targetSquare]) == m_colourToPlay) break;

			move = ENCODE_MOVE(sourceSquare, targetSquare, CAPTURE, m_board[targetSquare], 127);
			if (verifyMove(move)) m_generatedMoves[targetSquare] = move;

			break;
		}
		move = ENCODE_MOVE(sourceSquare, targetSquare, QUIET_MOVE, 0, 127);
		if (verifyMove(move)) m_generatedMoves[targetSquare] = move;
	}

	for (int j = 1; j <= pos15DiagDistToEdge; j++) {
		int targetSquare = sourceSquare + 15 * j;

		// Check if there is piece on target square
		if (m_board[targetSquare] != 0) {

			// Check if target square contains piece of same colour
			if (EXTRACT_PIECE_COLOUR(m_board[targetSquare]) == m_colourToPlay) break;

			move = ENCODE_MOVE(sourceSquare, targetSquare, CAPTURE, m_board[targetSquare], 127);
			if (verifyMove(move)) m_generatedMoves[targetSquare] = move;

			break;
		}
		move = ENCODE_MOVE(sourceSquare, targetSquare, QUIET_MOVE, 0, 127);
		if (verifyMove(move)) m_generatedMoves[targetSquare] = move;
	}

	for (int j = 1; j <= pos17DiagDistToEdge; j++) {
		int targetSquare = sourceSquare + 17 * j;

		// Check if there is piece on target square
		if (m_board[targetSquare] != 0) {

			// Check if target square contains piece of same colour
			if (EXTRACT_PIECE_COLOUR(m_board[targetSquare]) == m_colourToPlay) break;

			move = ENCODE_MOVE(sourceSquare, targetSquare, CAPTURE, m_board[targetSquare], 127);
			if (verifyMove(move)) m_generatedMoves[targetSquare] = move;

			break;
		}
		move = ENCODE_MOVE(sourceSquare, targetSquare, QUIET_MOVE, 0, 127);
		if (verifyMove(move)) m_generatedMoves[targetSquare] = move;
	}
}

inline void Chess::generateSraight(int max, int sourceSquare) {
	int negVerticalDistToEdge = sourceSquare / 16;
	int posVerticalDistToEdge = 7 - sourceSquare / 16;
	int negHorizontalDistToEdge = sourceSquare % 16;
	int posHorizontalDistToEdge = 7 - sourceSquare % 16;

	int move = 0;

	// Vertical Negative
	for (int j = 1; j <= negVerticalDistToEdge; j++) {
		int targetSquare = sourceSquare + -16 * j;

		// Check if there is piece on target square
		if (m_board[targetSquare] != 0) {

			// Check if target square contains piece of same colour
			if (EXTRACT_PIECE_COLOUR(m_board[targetSquare]) == m_colourToPlay) break;

			move = ENCODE_MOVE(sourceSquare, targetSquare, CAPTURE, m_board[targetSquare], 127);
			if (verifyMove(move)) m_generatedMoves[targetSquare] = move;
				
			break;
		}
		move = ENCODE_MOVE(sourceSquare, targetSquare, QUIET_MOVE, 0, 127);
		if (verifyMove(move)) m_generatedMoves[targetSquare] = move;
	}

	// Prefetch squares into cache for horizontal moves
	_mm_prefetch((char*)(m_board.data() + sourceSquare + 1), _MM_HINT_T0);

	// Vertical Positive
	for (int j = 1; j <= posVerticalDistToEdge; j++) {
		int targetSquare = sourceSquare + 16 * j;

		// Check if there is piece on target square
		if (m_board[targetSquare] != 0) {

			// Check if target square contains piece of same colour
			if (EXTRACT_PIECE_COLOUR(m_board[targetSquare]) == m_colourToPlay) break;

			move = ENCODE_MOVE(sourceSquare, targetSquare, CAPTURE, m_board[targetSquare], 127);
			if (verifyMove(move)) m_generatedMoves[targetSquare] = move;

			break;
		}
		move = ENCODE_MOVE(sourceSquare, targetSquare, QUIET_MOVE, 0, 127);
		if (verifyMove(move)) m_generatedMoves[targetSquare] = move;
	}

	// Horizontal Negative
	for (int j = 1; j <= negHorizontalDistToEdge; j++) {
		int targetSquare = sourceSquare + -1 * j;

		// Check if there is piece on target square
		if (m_board[targetSquare] != 0) {

			// Check if target square contains piece of same colour
			if (EXTRACT_PIECE_COLOUR(m_board[targetSquare]) == m_colourToPlay) break;

			move = ENCODE_MOVE(sourceSquare, targetSquare, CAPTURE, m_board[targetSquare], 127);
			if (verifyMove(move)) m_generatedMoves[targetSquare] = move;

			break;
		}
		move = ENCODE_MOVE(sourceSquare, targetSquare, QUIET_MOVE, 0, 127);
		if (verifyMove(move)) m_generatedMoves[targetSquare] = move;
	}

	// Vertical Positive
	for (int j = 1; j <= posHorizontalDistToEdge; j++) {
		int targetSquare = sourceSquare + 1 * j;

		// Check if there is piece on target square
		if (m_board[targetSquare] != 0) {

			// Check if target square contains piece of same colour
			if (EXTRACT_PIECE_COLOUR(m_board[targetSquare]) == m_colourToPlay) break;

			move = ENCODE_MOVE(sourceSquare, targetSquare, CAPTURE, m_board[targetSquare], 127);
			if (verifyMove(move)) m_generatedMoves[targetSquare] = move;

			break;
		}
		move = ENCODE_MOVE(sourceSquare, targetSquare, QUIET_MOVE, 0, 127);
		if (verifyMove(move)) m_generatedMoves[targetSquare] = move;
	}

	
}

inline void Chess::generateKingWhite(int sourceSquare) {
	int move = 0;

	for (int i = 0; i < 8; i++) {
		int targetSquare = sourceSquare + directionalOffsetss[i];

		if (targetSquare & 0x88) continue;

		// Check if there is piece on target square
		if (m_board[targetSquare] != 0) {

			// Check if target square contains piece of same colour
			if (EXTRACT_PIECE_COLOUR(m_board[targetSquare]) == m_colourToPlay) continue;

			move = ENCODE_MOVE(sourceSquare, targetSquare, CAPTURE, m_board[targetSquare], 127);
			if (verifyMove(move)) m_generatedMoves[targetSquare] = move;

			continue;
		}
		move = ENCODE_MOVE(sourceSquare, targetSquare, QUIET_MOVE, 0, 127);
		if (verifyMove(move)) m_generatedMoves[targetSquare] = move;
	}

	// Kingside castling
	if (m_canWhiteKingsideCastle) {
		// Check if no pieces inbetween
		if (m_board[sourceSquare + 1] == 0 && m_board[sourceSquare + 2] == 0 && m_board[sourceSquare + 3] == WHITE + ROOK) {
			move = ENCODE_MOVE(sourceSquare, sourceSquare + 2, KING_CASTLE, 0, 127);
			if (verifyMove(move)) m_generatedMoves[sourceSquare + 2] = move;
			
		}
	}
	// Queenside castling
	if (m_canWhiteQueensideCastle) {
		if (m_board[sourceSquare - 1] == 0 && m_board[sourceSquare - 2] == 0 && m_board[sourceSquare - 3] == 0 && m_board[sourceSquare - 4] == WHITE + ROOK) {
			move = ENCODE_MOVE(sourceSquare, sourceSquare - 2, QUEEN_CASTLE, 0, 127);
			if (verifyMove(move)) m_generatedMoves[sourceSquare - 2] = move;
			
		}
	}
}

inline void Chess::generateKingBlack(int sourceSquare) {
	int move = 0;

	for (int i = 0; i < 8; i++) {
		int targetSquare = sourceSquare + directionalOffsetss[i];

		if (targetSquare & 0x88) continue;

		// Check if there is piece on target square
		if (m_board[targetSquare] != 0) {

			// Check if target square contains piece of same colour
			if (EXTRACT_PIECE_COLOUR(m_board[targetSquare]) == m_colourToPlay) continue;

			move = ENCODE_MOVE(sourceSquare, targetSquare, CAPTURE, m_board[targetSquare], 127);
			if (verifyMove(move)) m_generatedMoves[targetSquare] = move;

			continue;
		}
		move = ENCODE_MOVE(sourceSquare, targetSquare, QUIET_MOVE, 0, 127);
		if (verifyMove(move)) m_generatedMoves[targetSquare] = move;
	}

	// Kingside castling
	if (m_canBlackKingsideCastle) {
		// Check if no pieces inbetween
		if (m_board[sourceSquare + 1] == 0 && m_board[sourceSquare + 2] == 0 && m_board[sourceSquare + 3] == BLACK + ROOK) {
			move = ENCODE_MOVE(sourceSquare, sourceSquare + 2, KING_CASTLE, 0, 127);
			if (verifyMove(move)) m_generatedMoves[sourceSquare + 2] = move;
			
		}
	}
	// Queenside castling
	if (m_canBlackQueensideCastle) {
		if (m_board[sourceSquare - 1] == 0 && m_board[sourceSquare - 2] == 0 && m_board[sourceSquare - 3] == 0 && m_board[sourceSquare - 4] == BLACK + ROOK) {
			move = ENCODE_MOVE(sourceSquare, sourceSquare - 2, QUEEN_CASTLE, 0, 127);
			if (verifyMove(move)) m_generatedMoves[sourceSquare - 2] = move;
			
		}
	}
}