#pragma once

#include <SDL_events.h>
#include <string>
#include <array>
#include <random>

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
* Macros used to extract piece type and colour from piece ID
*/
#define EXTRACT_PIECE_TYPE(id) (id % 8)
#define EXTRACT_PIECE_COLOUR(id) (id / 16) // Returns piece colour as 1 or 0, black or white respectively

// Macro used for switching turn
#define SWITCH_COLOUR(colour) (colour % 2 + 1)

// Macros for encode move and extracting move info
#define ENCODE_MOVE(source, target, code, pieceCaptured, capturedLookup) ( source | (target << 7) | (code << 14) | (pieceCaptured << 18) | (capturedLookup << 24))

#define EXTRACT_SOURCE_SQUARE(move) (move & 0x7f)
#define EXTRACT_TARGET_SQUARE(move) ((move >> 7) & 0x7f)
#define EXTRACT_MOVE_CODE(move) ((move >> 14) & 0xf)
#define EXTRACT_PIECE_CAPTURED(move) ((move >> 18) & 0x3f)
#define EXTRACT_CAPTURED_LOOKUP(move) ((move >> 24) & 0x7f)

// Codes relevant to move data encoding and move data extraction macros
#define QUIET_MOVE 0
#define DOUBLE_PAWN_MOVE 1
#define KING_CASTLE 2
#define QUEEN_CASTLE 3
#define CAPTURE 4
#define ENPASSANT_CAPTURE 5
#define KNIGHT_PROMO 8
#define BISHOP_PROMO 9
#define ROOK_PROMO 10
#define QUEEN_PROMO 11
#define KNIGHT_CAPTURE_PROMO 12
#define BISHOP_CAPTURE_PROMO 13
#define ROOK_CAPTURE_PROMO 14
#define QUEEN_CAPTURE_PROMO 15

// Macros for encoding board info, updating board info and extracting board info
#define ENCODE_INFO(colourToPlay, castlingRights, enpassantSquare, whiteKingSquare, blackKingSquare) ((colourToPlay | (castlingRights << 2) | (enpassantSquare << 6) | (whiteKingSquare << 13) | (blackKingSquare << 20)))

#define EXTRACT_COLOUR_TO_PLAY(info) (info & 0x3)
#define EXTRACT_CASTLING_RIGHTS(info) ((info >> 2) & 0xf)
#define EXTRACT_ENPASSANT_SQUARE(info) ((info >> 6) & 0x7f)
#define EXTRACT_WHITE_KING_SQUARE(info) ((info >> 13) & 0x7f)
#define EXTRACT_BLACK_KING_SQUARE(info) ((info >> 20) & 0x7f)

#define UPDATE_COLOUR_TO_PLAY(info, colourToPlay) ((info & 0xffffffc) | (colourToPlay))
#define UPDATE_CASTLING_RIGHTS(info, castleRights) ((info & 0xfffffc3) | (castleRights << 2))
#define UPDATE_ENPASSANT_SQUARE(info, enpassantSquare) ((info & 0xfffe03f) | (enpassantSquare << 6))
#define UPDATE_WHITE_KING_SQUARE(info, whiteKingSquare) ((info & 0xff01fff) | (whiteKingSquare << 13))
#define UPDATE_BLACK_KING_SQUARE(info, blackKingSquare) ((info & 0x80fffff) | (blackKingSquare << 20))

// 1111 1111 1111 1111 1111 1111 1111
//  $$$ $$$$ |||| |||^ ^^^^ ^^## ##..


class ChessEngine {
public:
	
	ChessEngine(int depth, unsigned int seed, int engineColour);

	/*
	* Load position from fen
	*/
	void loadFromFEN(std::string FEN);

	/*
	*
	*/
	void initZobristHashArray();

	/*
	* Print board state
	*/
	void printBoard();


	/*
	* Generate possible pseudo legal moves for selected piece
	* POSSIBLE OPTIMISATION (plus readability improvements), change nested ifs to multiple conditions
	*/
	int generatePseudoLegalMoves(int* pseudoLegalMoves, int* piecePositions, int* board, int* lookup, char castlingRights, int enpassantSquare, int colourToPlay, int blackKingSquare, int whiteKingSquare);

	inline void generatePawnWhite(int* pseudoLegalMoves, int &index, int sourceSquare, int colourToPlay, int enpassantSquare, int* board, int* lookup);
	inline void generatePawnBlack(int* pseudoLegalMoves, int &index, int sourceSquare, int colourToPlay, int enpassantSquare, int* board, int* lookup);
	inline void generateKnight(int* pseudoLegalMoves, int &index, int sourceSquare, int colourToPlay, int* board, int* lookup);
	inline void generateDiagonal(int* pseudoLegalMoves, int &index, int max, int sourceSquare, int colourToPlay, int* board, int* lookup);
	inline void generateSraight(int* pseudoLegalMoves, int &index, int max, int sourceSquare, int colourToPlay, int* board, int* lookup);
	inline void generateKingWhite(int* pseudoLegalMoves, int &index, int sourceSquare, int* board, int* lookup, char castlingRights);
	inline void generateKingBlack(int* pseudoLegalMoves, int &index, int sourceSquare, int* board, int* lookup, char castlingRights);

