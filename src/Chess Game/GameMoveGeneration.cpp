#include "Chess.h"

std::array<const int, 8> knightOffsetss = { -33, -31, // 2 rows above
										  -18, -14, // 1 row above
										  18, 14,   // 1 row below
										  33, 31 };  // 2 rows below
std::array<const int, 8> directionalOffsetss = { -16, -15, 1, 17, 16, 15, -1, -17 };

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
	// // Regular piece move and pawn promotion
	// if square in front of pawn is empty
	if (m_board[sourceSquare - 16] == 0) {

		// If pawn on back rank, generate promotion moves
		if ((sourceSquare - 16) / 16 == 0) {
			for (int piece = KNIGHT_PROMO; piece <= QUEEN_PROMO; piece++) {
				m_generatedMoves[sourceSquare - 16] = ENCODE_MOVE(sourceSquare, sourceSquare - 16, piece, 0, 127);
				
			}
		}
		else {
			m_generatedMoves[sourceSquare - 16] = ENCODE_MOVE(sourceSquare, sourceSquare - 16, QUIET_MOVE, 0, 127);
			

			// If pawn is on starting square
			if (sourceSquare / 16 == 6) {

				// If square two squares away from pawn is empty
				if (m_board[sourceSquare - 32] == 0) {
					m_generatedMoves[sourceSquare - 32] = ENCODE_MOVE(sourceSquare, sourceSquare - 32, DOUBLE_PAWN_MOVE, 0, 127);
					
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
					m_generatedMoves[sourceSquare - 17] = ENCODE_MOVE(sourceSquare, sourceSquare - 17, piece, m_board[sourceSquare - 17], 127);
					
				}
			}
			else {
				m_generatedMoves[sourceSquare - 17] = ENCODE_MOVE(sourceSquare, sourceSquare - 17, CAPTURE, m_board[sourceSquare - 17], 127);
				
			}
		}
	}

	if (!(sourceSquare - 15 & 0x88) && m_board[sourceSquare - 15] != 0) {
		// If piece to capture is not same colour as piece being moved
		if (EXTRACT_PIECE_COLOUR(m_board[sourceSquare - 15]) != m_colourToPlay) {

			// If target square is on back rank, generate promotion moves
			if ((sourceSquare - 15) / 16 == 0) {
				for (int piece = KNIGHT_CAPTURE_PROMO; piece <= QUEEN_CAPTURE_PROMO; piece++) {
					m_generatedMoves[sourceSquare - 15] = ENCODE_MOVE(sourceSquare, sourceSquare - 15, piece, m_board[sourceSquare - 15], 127);
					
				}
			}
			else {
				m_generatedMoves[sourceSquare - 15] = ENCODE_MOVE(sourceSquare, sourceSquare - 15, CAPTURE, m_board[sourceSquare - 15], 127);
				
			}
		}
	}

	// // Enpassant capture
	// If square to left is enpassant square
	if (m_enpassantSquare == sourceSquare - 1) {
		m_generatedMoves[sourceSquare - 17] = ENCODE_MOVE(sourceSquare, sourceSquare - 17, ENPASSANT_CAPTURE, PAWN + BLACK, 127);
		
	}

	// If square to right is enpassant square
	if (m_enpassantSquare == sourceSquare + 1) {
		m_generatedMoves[sourceSquare - 15] = ENCODE_MOVE(sourceSquare, sourceSquare - 15, ENPASSANT_CAPTURE, PAWN + BLACK, 127);
		
	}
}

inline void Chess::generatePawnBlack(int sourceSquare) {
	// // Regular piece move and pawn promotion
	// if square in front of pawn is empty
	if (m_board[sourceSquare + 16] == 0) {

		// If pawn on back rank, generate promotion moves
		if ((sourceSquare + 16) / 16 == 7) {
			for (int piece = KNIGHT_PROMO; piece <= QUEEN_PROMO; piece++) {
				m_generatedMoves[sourceSquare + 16] = ENCODE_MOVE(sourceSquare, sourceSquare + 16, piece, 0, 127);
				
			}
		}
		else {
			m_generatedMoves[sourceSquare + 16] = ENCODE_MOVE(sourceSquare, sourceSquare + 16, QUIET_MOVE, 0, 127);
			

			// If pawn is on starting square
			if (sourceSquare / 16 == 1) {

				// If square two squares away from pawn is empty
				if (m_board[sourceSquare + 32] == 0) {
					m_generatedMoves[sourceSquare + 32] = ENCODE_MOVE(sourceSquare, sourceSquare + 32, DOUBLE_PAWN_MOVE, 0, 127);
					
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
					m_generatedMoves[sourceSquare + 17] = ENCODE_MOVE(sourceSquare, sourceSquare + 17, piece, m_board[sourceSquare + 17], 127);
					
				}
			}
			else {
				m_generatedMoves[sourceSquare + 17] = ENCODE_MOVE(sourceSquare, sourceSquare + 17, CAPTURE, m_board[sourceSquare + 17], 127);
				
			}
		}
	}

	if (!(sourceSquare + 15 & 0x88) && m_board[sourceSquare + 15] != 0) {
		// If piece to capture is not same colour as piece being moved
		if (EXTRACT_PIECE_COLOUR(m_board[sourceSquare + 15]) != m_colourToPlay) {

			// If target square is on back rank, generate promotion moves
			if ((sourceSquare + 15) / 16 == 7) {
				for (int piece = KNIGHT_CAPTURE_PROMO; piece <= QUEEN_CAPTURE_PROMO; piece++) {
					m_generatedMoves[sourceSquare + 15] = ENCODE_MOVE(sourceSquare, sourceSquare + 15, piece, m_board[sourceSquare + 15], 127);
					
				}
			}
			else {
				m_generatedMoves[sourceSquare + 15] = ENCODE_MOVE(sourceSquare, sourceSquare + 15, CAPTURE, m_board[sourceSquare + 15], 127);
				
			}
		}
	}

	// // Enpassant capture
	// If square to left is enpassant square
	if (m_enpassantSquare == sourceSquare - 1) {
		m_generatedMoves[sourceSquare + 15] = ENCODE_MOVE(sourceSquare, sourceSquare + 15, ENPASSANT_CAPTURE, PAWN + WHITE, 127);
		
	}

	// If square to right is enpassant square
	if (m_enpassantSquare == sourceSquare + 1) {
		m_generatedMoves[sourceSquare + 17] = ENCODE_MOVE(sourceSquare, sourceSquare + 17, ENPASSANT_CAPTURE, PAWN + WHITE, 127);
		
	}
}

