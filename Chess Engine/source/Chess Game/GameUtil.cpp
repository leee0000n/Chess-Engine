#include "Chess.h"

#include <iostream>

std::string validateFENStrings(std::string FEN) {
	std::string startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	if (FEN == "startingposition") {
		FEN = startingFen;
		return FEN;
	}

	int kingNum = 0;

	// Check if fen is valid
	int slashNum = 0;
	int squaresAccountedFor = 0;

	bool loop = true;
	for (char chr : FEN) {
		switch (chr) {
		case '/':
			if (squaresAccountedFor < 8) return startingFen;
			slashNum++;

			break;
		case ' ':
			loop = false;
			break;

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
			kingNum++;
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
			kingNum++;
			break;
		default: // Current character is a number of empty spaces
			squaresAccountedFor += chr - 48;
			break;
		}

		if (!loop) break; // when first space is encountered, stop placing pieces
	}
	if (kingNum != 2) return startingFen;
	else if (slashNum < 7) return startingFen;
	return FEN;
}

void Chess::loadFromFEN(std::string FEN) {
	FEN = validateFENStrings(FEN);

	m_board.fill(0);

	int pieceSquare = 0;
	bool loop = true;


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
			break;
		case 'n': // KNIGHT
			m_board[pieceSquare] = BLACK + KNIGHT;

			break;
		case 'b': // BISHOP
			m_board[pieceSquare] = BLACK + BISHOP;
			break;
		case 'r': // ROOK
			m_board[pieceSquare] = BLACK + ROOK;
			break;
		case 'q': // QUEEN
			m_board[pieceSquare] = BLACK + QUEEN;
			break;
		case 'k': // KING
			m_board[pieceSquare] = BLACK + KING;
			m_blackKingSquare = pieceSquare;

			break;

			// WHITE PIECES
		case 'P': // PAWN
			m_board[pieceSquare] = WHITE + PAWN;
			break;
		case 'N': // KNIGHT
			m_board[pieceSquare] = WHITE + KNIGHT;
			break;
		case 'B': // BISHOP
			m_board[pieceSquare] = WHITE + BISHOP;
			break;
		case 'R': // ROOK
			m_board[pieceSquare] = WHITE + ROOK;
			break;
		case 'Q': // QUEEN
			m_board[pieceSquare] = WHITE + QUEEN;
			break;
		case 'K': // KING
			m_board[pieceSquare] = WHITE + KING;
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

	m_canWhiteKingsideCastle = false;
	m_canWhiteQueensideCastle = false;
	m_canBlackKingsideCastle = false;
	m_canBlackQueensideCastle = false;

	int j = index;
	for (int i = j; i < j + 4; i++) {
		char letter = FEN[i];
		switch (letter) {
		case 'K':
			m_canWhiteKingsideCastle = true;
			index++;
			break;
		case 'Q':
			m_canWhiteQueensideCastle = true;
			index++;
			break;
		case 'k':
			m_canBlackKingsideCastle = true;
			index++;
			break;
		case 'q':
			m_canBlackQueensideCastle = true;
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

void Chess::initEngine(int depth, int seed, int engineColour, std::string FEN) {
	m_engine = new ChessEngine(depth, seed, engineColour);
	m_engine->loadFromFEN(FEN);
	m_engineColour = engineColour;
}

void Chess::handleEvent(SDL_Event& e) {
	if (e.type == SDL_MOUSEBUTTONDOWN) {

		if (m_promoSquare != -1) return;

		int x, y;
		SDL_GetMouseState(&x, &y);

		int square = x / 75 + y / 75 * 16; // Calculate index of square that was clicked on

		// If a piece has been selected
		if (m_selectedSquare != -1) {

			/*
			* This if statement makes piece selection less clunky, so you can select one piece and the select
			*  another one directly after with one mouse click
			*/
			// If same coloured piece is selected
			if (EXTRACT_PIECE_COLOUR(m_board[square]) == m_colourToPlay) {
				// Generate pseudo legal moves for it
				m_selectedSquare = square;
				generatePseudoLegalMoves();
				return;
			}

			if (m_generatedMoves[square] != 0) { // If move is legal
				int moveData = m_generatedMoves[square];

				if (playMove(moveData)) {
					m_movedPieceSource = m_selectedSquare;
					m_movedPieceTarget = square;

				}

			}

			m_generatedMoves.fill(0);
			m_selectedSquare = -1; // Unselect piece regardless of whether move is legal or not
		}
		else {
			if (m_board[square] != 0) { // If a piece is selected
				m_selectedSquare = square; // Change selectedPieceSquare to square of selected piece
				generatePseudoLegalMoves();
			}
		}
	}
	else if (e.type == SDL_KEYDOWN) {

		if (m_promoSquare == -1) return;

		switch (e.key.keysym.sym) {
			// Knight under promotion
		case SDLK_n:
			m_board[m_promoSquare] = SWITCH_COLOUR(m_colourToPlay) * 16 + KNIGHT;
			m_promoSquare = -1;
			break;
		case SDLK_b:
			m_board[m_promoSquare] = SWITCH_COLOUR(m_colourToPlay) * 16 + BISHOP;
			m_promoSquare = -1;
			break;
		case SDLK_r:
			m_board[m_promoSquare] = SWITCH_COLOUR(m_colourToPlay) * 16 + ROOK;
			m_promoSquare = -1;
			break;
		case SDLK_q:
			m_board[m_promoSquare] = SWITCH_COLOUR(m_colourToPlay) * 16 + QUEEN;
			m_promoSquare = -1;
			break;
		default:
			// Unneeded key pressed
			break;
		}
	}
}

int Chess::getColourToPlay() { return m_colourToPlay; }

int Chess::getEngineColour() { return m_engineColour; }

bool Chess::isEnginesTurnToPlay() { return m_engineColour == m_colourToPlay; }