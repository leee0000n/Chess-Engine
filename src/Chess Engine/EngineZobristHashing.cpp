#include "ChessEngine.h"

void ChessEngine::initZobristHashArray() {
	m_PRNG_64.seed(m_randomSeed);

	// Zobrist hash for each piece on each square
	for (int i = PAWN - 1; i < KING; i++) {
		for (int j = 0; j < 64; j++) {
			m_zobristHashArray[i * 64 + j] = m_PRNG_64();
		}
	}

	int index = 12 * 64;

	// Pseudo-random for colour to play being black
	m_zobristHashArray[index++] = m_PRNG_64();

	// white king side castle
	m_zobristHashArray[index++] = m_PRNG_64();

	// white queen side castle
	m_zobristHashArray[index++] = m_PRNG_64();

	// black king side castle
	m_zobristHashArray[index++] = m_PRNG_64();

	// black queen side castle
	m_zobristHashArray[index++] = m_PRNG_64();

	// files a to h for enpassant square
	for (int i = index; i < index + 8; i++) {
		m_zobristHashArray[i] = m_PRNG_64();
	}
}

