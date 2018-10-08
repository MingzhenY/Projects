# Projects

## Awari

After reading the AlphaZero paper, the idea that the program learns only by selfplay is intresting, the only inputs needed are the game rules.

The code is written using template so that it's easy to define a new game
and play against computer.

Might add methods other than MCTS in the future.  

To use the code, define your own game class in games.h.
See main.cpp for examples.

games.h:
The game classes are defined here.
It should be a two-player perfect information game.

gametest.h:
Given the game class, checks if it is well defined.

mcts.h:
Uses Monte-Carlo Tree Search to find best moves.

gameplay.h:
The code to play the game against computer.

main.h:
main code