inline void Chess::generateKnight(int sourceSquare) {
	for (int offset : knightOffsetss) {
		int targetSquare = sourceSquare + offset;

		// If move is off the m_board then go to next offset
		if (targetSquare & 0x88) continue;

		// If there is a piece on target square
		if (m_board[targetSquare] != 0) {

			// If piece on the square is same colour
			if (EXTRACT_PIECE_COLOUR(m_board[targetSquare]) == m_colourToPlay) continue;

			m_generatedMoves[targetSquare] = ENCODE_MOVE(sourceSquare, targetSquare, CAPTURE, m_board[targetSquare], 127);
			
			continue;
		}
		// If move is not a capture
		m_generatedMoves[targetSquare] = ENCODE_MOVE(sourceSquare, targetSquare, QUIET_MOVE, 0, 127);
		
	}
}

inline void Chess::generateDiagonal(int max, int sourceSquare) {
	for (int i = 1; i < 8; i += 2) {
		for (int j = 1; j < max + 1; j++) {
			int targetSquare = sourceSquare + directionalOffsetss[i] * j;

			// Check if square is off m_board
			if (targetSquare & 0x88) break;

			// Check if there is piece on target square
			if (m_board[targetSquare] != 0) {

				// Check if target square contains piece of same colour
				if (EXTRACT_PIECE_COLOUR(m_board[targetSquare]) == m_colourToPlay) break;

				m_generatedMoves[targetSquare] = ENCODE_MOVE(sourceSquare, targetSquare, CAPTURE, m_board[targetSquare], 127);
				
				break;
			}
			m_generatedMoves[targetSquare] = ENCODE_MOVE(sourceSquare, targetSquare, QUIET_MOVE, 0, 127);
			
		}
	}
}

inline void Chess::generateSraight(int max, int sourceSquare) {
	for (int i = 0; i < 8; i += 2) {
		for (int j = 1; j < max + 1; j++) {
			int targetSquare = sourceSquare + directionalOffsetss[i] * j;

			// Check if square is off m_board
			if (targetSquare & 0x88) break;

			// Check if there is piece on target square
			if (m_board[targetSquare] != 0) {

				// Check if target square contains piece of same colour
				if (EXTRACT_PIECE_COLOUR(m_board[targetSquare]) == m_colourToPlay) break;

				m_generatedMoves[targetSquare] = ENCODE_MOVE(sourceSquare, targetSquare, CAPTURE, m_board[targetSquare], 127);
				
				break;
			}
			m_generatedMoves[targetSquare] = ENCODE_MOVE(sourceSquare, targetSquare, QUIET_MOVE, 0, 127);
			

		}
	}
}

inline void Chess::generateKingWhite(int sourceSquare) {
	// Generate normal moves
	generateDiagonal(1, sourceSquare);
	generateSraight(1, sourceSquare);

	// Kingside castling
	if (m_canWhiteKingsideCastle) {
		// Check if no pieces inbetween
		if (m_board[sourceSquare + 1] == 0 && m_board[sourceSquare + 2] == 0 && m_board[sourceSquare + 3] == WHITE + ROOK) {
			m_generatedMoves[sourceSquare + 2] = ENCODE_MOVE(sourceSquare, sourceSquare + 2, KING_CASTLE, 0, 127);
			
		}
	}
	// Queenside castling
	if (m_canWhiteQueensideCastle) {
		if (m_board[sourceSquare - 1] == 0 && m_board[sourceSquare - 2] == 0 && m_board[sourceSquare - 3] == 0 && m_board[sourceSquare - 4] == WHITE + ROOK) {
			m_generatedMoves[sourceSquare - 2] = ENCODE_MOVE(sourceSquare, sourceSquare - 2, QUEEN_CASTLE, 0, 127);
			
		}
	}
}

inline void Chess::generateKingBlack(int sourceSquare) {
	// Generate normal moves
	generateDiagonal(1, sourceSquare);
	generateSraight(1, sourceSquare);

	// Kingside castling
	if (m_canBlackKingsideCastle) {
		// Check if no pieces inbetween
		if (m_board[sourceSquare + 1] == 0 && m_board[sourceSquare + 2] == 0 && m_board[sourceSquare + 3] == BLACK + ROOK) {
			m_generatedMoves[sourceSquare + 2] = ENCODE_MOVE(sourceSquare, sourceSquare + 2, KING_CASTLE, 0, 127);
			
		}
	}
	// Queenside castling
	if (m_canBlackQueensideCastle) {
		if (m_board[sourceSquare - 1] == 0 && m_board[sourceSquare - 2] == 0 && m_board[sourceSquare - 3] == 0 && m_board[sourceSquare - 4] == BLACK + ROOK) {
			m_generatedMoves[sourceSquare - 2] = ENCODE_MOVE(sourceSquare, sourceSquare - 2, QUEEN_CASTLE, 0, 127);
			
		}
	}
}