	/*
	* Play move made by player
	* Return true if move iis legal, otherwise return false
	*/
	bool playPlayerMove(int encodedMove, int endcodedInfo);

	int makeMove(int encodedMove, int encodedInfo, int* board, int* piecePositions, int* lookupPiece);
	void unmakeMove(int encodedMove, int encodedInfo, int* board, int* piecePositions, int* lookupPiece);
	bool verifyMove(int encodedMove, int encodedInfo, int* board, int* piecePositions, int* lookupPiece);
	bool isKingInCheck(int* board, int kingSquare, int colourToPlay);
	
	
	/// <summary>
	/// Start perft function from outside caller
	/// </summary>
	void perft();

	/// <summary>
	/// Go through EVERY SINGLE POSITION reachable from current position and count stats like number of final positions reached, number of captures, enpassant etc.
	/// For debugging purposes
	/// </summary>
	/// <param name="depthLeft"> Number of depths left to searcvh</param>
	/// <param name="encodedInfo"> Info related to the board state</param>
	/// <param name="board"> The actual board </param>
	/// <param name="piecePositions"> The square occupied by ever piece on the board</param>
	/// <param name="lookupPiece"> The index of each piece in the piecePositions array </param>
	void perft(int depthLeft, int encodedInfo, int* board, int* piecePositions, int* lookupPiece);

	/// <summary>
	/// Getter function
	/// </summary>
	/// <returns> Return search cepth of engine</returns>
	int getDepth();

	/// <summary>
	/// Start alpha beta pruning from outside caller and pick correct alpha beta pruning method based off of engine colour. Black = min, White = max
	/// </summary>
	/// <returns> Move played by engine</returns>
	int playEngineMove();

	/// <summary>
	/// Alpha beta pruning MAX. Represents white
	/// </summary>
	/// <param name="alpha"> Alpha cutoff value </param>
	/// <param name="beta"> Beta cutoff value </param>
	/// <param name="depthLeft"> Number of depths left to search </param>
	/// <param name="encodedInfo"> Info related to the board state</param>
	/// <param name="board"> The actual board </param>
	/// <param name="piecePositions"> The square occupied by ever piece on the board</param>
	/// <param name="lookupPiece"> The index of each piece in the piecePositions array </param>
	/// <returns> </returns>
	int ABMax(int alpha, int beta, int depthLeft, int encodedInfo, int* board, int* piecePositions, int* lookupPiece);

	/// <summary>
	/// Alpha beta pruning MIN. Represents black
	/// </summary>
	/// <param name="alpha"> Alpha cutoff value </param>
	/// <param name="beta"> Beta cutoff value </param>
	/// <param name="depthLeft"> Number of depths left to search </param>
	/// <param name="encodedInfo"> Info related to the board state</param>
	/// <param name="board"> The actual board </param>
	/// <param name="piecePositions"> The square occupied by ever piece on the board</param>
	/// <param name="lookupPiece"> The index of each piece in the piecePositions array </param>
	/// <returns> </returns>
	int ABMin(int alpha, int beta, int depthLeft, int encodedInfo, int* board, int* piecePositions, int* lookupPiece);

	/// <summary>
	/// Evaluate the position reached
	/// </summary>
	/// <param name="encodedInfo"> Info related to the board state</param>
	/// <param name="board"> The actual board </param>
	/// <param name="piecePositions"> The square occupied by ever piece on the board</param>
	/// <returns> Returns score of the position </returns>
	int evaluate(int encodedInfo, int* board, int* piecePositions);

	/*
	* Move ordering
	*/
	void moveOrdering(int* pseudoLegalMoves, int moveNum);


private:
	// Engine colour
	int m_engineColour;
	
	// Depth
	int m_depth;

	// Next move encoded
	int m_nextEncodedMove;

	// Board state
	int m_board[128];

	// Piece locations
	int m_piecePositions[33];
	int m_lookupPiece[128];

	// // Board data
	// Castling rights  BLACK QUEEN, BLACK KING, WHITE QUEEN, WHITE KING (1111)
	char m_castlingRights;

	// Square of pawn that can be taken by enpassant
	int m_enpassantSquare;

	// Colour to play
	int m_colourToPlay;

	// Promo square
	int m_promoSquare;

	// King square
	int m_whiteKingSquare;
	int m_blackKingSquare;

	// Last piece taken
	int m_lastPieceTaken;

	// Half mvoes since last pawn move
	int m_halfMovesSincePawnMove;

	// Full moves since start of game
	int m_movesSinceStart;

	// ZOBRIST HASING STUFF
	// Zobrist hash array
	int m_zobristHashArray[12 * 64 + 1 + 4 + 8];
	
	// Random number seed function
	unsigned int m_randomSeed;

	// Pseudo-random number generator
	std::mt19937_64 m_PRNG_64;
};

