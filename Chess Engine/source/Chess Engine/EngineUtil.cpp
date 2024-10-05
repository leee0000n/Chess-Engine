#include "ChessEngine.h"

#include <random>

ChessEngine::ChessEngine(int depth, unsigned int seed, int engineColour) {
	m_depth = depth;
	m_randomSeed = seed;
	m_engineColour = engineColour;
}

std::string validateFENString(std::string FEN) {
	std::string startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	if (FEN == "startingposition") {
		FEN = startingFen;
		return FEN;
	}

	// Check if fen is valid
	int slashNum = 0;
	int squaresAccountedFor = 0;
	for (char chr : FEN) {
		switch (chr) {
		case '/':
			if (squaresAccountedFor < 8) return startingFen;
			slashNum++;

			//BLACK PIECES
		case 'p': // PAWN
			squaresAccountedFor++;
			break;
		case 'n': // KNIGHT
			squaresAccountedFor++;
			break;
		case 'b': // BISHOP
			squaresAccountedFor++;
			break;
		case 'r': // ROOK
			squaresAccountedFor++;
			break;
		case 'q': // QUEEN
			squaresAccountedFor++;
			break;
		case 'k': // KING
			squaresAccountedFor++;
			break;

			// WHITE PIECES
		case 'P': // PAWN
			squaresAccountedFor++;
			break;
		case 'N': // KNIGHT
			squaresAccountedFor++;
			break;
		case 'B': // BISHOP
			squaresAccountedFor++;
			break;
		case 'R': // ROOK
			squaresAccountedFor++;
			break;
		case 'Q': // QUEEN
			squaresAccountedFor++;
			break;
		case 'K': // KING
			squaresAccountedFor++;
			break;
		default: // Current character is a number of empty spaces
			squaresAccountedFor += chr - 48;
			break;
		}
	}

	if (slashNum < 7) return startingFen;
	return FEN;
}

