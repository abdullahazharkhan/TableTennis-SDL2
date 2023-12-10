# Table Tennis Game (Pong) - C with SDL2

Table Tennis is a Pong game developed in C using the SDL2 library. This game offers an immersive experience with the following features:

## Features

### Colorful GUI
The game boasts a vibrant and visually appealing graphical user interface with a spectrum of colors enhancing the gaming experience.

### Player vs. Simple AI Bot
Challenge yourself against a simple AI bot designed to engage players in thrilling matches.

### Bot Losing a Point
The AI bot participates actively, not just in scoring but also in conceding points, ensuring a dynamic and competitive gameplay environment.

### Interactive Score Bar
A dynamic score bar located below the gameplay area reflects the ongoing score status:
- Turns red when the AI scores.
- Turns blue when the player scores.
The score bar also moves towards the paddle that scores a point, providing a clear visual cue of the current scorer.

### ScoreBoard.txt
The game maintains a file named "ScoreBoard.txt" to track scores throughout the gameplay. At the game's conclusion, the file displays the winner based on the accumulated scores.

### Score Display in Window Title
The current score is prominently displayed in the window title during gameplay, allowing players to track the progress without distractions.

## Implementation Details

### Usage
Players can refer to the README file for instructions on building, running, and enjoying the Table Tennis game.

## Command to Compile
gcc -Isrc/include -Lsrc/lib -o main main.c -lmingw32 -lSDL2main -lSDL2

## Acknowledgements
The game development acknowledges the use of the SDL2 library and other relevant resources or tools contributing to the creation of this engaging gaming experience.

![rally](https://github.com/abdullahazharkhan/TableTennis-SDL2/assets/109475658/0816b0d4-18df-4621-987e-1f324a5b01aa)

