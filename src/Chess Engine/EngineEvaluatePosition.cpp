#include "ChessEngine.h"

int nightoffsets[8] = { -33, -31, // 2 rows above
                                          -18, -14, // 1 row above
                                          18, 14,   // 1 row below
                                          33, 31 };  // 2 rows below
int directionaloffsetsS[8] = { -16, -15, 1, 17, 16, 15, -1, -17 };

// White Pawn-Square Table
const int whitePawnTable[128] = { 0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0,
                            50, 50, 50, 50, 50, 50, 50, 50, 0, 0, 0, 0, 0, 0, 0, 0,
                            10, 10, 20, 30, 30, 20, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0,
                            5,  5, 10, 25, 25, 10,  5,  5, 0, 0, 0, 0, 0, 0, 0, 0,
                            0,  0,  0, 20, 20,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0,
                            5, -5,-10,  0,  0,-10, -5,  5, 0, 0, 0, 0, 0, 0, 0, 0,
                            5, 10, 10,-20,-20, 10, 10,  5, 0, 0, 0, 0, 0, 0, 0, 0,
                            0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0 };

// Black Pawn-Square Table
const int blackPawnTable[128] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            -5, -10, -10, 20, 20, -10, -10, -5, 0, 0, 0, 0, 0, 0, 0, 0,
                            -5, 5, 10, 25, 25, -10, -5, -5, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, -20, -20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            -10, -10, -20, -30, -30, -20, -10, -10, 0, 0, 0, 0, 0, 0, 0, 0,
                            -50, -50, -50, -50, -50, -50, -50, -50, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// Knight Square Table
const int knightTable[128] = { -50, -40, -30, -30, -30, -30, -40, -50, 0, 0, 0, 0, 0, 0, 0, 0,
                        -40, -20,  0,  0,  0,  0, -20, -40, 0, 0, 0, 0, 0, 0, 0, 0,
                        -30,  0, 10, 15, 15, 10,  0, -30, 0, 0, 0, 0, 0, 0, 0, 0,
                        -30,  5, 15, 20, 20, 15,  5, -30, 0, 0, 0, 0, 0, 0, 0, 0,
                        -30,  0, 15, 20, 20, 15,  0, -30, 0, 0, 0, 0, 0, 0, 0, 0,
                        -30,  5, 10, 15, 15, 10,  5, -30, 0, 0, 0, 0, 0, 0, 0, 0,
                        -40, -20,  0,  5,  5,  0, -20, -40, 0, 0, 0, 0, 0, 0, 0, 0,
                        -50, -40, -30, -30, -30, -30, -40, -50, 0, 0, 0, 0, 0, 0, 0, 0 };

// Bishop-Square Table
const int bishopTable[128] = { -20, -10, -10, -10, -10, -10, -10, -20, 0, 0, 0, 0, 0, 0, 0, 0,
                        -10,  0,  0,  0,  0,  0,  0, -10, 0, 0, 0, 0, 0, 0, 0, 0,
                        -10,  0,  5, 10, 10,  5,  0, -10, 0, 0, 0, 0, 0, 0, 0, 0,
                        -10,  5,  5, 10, 10,  5,  5, -10, 0, 0, 0, 0, 0, 0, 0, 0,
                        -10,  0, 10, 10, 10, 10,  0, -10, 0, 0, 0, 0, 0, 0, 0, 0,
                        -10, 10, 10, 10, 10, 10, 10, -10, 0, 0, 0, 0, 0, 0, 0, 0,
                        -10,  5,  0,  0,  0,  0,  5, -10, 0, 0, 0, 0, 0, 0, 0, 0,
                        -20, -10, -10, -10, -10, -10, -10, -20, 0, 0, 0, 0, 0, 0, 0, 0 };

// Rook-Square Table
const int rookTable[128] = { 0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0,
                        5, 10, 10, 10, 10, 10, 10,  5, 0, 0, 0, 0, 0, 0, 0, 0,
                        -5,  0,  0,  0,  0,  0,  0, -5, 0, 0, 0, 0, 0, 0, 0, 0,
                        -5,  0,  0,  0,  0,  0,  0, -5, 0, 0, 0, 0, 0, 0, 0, 0,
                        -5,  0,  0,  0,  0,  0,  0, -5, 0, 0, 0, 0, 0, 0, 0, 0,
                        -5,  0,  0,  0,  0,  0,  0, -5, 0, 0, 0, 0, 0, 0, 0, 0,
                        -5,  0,  0,  0,  0,  0,  0, -5, 0, 0, 0, 0, 0, 0, 0, 0,
                        0,  0,  0,  5,  5,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0 };

