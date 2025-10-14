#include "ChessEngine.h"
#include "LTimer.h"

#include <iostream>

std::array<std::string, 128> squareToCoords = { "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", "0", "0", "0", "0", "0", "0", "0", "0",
									 "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", "0", "0", "0", "0", "0", "0", "0", "0",
									 "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", "0", "0", "0", "0", "0", "0", "0", "0",
									 "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "0", "0", "0", "0", "0", "0", "0", "0",
									 "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", "0", "0", "0", "0", "0", "0", "0", "0",
									 "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", "0", "0", "0", "0", "0", "0", "0", "0",
									 "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", "0", "0", "0", "0", "0", "0", "0", "0",
									 "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "0", "0", "0", "0", "0", "0", "0", "0" };

std::array<char, 13> pieceCodeToFenLetter = {'.', 'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};

unsigned int finalPosNum = 0;
unsigned int totalFinalPosNum = 0;

unsigned int captureNum = 0;
unsigned int epNum = 0;
unsigned int castleNum = 0;
unsigned int promoNum = 0;

void ChessEngine::perft() {
	int encodedInfo = ENCODE_INFO(m_colourToPlay, m_castlingRights, m_enpassantSquare, m_whiteKingSquare, m_blackKingSquare);
	LTimer timer;
	timer.start();

	perft(m_depth, encodedInfo, m_board, m_piecePositions, m_lookupPiece);

	int time = timer.getTicks() / 1000.0;

	std::cout << "Time taken: " << time << "\n";
	std::cout << "Final positions per second: " << (time > 0 ? totalFinalPosNum / time : -1) << "\n";
	std::cout << "Total final positions: " << totalFinalPosNum << "\n";
	std::cout << "Capture num: " << captureNum << "\n";
	std::cout << "Enpassant capture num: " << epNum << "\n";
	std::cout << "Castle num: " << castleNum << "\n";
	std::cout << "Promotion num: " << promoNum << "\n";

}

void ChessEngine::perft(int depthLeft, int encodedInfo, int* board, int* piecePositions, int* lookupPiece) {
	// Recursion stop condition
	if (depthLeft == 0) {
		finalPosNum++;
		return;
	}

	int pseudoLegalMoves[218];

	// Generate all pseudo legal moves for current position
	int moveNum = generatePseudoLegalMoves(pseudoLegalMoves, piecePositions, board, lookupPiece, EXTRACT_CASTLING_RIGHTS(encodedInfo), EXTRACT_ENPASSANT_SQUARE(encodedInfo), EXTRACT_COLOUR_TO_PLAY(encodedInfo), EXTRACT_BLACK_KING_SQUARE(encodedInfo), EXTRACT_WHITE_KING_SQUARE(encodedInfo));


	for (int i = 0; i < moveNum; i++) {
		int encodedMove = pseudoLegalMoves[i];

		//std::string moveDebug = squareToCoords[EXTRACT_SOURCE_SQUARE(encodedMove)] + squareToCoords[EXTRACT_TARGET_SQUARE(encodedMove)] + ": " + pieceCodeToFenLetter[(EXTRACT_PIECE_TYPE((EXTRACT_PIECE_COLOUR(m_board[EXTRACT_SOURCE_SQUARE(encodedMove)]) - 1) < 0 ? 0 : m_board[EXTRACT_SOURCE_SQUARE(encodedMove)]) + 6 * (EXTRACT_PIECE_COLOUR(m_board[EXTRACT_SOURCE_SQUARE(encodedMove)]) - 1))];
		
		int newInfo = makeMove(encodedMove, encodedInfo, board, piecePositions, lookupPiece);
		//m_moveSequeneceDebug += squareToCoords[EXTRACT_SOURCE_SQUARE(encodedMove)] + squareToCoords[EXTRACT_TARGET_SQUARE(encodedMove)] + " ";
		if (verifyMove(encodedMove, newInfo, board, piecePositions, lookupPiece)) { // Verify move problem, castling possible even if king in check /////////////////////
			perft(depthLeft - 1, newInfo, board, piecePositions, lookupPiece);
			switch (EXTRACT_MOVE_CODE(encodedMove)) {
				case CAPTURE:
					captureNum++;
					break;
				case ENPASSANT_CAPTURE:
					epNum++;
					break;
				case KING_CASTLE:
					castleNum++;
					break;
				case QUEEN_CASTLE:
					castleNum++;
					break;
				case KNIGHT_CAPTURE_PROMO:
					promoNum++;
					break;
				case BISHOP_CAPTURE_PROMO:
					promoNum++;
					break;
				case ROOK_CAPTURE_PROMO:
					promoNum++;
					break;
				case QUEEN_CAPTURE_PROMO:
					promoNum++;
					break;

			}
		}

		if (depthLeft == m_depth) {
			std::cout << squareToCoords[EXTRACT_SOURCE_SQUARE(encodedMove)] << squareToCoords[EXTRACT_TARGET_SQUARE(encodedMove)];

			switch (EXTRACT_MOVE_CODE(encodedMove)) {
			case KNIGHT_PROMO:
				std::cout << "n";
				break;
			case KNIGHT_CAPTURE_PROMO:
				std::cout << "n";
				break;
			case BISHOP_PROMO:
				std::cout << "b";
				break;
			case BISHOP_CAPTURE_PROMO:
				std::cout << "b";
				break;
			case ROOK_PROMO:
				std::cout << "r";
				break;
			case ROOK_CAPTURE_PROMO:
				std::cout << "r";
				break;
			case QUEEN_PROMO:
				std::cout << "q";
				break;
			case QUEEN_CAPTURE_PROMO:
				std::cout << "q";
				break;

			default:
				break;
			}

			std::cout << ": " << finalPosNum << "\n";
			totalFinalPosNum += finalPosNum;
			finalPosNum = 0;
		}

		unmakeMove(encodedMove, encodedInfo, board, piecePositions, lookupPiece);
		/*if (m_moveSequeneceDebug.length() > 0) {
			for (int i = 0; i < 5; i++) {
				m_moveSequeneceDebug.pop_back();
			}
		}*/
		
	}
}

void ChessEngine::printBoard() {
	for (int i = 0; i < 120; i++) {
		if (i & 0x88) {
			std::cout << "\n";
			i += 8;
		}

		int piece = EXTRACT_PIECE_TYPE(m_board[i]);
		if (piece != 0) {
			int pieceColour = EXTRACT_PIECE_COLOUR(m_board[i]);

			std::cout << pieceCodeToFenLetter[piece + 6 * (pieceColour - 1)] << " ";
			continue;
		}

		std::cout << ". ";
		
	}

	std::cout << std::endl;
}