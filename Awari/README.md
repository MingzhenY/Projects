# Awari

## Motivation

After reading the AlphaZero paper, the idea that the program learns only by selfplay is interesting, the only inputs needed are the game rules.

AlphaZero uses neural network and MCTS(Monte-Carlo Tree Search).
Didn't know much about neural networks, but MCTS seems easy to implement.
With the same thinking time, MCTS program easily beats a program that looks several
steps ahead(DFS).

## Game Board

        f   e   d   c   b   a
    12  5   4   3   2   1   0
        6   7   8   9  10  11  13
        A   B   C   D   E   F

    Player 0 controls pits 0 to 5 and pit 13. pit 13 is used to store captured stones.
    Player 1 controls pits 6 to 12 where pit 12 is used to store captured stones.
    pits 0 to 5 are called a to f.
    pits 6 to 11 are called A to F.

## Rules

    1. At the start of the game, each pit(except for 12 and 13) has 4 stones.
    2. At each step a player selects a non-empty pit X from his/her row, starting
        with X's neighbor, he/she then sows all stones from X, one at a time,
        counter-clockwise over the board(omitting 12 and 13 and itself). After
        the move, X will be empty even if X has 12 stones or more. Captured Stones
        are removed and stored in either pit 6 or 13, depending on the player.
    3. Stones are captured if the last stone sown lands in an enemy pit which after
        landing contains 2 or 3 stones. If such a capture is made, and the preceding
        pit contains 2 or 3 stones and the pit is an enemy pit, those stones are also
        captured. This procedure is successively repeated for the pits preceding and
        ends as soon as a pit is encountered containing a number of stones other than
        2 or 3, or the end of the opposing row is reached.
    4. A move is described by the name of the pit, followed by the number of stones sown
        (the name of the pit by itself defines the move, but such a notations is prone to error).
        The number of stones captured, if any, is indicated by the amount preceded by a "x".
        For example: A1, C4 x 2, D19 x 7, ...
    5. End of the game:
    5.1. The Goal of awari is to capture more stones than the opponent. The game ends
        as soon as one of the player has collected 25 or more stones.
    5.2. If a player is unable to move, the remaining stones are captured by the opponent.
    5.3. If the same position is encountered for the third time, with the same player to move,
        the remaining stones on the board are evenly devided among the players.
    6. The winner is the player who captured the most stones. If both players capture 24 stones,
        the game is drawn.
    7. A last rule exists to prevent players from running out of moves early in the game. Whenever
        possible, a player is forced to choose a move such that the opponent is able to make a reply
        move. It is, however, not compulsory to look several moves ahead to ensure that the opponent
        will continue to be able to reply.

## Monte-Carlo Tree Search

MCTS performs much better than DFS.
Normally MCTS will keep the number of nodes at 'NodeLimit'.
But in the end game, the number of states are less than 'NodeLimit'.
So I used a simulation limit instead. We will keep the total simulation
number at 'SimLimit'. A end game node can be simulated multiple times if it
is selected multiple times.
When a player moves, the irrelevant parts of the game tree are deleted.
The relevant parts are reused.

## Changes from V1 to V2

1.The MCTS codes now uses a template.
It is easier now to apply the same code to more games.

2.Uses std::string to represent the game board and the moves instead of classes in V1.
Tried to keep them as classes but failed, strings are much easier to work with.

3.Now awari.h and mcts.h are seperate. One does not need to include the other.
This makes more sense.

4.Game History are removed (should be added later).

## Future Work for V2

1.The requirements of the class Game is not complete.
There are requirements from mcts.h and also awari.cpp.

2.Need to test on more games.

3.Add game history in awari.cpp

4.clean up the code

## Changes from V2 to V3

1.Added a new game Mancala. It has different rules.

2.Game.ValidMovesVec() generate a vector of the short version of a move(a-f and A-F).
After Play(), included in each move is the number of stones sowed and captured.
For example, F3X1 means sowed 3 stones, captured 1 stones.

If After Play(), a ends starts with '|', it means that the player has additional turns.
So a move has 3 parts:

* ActualMove
* MoveInfo(Optional)
* does the player has another turn

After Expanding, MCT->Moves stores moves with all the information.

3.Game.IfPlay now changes the input Move.
This allows the MCT Node to store the updated move.
The ending '|' is needed when selecting the best childnode.

4.In class Mancala, Game.board and MCT->Board are now exactly the same.
In old Awari.h, MCT->Board includes in the string the player to move and the state of the game,
but in Game, they are seperate from Game.board as Game.player and Game.state.

## Changes from V3 to V4

1.Added gametest.h that checks if a game class is well defined.

2.Added gameplay.h that has all the gameplay codes.

3.Added games.h that stored all the defined games.
    The basic requirements are stated at the start of the file.

4.Rewrite mcts.h and main.cpp
    The first character of a move now indicates whether the player's
    turn ends.

5.Added the game Adi from "The complete mancala games" by Larry Russ page 59.
In a move, a player picks up all the seeds in a pit and place them one by one
counterclockwise starting from the next pit. As long as the last seed does
not lands in a pit that has 0 or 3 seeds(1 or 4 after adding the last seed),
the player picks up all the seeds in this pit and start again.

There are situations where a move goes into a loop and never ends. This is
not mentioned in the book. I split the remaining seeds evenly between two
player when this happens.

6.Added the game Awari(called Oware on Wikipedia).
The rules I used here comes from the paper "Searching for Solutions in Games and Artificial Intelligence" by Louis Victor Allis

## Future Work
1.Add more games in games.h

2.Store the MCTS nodes somehow, so that the program
becomes smarter everytime you play.

3.implement Multi_MCTS when the outcome of a game will change
how the rest of the games are played.

4.The game class should has a function that terminates the game early.
This can be used to determin the winner before the game ends.
For example, in a random simulation, if the game goes into a infinite
loop(or takes too long), terminate early and estimate the outcome.