// Queen-Square Table
const int queenTable[128] = {
    -20, -10, -10, -5,  -5, -10, -10, -20, 0, 0, 0, 0, 0, 0, 0, 0,
    -10,  0,   0,  0,   0,  0,  0, -10, 0, 0, 0, 0, 0, 0, 0, 0,
    -10,  0,   5,  5,   5,  5,  0, -10, 0, 0, 0, 0, 0, 0, 0, 0,
    -5,   0,   5,  5,   5,  5,  0, -5,  0, 0, 0, 0, 0, 0, 0, 0,
    0,    0,   5,  5,   5,  5,  0, -5,  0, 0, 0, 0, 0, 0, 0, 0,
    -10,  5,   5,  5,   5,  5,  0, -10, 0, 0, 0, 0, 0, 0, 0, 0,
    -10,  0,   5,  0,   0,  0,  0, -10, 0, 0, 0, 0, 0, 0, 0, 0,
    -20, -10, -10, -5,  -5, -10, -10, -20, 0, 0, 0, 0, 0, 0, 0, 0
};

// King Middle Game Evaluation Table
const int kingMidGameTable[128] = {
    -30, -40, -40, -50, -50, -40, -40, -30, 0, 0, 0, 0, 0, 0, 0, 0,
    -30, -40, -40, -50, -50, -40, -40, -30, 0, 0, 0, 0, 0, 0, 0, 0,
    -30, -40, -40, -50, -50, -40, -40, -30, 0, 0, 0, 0, 0, 0, 0, 0,
    -30, -40, -40, -50, -50, -40, -40, -30, 0, 0, 0, 0, 0, 0, 0, 0,
    -20, -30, -30, -40, -40, -30, -30, -20, 0, 0, 0, 0, 0, 0, 0, 0,
    -10, -20, -20, -20, -20, -20, -20, -10, 0, 0, 0, 0, 0, 0, 0, 0,
    20,   20,   0,   0,   0,   0,  20,  20, 0, 0, 0, 0, 0, 0, 0, 0,
    20,   30,  10,   0,   0,  10,  30,  20, 0, 0, 0, 0, 0, 0, 0, 0
};

// King End Game Evaluation Table
const int kingEndGameTable[128] = {
    -50, -40, -30, -20, -20, -30, -40, -50, 0, 0, 0, 0, 0, 0, 0, 0,
    -30, -20, -10,  0,   0, -10, -20, -30, 0, 0, 0, 0, 0, 0, 0, 0,
    -30, -10,  20, 30,   30,  20, -10, -30, 0, 0, 0, 0, 0, 0, 0, 0,
    -30, -10,  30, 40,   40,  30, -10, -30, 0, 0, 0, 0, 0, 0, 0, 0,
    -30, -10,  30, 40,   40,  30, -10, -30, 0, 0, 0, 0, 0, 0, 0, 0,
    -30, -10,  20, 30,   30,  20, -10, -30, 0, 0, 0, 0, 0, 0, 0, 0,
    -30, -30,  0,   0,   0,   0, -30, -30, 0, 0, 0, 0, 0, 0, 0, 0,
    -50, -30, -30, -30, -30, -30, -30, -50, 0, 0, 0, 0, 0, 0, 0, 0
};

const int MOBILITY_REWARD = 9;

int materialEval(int* board, int* piecePositions) {
    int score = 0;

    const int centrePawnValue = 15;

    // // WHITE PIECES
    // PAWN, rewarding pawns closer to center more
    int pawnNumW = 0;
    for (int i = 1; i <= 8; i++) {
        if (piecePositions[i] >= 0) {
            score += 100 /*+ centrePawnValue * (piecePositions[i] % 8 <= 3 ? (piecePositions[i] % 8) - 1 : 6 - (piecePositions[i] % 8))*/;
            pawnNumW++;
        }
    }

    // KNIGHT
    if (piecePositions[9] >= 0) score += 320;
    if (piecePositions[10] >= 0) score += 320;

    // BISHOP
    if (piecePositions[11] >= 0) score += 330;
    if (piecePositions[12] >= 0) score += 330;

    // ROOK
    if (piecePositions[13] >= 0) score += 500;
    if (piecePositions[14] >= 0) score += 500;

    // QUEEN
    if (piecePositions[15] >= 0) score += 900;

    // KING
    if (piecePositions[16] >= 0) score += 20000;

    // // BLACK PIECES
    // PAWN, rewarding pawns closer to center more
    int pawnNumB = 0;
    for (int i = 17; i <= 24; i++) {
        if (piecePositions[i] >= 0) {
            score -= 100 /*+ centrePawnValue * (piecePositions[i] % 8 <= 3 ? (piecePositions[i] % 8) - 1 : 6 - (piecePositions[i] % 8))*/;
            pawnNumB++;
        }
    }

    // KNIGHT
    if (piecePositions[25] >= 0) score -= 320;
    if (piecePositions[26] >= 0) score -= 320;

    // BISHOP
    if (piecePositions[27] >= 0) score -= 330;
    if (piecePositions[28] >= 0) score -= 330;

    // ROOK
    if (piecePositions[29] >= 0) score -= 500;
    if (piecePositions[30] >= 0) score -= 500;

    // QUEEN
    if (piecePositions[31] >= 0) score -= 900;

    // KING
    if (piecePositions[32] >= 0) score -= 20000;

    return score;
}

