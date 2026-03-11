#include "Chess.h"


bool Chess::playMove(int moveData) {
	if (m_colourToPlay == m_engineColour) return false;

	int oldInfo = ENCODE_INFO(m_colourToPlay, (m_canWhiteQueensideCastle | (m_canWhiteKingsideCastle << 1) | (m_canBlackQueensideCastle << 2) | (m_canBlackKingsideCastle << 3)), m_enpassantSquare, m_whiteKingSquare, m_blackKingSquare);

	makeMove(moveData);

	// Change turn
	m_colourToPlay = SWITCH_COLOUR(m_colourToPlay);

	m_engine->playPlayerMove(moveData, oldInfo);

	return true;
}

void Chess::playEngineMove() {
	if (m_colourToPlay != m_engineColour) throw;

	int movePlayed = m_engine->playEngineMove();

	makeMove(movePlayed);

	m_movedPieceSource = EXTRACT_SOURCE_SQUARE(movePlayed);
	m_movedPieceTarget = EXTRACT_TARGET_SQUARE(movePlayed);

	m_colourToPlay = SWITCH_COLOUR(m_colourToPlay);
}