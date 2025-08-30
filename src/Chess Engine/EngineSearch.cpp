#include "ChessEngine.h"

#include <iostream>


int ChessEngine::playEngineMove() {

    m_nextEncodedMove = 0;

    int encodedInfo = ENCODE_INFO(m_colourToPlay, m_castlingRights, m_enpassantSquare, m_whiteKingSquare, m_blackKingSquare);
    int score;
    if (m_engineColour == WHITE_AS_1) {
        score = ABMax(-INT_MAX, INT_MAX, m_depth, encodedInfo, m_board, m_piecePositions, m_lookupPiece);
    }
    else if (m_engineColour == BLACK_AS_2) {
        score = ABMin(-INT_MAX, INT_MAX, m_depth, encodedInfo, m_board, m_piecePositions, m_lookupPiece);
    }

    // Black wins
    if (score == -INT_MAX + 2) {
        std::cout << "Black Wins" << std::endl;
    }
    // White wins
    else if (score == INT_MAX - 2) {
        std::cout << "White Wins" << std::endl;
    }

    // There was an error
    else if (m_nextEncodedMove <= 0) throw;

    int newInfo = makeMove(m_nextEncodedMove, encodedInfo, m_board, m_piecePositions, m_lookupPiece);

    m_castlingRights = EXTRACT_CASTLING_RIGHTS(newInfo);
    m_enpassantSquare = EXTRACT_ENPASSANT_SQUARE(newInfo);
    m_whiteKingSquare = EXTRACT_WHITE_KING_SQUARE(newInfo);
    m_blackKingSquare = EXTRACT_BLACK_KING_SQUARE(newInfo);

    m_colourToPlay = SWITCH_COLOUR(m_colourToPlay);

    return m_nextEncodedMove;
}

int ChessEngine::ABMax(int alpha, int beta, int depthLeft, int encodedInfo, int* board, int* piecePositions, int* lookupPiece) {
    if (depthLeft == 0) return evaluate(encodedInfo, board, piecePositions);

    // MinMax values
    int bestValue = -INT_MAX;
    int score;

    // Move generation
    int pseudoLegalMoves[218];
    int moveNum = generatePseudoLegalMoves(pseudoLegalMoves, piecePositions, board, lookupPiece, EXTRACT_CASTLING_RIGHTS(encodedInfo), EXTRACT_ENPASSANT_SQUARE(encodedInfo), EXTRACT_COLOUR_TO_PLAY(encodedInfo), EXTRACT_BLACK_KING_SQUARE(encodedInfo), EXTRACT_WHITE_KING_SQUARE(encodedInfo));

    int isTherePlayableMove = false;

    for (int i = 0; i < moveNum; i++) {
        int encodedMove = pseudoLegalMoves[i];

        if (EXTRACT_SOURCE_SQUARE(encodedMove) == 80 && EXTRACT_TARGET_SQUARE(encodedMove) == 112) {
            encodedInfo = encodedInfo;
        }

		int newInfo = makeMove(encodedMove, encodedInfo, board, piecePositions, lookupPiece);
		if (verifyMove(encodedMove, newInfo, board, piecePositions, lookupPiece)) {
            isTherePlayableMove = true;

            score = ABMin(alpha, beta, depthLeft - 1, newInfo, board, piecePositions, lookupPiece); // Next depth

            // Prune
            if (score > bestValue) {
                bestValue = score;
                if (m_engineColour == WHITE_AS_1 && depthLeft == m_depth) m_nextEncodedMove = encodedMove;
                if (score > alpha) {
                    alpha = score;
                    
                }
            }
            else if (score >= beta) {
                unmakeMove(encodedMove, encodedInfo, board, piecePositions, lookupPiece);
                return score;
            }
            
        }

		unmakeMove(encodedMove, encodedInfo, board, piecePositions, lookupPiece);
    }

    // Check for checkmate or draw
    if (!isTherePlayableMove) {
        if (isKingInCheck(board, EXTRACT_BLACK_KING_SQUARE(encodedInfo), BLACK_AS_2)) {
            return -INT_MAX + 2;
        }
        else return 0;
    }

    return bestValue;
}


int ChessEngine::ABMin(int alpha, int beta, int depthLeft, int encodedInfo, int* board, int* piecePositions, int* lookupPiece) {
    if (depthLeft == 0) return evaluate(encodedInfo, board, piecePositions);

    // MinMax values
    int bestValue = INT_MAX;
    int score;

    // Move generation
    int pseudoLegalMoves[218];
    int moveNum = generatePseudoLegalMoves(pseudoLegalMoves, piecePositions, board, lookupPiece, EXTRACT_CASTLING_RIGHTS(encodedInfo), EXTRACT_ENPASSANT_SQUARE(encodedInfo), EXTRACT_COLOUR_TO_PLAY(encodedInfo), EXTRACT_BLACK_KING_SQUARE(encodedInfo), EXTRACT_WHITE_KING_SQUARE(encodedInfo));

    bool isTherePlayableMove = false;

    for (int i = 0; i < moveNum; i++) {
        int encodedMove = pseudoLegalMoves[i];

        if (EXTRACT_SOURCE_SQUARE(encodedMove) == 69 && EXTRACT_TARGET_SQUARE(encodedMove) == 103) {
           encodedInfo = encodedInfo;
        }

        int newInfo = makeMove(encodedMove, encodedInfo, board, piecePositions, lookupPiece);
        if (verifyMove(encodedMove, newInfo, board, piecePositions, lookupPiece)) {
           isTherePlayableMove = true; // To check if in checkmate or draw

           score = ABMax(alpha, beta, depthLeft - 1, newInfo, board, piecePositions, lookupPiece); // Next depth

           // Prune
           if (score < bestValue) {
                bestValue = score;
                if (m_engineColour == BLACK_AS_2 && depthLeft == m_depth) m_nextEncodedMove = encodedMove;
                if (score < beta) {
                    beta = score;
                    
                }
            }
            else if (score <= alpha) {
                unmakeMove(encodedMove, encodedInfo, board, piecePositions, lookupPiece);
                return score;
            }
        }

        unmakeMove(encodedMove, encodedInfo, board, piecePositions, lookupPiece);
    }
    
    // Check for checkmate or draw
    if (!isTherePlayableMove) {
        if (isKingInCheck(board, EXTRACT_BLACK_KING_SQUARE(encodedInfo), BLACK_AS_2)) {
            return INT_MAX - 2;
        }
        else return 0;
    }

    return bestValue;
}

void ChessEngine::moveOrdering(int* pseudoLegalMoves, int moveNum) {
    
}