int mobilityEvalWhite(int* board, int* piecePositions) {
    int score = 0;

    

    for (int i = 9; i <= 16; i++) {
        int source = piecePositions[i];
        if (source < 0) continue;
        int piece = EXTRACT_PIECE_TYPE(board[source]);

        switch (piece) {
        case KNIGHT:
            for (int offset : nightoffsets) {
                int target = source + offset;
                if (target & 0x88) continue;
                if (WHITE_AS_1 == EXTRACT_PIECE_COLOUR(board[target])) continue;

                score += MOBILITY_REWARD;
            }
            break;
        case BISHOP:
            for (int i = 1; i < 8; i+=2) {
                for (int j = 1; j < 8; j++) {
                    int target = source + directionaloffsetsS[i] * j;

                    if (target & 0x88) break;
                    if (WHITE_AS_1 == EXTRACT_PIECE_COLOUR(board[target])) break;

                    score += MOBILITY_REWARD;

                    // If piece is enemy
                    if (board[target] != 0) break;
                }
            }
            break;
        case ROOK:
            for (int i = 0; i < 8; i += 2) {
                for (int j = 1; j < 8; j++) {
                    int target = source + directionaloffsetsS[i] * j;

                    if (target & 0x88) break;
                    if (WHITE_AS_1 == EXTRACT_PIECE_COLOUR(board[target])) break;

                    score += MOBILITY_REWARD;

                    // If piece is enemy
                    if (board[target] != 0) break;
                }
            }
            break;
        case QUEEN:
            for (int i = 0; i < 8; i++) {
                for (int j = 1; j < 8; j++) {
                    int target = source + directionaloffsetsS[i] * j;

                    if (target & 0x88) break;
                    if (WHITE_AS_1 == EXTRACT_PIECE_COLOUR(board[target])) break;

                    score += MOBILITY_REWARD;

                    // If piece is enemy
                    if (board[target] != 0) break;
                }
            }
            break;
        }
    }



    return score;
}

int mobilityEvalBlack(int* board, int* piecePositions) {
    int score = 0;

    for (int i = 25; i <= 32; i++) {
        int source = piecePositions[i];
        if (source < 0) continue;
        int piece = EXTRACT_PIECE_TYPE(board[source]);

        switch (piece) {
        case KNIGHT:
            for (int offset : nightoffsets) {
                int target = source + offset;
                if (target & 0x88) continue;
                if (BLACK_AS_2 == EXTRACT_PIECE_COLOUR(board[target])) continue;

                score -= MOBILITY_REWARD;
            }
            break;
        case BISHOP:
            for (int i = 1; i < 8; i += 2) {
                for (int j = 1; j < 8; j++) {
                    int target = source + directionaloffsetsS[i] * j;

                    if (target & 0x88) break;
                    if (BLACK_AS_2 == EXTRACT_PIECE_COLOUR(board[target])) break;

                    score -= MOBILITY_REWARD;

                    // If piece is enemy
                    if (board[target] != 0) break;
                }
            }
            break;
        case ROOK:
            for (int i = 0; i < 8; i += 2) {
                for (int j = 1; j < 8; j++) {
                    int target = source + directionaloffsetsS[i] * j;

                    if (target & 0x88) break;
                    if (BLACK_AS_2 == EXTRACT_PIECE_COLOUR(board[target])) break;

                    score -= MOBILITY_REWARD;

                    // If piece is enemy
                    if (board[target] != 0) break;
                }
            }
            break;
        case QUEEN:
            for (int i = 0; i < 8; i++) {
                for (int j = 1; j < 8; j++) {
                    int target = source + directionaloffsetsS[i] * j;

                    if (target & 0x88) break;
                    if (BLACK_AS_2 == EXTRACT_PIECE_COLOUR(board[target])) break;

                    score -= MOBILITY_REWARD;

                    // If piece is enemy
                    if (board[target] != 0) break;
                }
            }
            break;
        }
    }



    return score;
}

//int counter = 0;

