## Snake Game

### Description

A simple Snake game written in C.

### How to run

- Install SDL2
- Update the Include path in the C/C++: Edit Configurations (UI) to include the filepath
    i.e, for Mac, Apple Silicon, we add this following path: "/opt/homebrew/include/"
- Then as noticed in game.c, we have #include <SDL2/SDL.h>

1. Compile the code using the command `gcc ./src/game.c -o game`
2. Run the game using the command `./game`