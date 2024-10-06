# Chess Engine

This project is a chess engine designed to let users play against the chess engin or another human player. It uses Minimax and Alpha Beta Pruning to calculate the best move. The project is a desktop application which provides a Chess board to play and a Chess engine which can play moves on the board. The chess engine is it's own standalone module and can be used in other projects as a bot opponent. 

## Table of Contents
  - [Project Ambitions](#project-ambitions)
  - [Features](#features)
  - [Technologies Used](#technologies-used)
  - [Installation](#installation)
  - [Usage](#usage)
      - [Playing With Two Humans](#playing-with-two-humans)
      - [Playing Against the Engine](#playing-against-the-engine )
      - [Integrating the Engine into Your Projects](#integrating-the-engine-into-your-projects)
  - [Project Status](#project-status)
      - [Notable Wins](#notable-wins)
      - [Upcoming Features](#upcoming-features)
  - [Known Issues](#known-issues)
  - [Ackowledgments](#acknowledgments)

## Project Ambitions
The ambition behind this project is to develop a fully functional chess engine, for positions reachable in a normal game, that could be used either as a standalone app or as a backend for a larger chess platform. It's aimed at providing a competitive and enjoyable experience for chess players of all skill levels.

## Features
Current features:
 - A chess board that allows for play against the engine or against another person on the same computer
 - An engine that can make moves on the board when it is its turn to do so
 - 

## Technologies Used
 - **Programming language**: C++
 - **Libraries**: SDL2, SDL2_image

## Installation

## Usage
### Playing With Two Humans
Currently, there is no variable which can be changed in the code which could make this happen easily. To accomplish this, the user must change a few lines of code in ```App.cpp``` inside the main app loop. 
```cpp
if (!game.isEnginesTurnToPlay()) eventHandler.pollEvent();
else {
    game.playEngineMove();
}
```
These lines must be changed to...
```cpp
eventHandler.pollEvent();
```
... for epik two player gaming to be enabled

### Playing Against the Engine
As of now, the engine is ready to play against as soon as you download the code. To change the colour that the engine plays, you must change one argument in ```App.cpp```.
```cpp
game.initEngine(depth, seed, BLACK_AS_2, FEN);
```
The 3rd argument controls the engine's colour. To switch it to white, change the argument to ```WHITE_AS_1```

The user also has the ability to change the depth that the engine searches to by changing the value stored in ```depth```, which is initalized a few lines above the previous bit of code, or by changing the respective argument in the code above. However, this is not recommended as in the current version of the engine, a depth of 5 is the highest it can go to whilst playing its moves at an acceptable speed.

### Integrating the Engine into Your Project
If the user is interseted in integrating the engine into their projects, they will only need a few methods provided by the engine. They will find these method in ```ChessEngine.h``` and all their definitions in the .cpp files whose names start with Engine. These should all be found in the Chess Engine folder.
```cpp
ChessEngine(int depth, unsigned int seed, int engineColour);
```
This is the constructer of the ChessEngine class.
```seed``` relates to a feature which is currently being worked on.
For the ```engineColour```, the user must pass a 1 for white or a 2 for black. The default is black if neither of these are passed.
```depth``` is the depth of the search

```cpp
void loadFromFen(std::string FEN);
```
This is the method that loads the board state from a fen string. If the fen string is invalid, it defaults to the starting position. Currently, fen strings that lead to positions where there is an unusual amount of any piece (the number of pieces of that type is more than what is found in a normal game) leads undefined behaviour.

```cpp
bool playPlayerMove(int encodeMove, int encodedInfo);
```
This is the method the user must call in their code when the player makes a move. It returns true if it is successful and false if the colour to play stored in the engine does not match the colour of the piece played.
`encodedMove` contains all the info on the move played. How moves are encoded can be found [here](#how-moves-are-encoded)
`encodedInfo` contains all the info on the board state before the move was played. How board states are encoded can be found [here](#how-board-states-are-encoded)

```cpp
int playEngineMove();
```
This method returns the move played in the encoded form described [here](#how-moves-are-encoded).

### How Moves Are EncodeD
All moves are encoded in a 4 byte integer. Below is a table which shows what each group of bits in the integer represents.

| Bits | Meaning |
| ---- | ------- |
| 0000 0000 0000 0000 0111 1111 | source square |
| 0000 0000 0011 1111 1000 0000 | target square |
| 0000 0011 1100 0000 0000 0000 | piece promotion |
| 0000 0100 0000 0000 0000 0000 | capture flag |
| 0000 1000 0000 0000 0000 0000 | double pawn move flag |
| 0001 0000 0000 0000 0000 0000 | enpassant flag |
| 0010 0000 0000 0000 0000 0000 | castling flag |

The chess engine header file (`ChessEngine.h`), contains some macros which can be used to encode a move and extract info from the encoded move. 
```cpp
#define ENCODE_MOVE(source, target, code, pieceCaptured, capturedLookup) ( source | (target << 7) | (code << 14) | (pieceCaptured << 18) | (capturedLookup << 24))

#define EXTRACT_SOURCE_SQUARE(move) (move & 0x7f)
#define EXTRACT_TARGET_SQUARE(move) ((move >> 7) & 0x7f)
#define EXTRACT_MOVE_CODE(move) ((move >> 14) & 0xf)
#define EXTRACT_PIECE_CAPTURED(move) ((move >> 18) & 0x3f)
#define EXTRACT_CAPTURED_LOOKUP(move) ((move >> 24) & 0x7f)
```

### How Board States Are Encoded
All board states are encoded in a 4 byte integer. Below is a table which shows what each group of bits in the integer represents.

// 1111 1111 1111 1111 1111 1111 1111
//  $$$ $$$$ |||| |||^ ^^^^ ^^## ##..

| Bits | Meaning |
| ---- | ------- |
| 0000 0000 0000 0000 0000 0000 0011 | colour to play |
| 0000 0000 0000 0000 0000 0011 1100 | castling rights |
| 0000 0000 0000 0001 1111 1100 0000 | square of last double pawn move |
| 0000 0000 1111 1110 0000 0000 0000 | square of the white king |
| 0111 1111 0000 0000 0000 0000 0000 | square of the black king |

The chess engine header file (`ChessEngine.h`), contains some macros which can be used to encode a board state, extract info from the encoded board state and updata a certain property of it. 
```cpp
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
```

## Project Status
### Notable Wins
Current notable wins are:
 - once against me (rated 1500 on [chess.com](https://www.chess.com/member/jaan2trappy)
 - 3 times against my friend (rated 1100 on [chess.com](https://www.chess.com/member/k26s)
 - fairy stockfish 14 level 5 on [lichess.com](https://lichess.org/)

### Upcoming Features
Current features in the works are:
 - Using zobrist hashing to store positions in a hash table to improve search speeds
 - Creating an opening database
 - Adding features in the GUI to make it easier to configure the engine
 - Changing piece designs to something easier to see
 - Removing bugs
 - 
## Known Issues
Current known issues are:
 - No easy way to configure the engine
 - No numbers and letters along the edge of the board to easily tell co-oridnates of each square
 - The user can move the engine's pieces
 - The engine doesn't really like its king that much
 - The engine refuses to castle
 - The engine takes too long at depths of 6 or higher
 - No draw by repition, 50 move rule or if there is a dead position
 - Cannot resign or draw without closing the program

## Acknowledgments
Some code has been borrowed or adapted from third party sources.
The sources and what has been borrowed are:
 - Boiler plate code related to SDL2 and SDL2_image ([Beginning Game Programming v2.0](https://lazyfoo.net/tutorials/SDL/))
