#pragma once

#include "EventHandler.h"
#include "ChessEngine.h"

#include <SDL_events.h>
#include <string>
#include <array>

// Piece defines
#define WHITE 16
#define BLACK 32

#define WHITE_AS_1 1
#define BLACK_AS_2 2

#define EMPTY 0
#define PAWN 1
#define KNIGHT 2
#define BISHOP 3
#define ROOK 4
#define QUEEN 5
#define KING 6

/* 
* Relevant to piecePositions array
* If piece is captured, element in array will be -1
*/
#define CAPTURED -1

/*
* Macros used to extract piece type and colour from piece ID
*/

#define EXTRACT_PIECE_TYPE(id) (id % 8)
#define EXTRACT_PIECE_COLOUR(id) (id / 16) // Returns piece colour as 1 or 0, black or white respectively

// Macro used for switching turn
#define SWITCH_COLOUR(colour) (colour % 2 + 1)

/*
* Macros used to encode move data into one integer and extract move data from integer

	Move formatting:
	0000 0000 0000 0000 0111 1111  || source square 0x7f
	0000 0000 0011 1111 1000 0000  || target square
	0000 0011 1100 0000 0000 0000  || piece promotion
	0000 0100 0000 0000 0000 0000  || capture flag
	0000 1000 0000 0000 0000 0000  || double pawn move flag
	0001 0000 0000 0000 0000 0000  || enpassant flag
	0010 0000 0000 0000 0000 0000  || castling flag
*/
#define ENCODE_MOVE(source, target, code, pieceCaptured, capturedLookup) ( source | (target << 7) | (code << 14) | (pieceCaptured << 18) | (capturedLookup << 24))

#define EXTRACT_SOURCE_SQUARE(move) (move & 0x7f)
#define EXTRACT_TARGET_SQUARE(move) ((move >> 7) & 0x7f)
#define EXTRACT_MOVE_CODE(move) ((move >> 14) & 0xf)
#define EXTRACT_PIECE_CAPTURED(move) ((move >> 18) & 0x3f)
#define EXTRACT_CAPTURED_LOOKUP(move) ((move >> 24) & 0x7f)

// Flags relevant to move data encoding and move data extraction macros
#define CAPTURE_TRUE 1 
#define DOUBLE_PAWN_MOVE_TRUE 1
#define ENPASSANT_TRUE 1
#define CASTLING_TRUE 1

class Chess : public EventHandler
{
public:
	
	/*
	* Call chess engine constructor
	*/
	void initEngine(int depth, int seed, int colourToPlay, std::string FEN);

	/*
	* Initialise board state from fen. Initialise board in starting position if invald fen passed
	*/
	void loadFromFEN(std::string FEN);
	
	/*
	* Handle events from even handler
	*/
	void handleEvent(SDL_Event &e);

	/*
	* Render board squares and piece move indicator
	*/
	void renderBoard();

	/*
	* Render pieces
	*/
	void renderPieces();

	/*
	* Render possible moves
	*/
	void renderPossibleMoves();

	/*
	* Generate possible psuedo legal moves for selected piece
	*/
	void generatePseudoLegalMoves();

	inline void generatePawnWhite(int sourceSquare);
	inline void generatePawnBlack(int sourceSquare);
	inline void generateKnight(int sourceSquare);
	inline void generateDiagonal(int max, int sourceSquare);
	inline void generateSraight(int max, int sourceSquare);
	inline void generateKingWhite(int sourceSquare);
	inline void generateKingBlack(int sourceSquare);

	/*
	* Verify that move is legal
	* Return true if legal, false if not legal
	*/
	bool verifyMove(int moveData);

	/*
	* Make move
	*/
	int makeMove(int moveData);

	/*
	* Unmake move
	*/
	void unmakeMove(int moveData);

	/*
	* Play move and update board data
	*/
	bool playMove(int moveData);

	/*
	* Play engine move
	*/
	void playEngineMove();

	/*
	* Get engine colour
	*/
	int getEngineColour();

	/*
	* Get colour to play
	*/
	int getColourToPlay();

	/*
	* Return true or false whether it is or isn't engine's turn to play
	*/
	bool isEnginesTurnToPlay();


private:
	ChessEngine* m_engine;
	int m_engineColour;
	
	// Used to render piece move indicators
	int m_movedPieceSource = 0;
	int m_movedPieceTarget = 0;

	// Square that has been selected
	int m_selectedSquare = -1;

	// Possible move for selected piece
	std::array<int, 218> m_generatedMoves;

	// Board state
	std::array<int, 128> m_board;

	// // Board data
	// Castling rights
	bool m_canWhiteKingsideCastle = true;
	bool m_canWhiteQueensideCastle = true;
	bool m_canBlackKingsideCastle = true;
	bool m_canBlackQueensideCastle = true;

	// Square of pawn that can be taken by enpassant
	int m_enpassantSquare = -1;

	// Colour to play
	int m_colourToPlay = 1;

	// Promo square
	int m_promoSquare = -1;

	// King square
	int m_whiteKingSquare;
	int m_blackKingSquare;

	// Last piece taken
	int m_lastPieceTaken = 0;

	int m_halfMovesSincePawnMove;

	int m_movesSinceStart;
};
