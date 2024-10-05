# Chess Engine

This project is a chess engine designed to let users play against the chess engin or another human player. It uses Minimax and Alpha Beta Pruning to calculate the best move. 

The project is a desktop application which provides a Chess board to play and a Chess engine which can play moves on the board. It is built using C++ and relies upon the SDL2 and SDL2_image libraries to provide a GUI to play on whereas the engine does not use any third party libraries. The chess engine is it's own standalone module and can be used in other projects as a bot opponent. Instructions on how to interact with it will can be found [here](#Instructions On How To Interact With the Engine)

The ambition behind this project is to develop a fully functional chess engine that could be used either as a standalone app or as a backend for a larger chess platform. It's aimed at providing a competitive and enjoyable experience for chess players of all skill levels.

Currently, the project is a work in progress. Basic game mechanics, including the rules of chess and AI move generation, are in place. The following tasks are still pending:

Enhancing the AI with stronger algorithms (or integrating Stockfish)
Improving the graphical user interface
Adding support for multiplayer games and save/reload functionality
Known issues include occasional bugs in detecting checkmate and stalemate situations, and the AI’s performance at higher difficulty levels is not yet fully optimized. Additionally, the user interface is basic and needs more refinement for a polished final product.

## Instructions On How To Interact With the Engine