void ChessEngine::loadFromFEN(std::string FEN) {
	FEN = validateFENString(FEN);

	memset(m_board, 0, sizeof m_board);

	int pieceSquare = 0;
	bool loop = true;

	int pIndexW = 1;
	int pIndexB = 17;
	int nIndexW = 9;
	int nIndexB = 25;
	int bIndexW = 11;
	int bIndexB = 27;
	int rIndexW = 13;
	int rIndexB = 29;
	int qIndexW = 15;
	int qIndexB = 31;
	int kIndexW = 16;
	int kIndexB = 32;

	int index = 0;
	while (true) {
		char piece = FEN[index];
		switch (piece) {

		case ' ': // END OF PIECE PLACEMENT
			loop = false;

			break;

		case '/': // NEXT RANK
			pieceSquare += 7;
			break;

			//BLACK PIECES
		case 'p': // PAWN
			m_board[pieceSquare] = BLACK + PAWN;
			m_piecePositions[pIndexB] = pieceSquare;
			m_lookupPiece[pieceSquare] = pIndexB;
			pIndexB++;
			break;
		case 'n': // KNIGHT
			m_board[pieceSquare] = BLACK + KNIGHT;
			m_piecePositions[nIndexB] = pieceSquare;
			m_lookupPiece[pieceSquare] = nIndexB;
			nIndexB++;

			break;
		case 'b': // BISHOP
			m_board[pieceSquare] = BLACK + BISHOP;
			m_piecePositions[bIndexB] = pieceSquare;
			m_lookupPiece[pieceSquare] = bIndexB;
			bIndexB++;
			break;
		case 'r': // ROOK
			m_board[pieceSquare] = BLACK + ROOK;
			m_piecePositions[rIndexB] = pieceSquare;
			m_lookupPiece[pieceSquare] = rIndexB;
			rIndexB++;
			break;
		case 'q': // QUEEN
			m_board[pieceSquare] = BLACK + QUEEN;
			m_piecePositions[qIndexB] = pieceSquare;
			m_lookupPiece[pieceSquare] = qIndexB;
			qIndexB++;
			break;
		case 'k': // KING
			m_board[pieceSquare] = BLACK + KING;
			m_piecePositions[kIndexB] = pieceSquare;
			m_lookupPiece[pieceSquare] = kIndexB;
			kIndexB++;
			m_blackKingSquare = pieceSquare;
			break;

			// WHITE PIECES
		case 'P': // PAWN
			m_board[pieceSquare] = WHITE + PAWN;
			m_piecePositions[pIndexW] = pieceSquare;
			m_lookupPiece[pieceSquare] = pIndexW;
			pIndexW++;
			break;
		case 'N': // KNIGHT
			m_board[pieceSquare] = WHITE + KNIGHT;
			m_piecePositions[nIndexW] = pieceSquare;
			m_lookupPiece[pieceSquare] = nIndexW;
			nIndexW++;
			break;
		case 'B': // BISHOP
			m_board[pieceSquare] = WHITE + BISHOP;
			m_piecePositions[bIndexW] = pieceSquare;
			m_lookupPiece[pieceSquare] = bIndexW;
			bIndexW++;
			break;
		case 'R': // ROOK
			m_board[pieceSquare] = WHITE + ROOK;
			m_piecePositions[rIndexW] = pieceSquare;
			m_lookupPiece[pieceSquare] = rIndexW;
			rIndexW++;
			break;
		case 'Q': // QUEEN
			m_board[pieceSquare] = WHITE + QUEEN;
			m_piecePositions[qIndexW] = pieceSquare;
			m_lookupPiece[pieceSquare] = qIndexW;
			qIndexW++;
			break;
		case 'K': // KING
			m_board[pieceSquare] = WHITE + KING;
			m_piecePositions[kIndexW] = pieceSquare;
			m_lookupPiece[pieceSquare] = kIndexW;
			kIndexW++;
			m_whiteKingSquare = pieceSquare;
			break;
		default: // Current character is a number of empty spaces
			pieceSquare += piece - 49; // Piece is a char so piece - 49 gives actual integer
			break;
		}
		// Goto next pieceSquareuare
		pieceSquare++;

		index++;
		if (!loop) break; // when first space is encountered, stop placing pieces
	}

	// Colour to play
	if (FEN[index] == 'w') {
		m_colourToPlay = 1;
	}
	else {
		m_colourToPlay = 2;
	}

	index += 2;

	int castlingRights = 0;
	int j = index;
	for (int i = j; i < j + 4; i++) {
		char letter = FEN[i];
		switch (letter) {
		case 'K':
			castlingRights |= 1;
			index++;
			break;
		case 'Q':
			castlingRights |= 2;
			index++;
			break;
		case 'k':
			castlingRights |= 4;
			index++;
			break;
		case 'q':
			castlingRights |= 8;
			index++;
			break;
		case '-':
			index++;
			break;
		case ' ':
			index++;
			i = 100;
			break;
		default:
			throw;
			break;
		}
	}

	m_castlingRights = castlingRights;

	// Enpassant square
	if (FEN[index] == '-') {
		m_enpassantSquare = 127;
		index += 2;
	}
	else {
		// Convert enpassant square from chess coords (e.g. b6) to board index
		int number = (8 - FEN[index + 1] + 49) * 16 + FEN[index] - 97;
		m_enpassantSquare = number;
		index += 3;
	}

	if (index >= FEN.size()) return;

	// Half moves since pawn move
	std::string number = "";
	for (int i = index; i < index + 3; i++) {
		char letter = FEN[i];
		if (letter == ' ') break;

		number += letter;
	}

	index += number.length() + 1;

	if (number.length() > 0 && number != "0") m_halfMovesSincePawnMove = std::stoi(number);
	else if (number == "0") m_halfMovesSincePawnMove = 0;

	if (index > FEN.size()) return;

	// Moves since start of game
	number = "";
	for (int i = index; i < FEN.length(); i++) {
		char letter = FEN[i];

		number += letter;
	}

	if (number.length() > 0 && number != "1") m_movesSinceStart = std::stoi(number);
	else if (number == "1") m_movesSinceStart = 1;
}

int ChessEngine::getDepth() {return m_depth;}