int pieceTable(int* piecePositions) {
    int score = 0;

    for (int i = 1; i <= 8; i++) {
        if (piecePositions[i] >= 0) {
            score += whitePawnTable[piecePositions[i]];
        }
    }

    // KNIGHT
    if (piecePositions[9] >= 0) score += knightTable[piecePositions[9]];
    if (piecePositions[10] >= 0) score += knightTable[piecePositions[10]];

    // BISHOP
    if (piecePositions[11] >= 0) score += bishopTable[piecePositions[11]];
    if (piecePositions[12] >= 0) score += bishopTable[piecePositions[12]];

    // ROOK
    if (piecePositions[13] >= 0) score += rookTable[piecePositions[13]];
    if (piecePositions[14] >= 0) score += rookTable[piecePositions[14]];

    // QUEEN
    if (piecePositions[15] >= 0) score += queenTable[piecePositions[15]];

    // KING
    if (piecePositions[16] >= 0) score += kingMidGameTable[piecePositions[16]];

    // // BLACK PIECES
    // PAWN, rewarding pawns closer to center more
    for (int i = 17; i <= 24; i++) {
        if (piecePositions[i] >= 0) {
            score += blackPawnTable[piecePositions[i]];
        }
    }

    // KNIGHT
    if (piecePositions[25] >= 0) score -= knightTable[piecePositions[25]];
    if (piecePositions[26] >= 0) score -= knightTable[piecePositions[26]];

    // BISHOP
    if (piecePositions[27] >= 0) score -= bishopTable[piecePositions[27]];
    if (piecePositions[28] >= 0) score -= bishopTable[piecePositions[28]];

    // ROOK
    if (piecePositions[29] >= 0) score -= rookTable[piecePositions[29]];
    if (piecePositions[30] >= 0) score -= rookTable[piecePositions[30]];

    // QUEEN
    if (piecePositions[31] >= 0) score -= queenTable[piecePositions[31]];

    // KING
    if (piecePositions[32] >= 0) score -= kingMidGameTable[piecePositions[32]];

    return score;
}

int ChessEngine::evaluate(int encodedInfo, int* board, int* piecePositions) {
    int score = 0;

    //counter++;

    score += pieceTable(piecePositions);
    score += materialEval(board, piecePositions);
    score += mobilityEvalWhite(board, piecePositions);
    score += mobilityEvalBlack(board, piecePositions);


    return score;
}

bool ChessEngine::isKingInCheck(int* board, int kingSquare, int colourToPlay) {
    if (colourToPlay == WHITE_AS_1) {

        // Check if there is a pawn checking the king
        if (board[kingSquare - 15] == BLACK + PAWN) {
            return true;
        }
        if (board[kingSquare - 17] == BLACK + PAWN) {
            return true;
        }
    }
    else if (colourToPlay == BLACK_AS_2) {

        // Check if there is a pawn checking the king
        if (board[kingSquare + 15] == WHITE + PAWN) {
            return true;
        }
        if (board[kingSquare + 17] == WHITE + PAWN) {
            return true;
        }
    }

    // Up Down Left Right offset
    for (int i = 0; i < 8; i += 2) {
        for (int j = 1; j <= 7; j++) {
            int square = kingSquare + directionaloffsetsS[i] * j;

            if (square & 0x88) break;

            int piece = board[square];

            // If king is same colour as piece on square
            if (colourToPlay == EXTRACT_PIECE_COLOUR(piece)) break;

            // If piece is rook or queen
            if (EXTRACT_PIECE_TYPE(piece) == ROOK || EXTRACT_PIECE_TYPE(piece) == QUEEN) return true;
            else if (piece != 0) break;
        }
    }

    // Diagonal offset
    for (int i = 1; i < 8; i += 2) {
        for (int j = 1; j <= 7; j++) {
            int square = kingSquare + directionaloffsetsS[i] * j;

            if (square & 0x88) break;

            int piece = board[square];

            // If king is same colour as piece on square
            if (colourToPlay == EXTRACT_PIECE_COLOUR(piece)) break;

            // If piece is rook or queen
            if (EXTRACT_PIECE_TYPE(piece) == BISHOP || EXTRACT_PIECE_TYPE(piece) == QUEEN) return true;
            else if (piece != 0) break;
        }
    }

    for (int offset : nightoffsets) {
        int square = kingSquare + offset;

        if (square & 0x88) continue;

        int piece = board[square];

        // If king is same colour as piece on square
        if (colourToPlay == EXTRACT_PIECE_COLOUR(piece)) continue;

        // If piece is rook or queen
        if (EXTRACT_PIECE_TYPE(piece) == KNIGHT) return true;
    }
}