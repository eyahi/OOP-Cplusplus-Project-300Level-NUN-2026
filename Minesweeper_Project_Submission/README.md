### MINESWEEPER C++ OBJECT ORIENTED PROGRAMMING PROJECT

### GROUP MEMBERS: Aminatu Tukur Kabir 20231685, Anjolaoluwa Olatunde 20232463, Maryam Abdullahi 20231533

## INTRODUCTION - PROJECT OVERVIEW 
This project is a fully functional desktop recreation of a minesweeper game. Developed using C++ and the Safe and Fast Multimedia Library (SFML); it  features custom assets, sound effects and a constant "Best Time" high score. The goal of the game is to reveal all the safe cells without triggering hidden mines. 

## FEATURES 
- Graphical User Interface (GUI): The game uses a spritesheet for tiles and a faces sheet for game status.
- Object Oriented Design (OOP): A MinesweeperGame class structure was built to manage game states, assets, and game logic.
- Mouse Interaction: Fully interactive grid responding to left and right mouse clicks in the window promptly.
- Restart Logic: Clicking the central face above the game board resets the board, the timer and game state at any moment.
- SFML Integration: Uses SFML for window management, real-time event polling, textures and audio.

## ALGORITHMS
- Mine Placement: Mines are placed randomly across the grid using std::rand() and the coordinates are verified to prevent overlaps.
- Flood Reveal: When an empty cell is clicked, the game uses a recursive algorithm to flood and reveal all adjacent empty/ numbered cells. This speeds up gameplay significantly.
- Neighbour Calculation: After mines are placed, every cell runs a check on the 8 cells closest to it to calculate the number of mines that could be nearby.

## DEPENDENCIES
- C++ Version: c++17 or higher (for std::optional support)
- Library: SFML 3.0.2 (64 bit) and the matching compiler
- Assets: All the images and audio files have to remain in their specific folders to be executed

## COMPILING INSTRUCTIONS 
To compile this project, the SFML library used must be linked to the compiler within VS Code.

1. Ensure the path to your SFML headers and libraries are included
2. The "sfml-graphics","sfml-window","sfml-system" and "sfml-audio" are linked
3. Update your .vscode/tasks.json and c_cpp_properties.json to include the library paths
4. Ensure the dll files from the SFML bin folder are in the same folder as the .exe file
5. To compile open the terminal, in powershell and type this out: 
g++ -std=c++17 main.cpp MinesweeperGame.cpp -o minesweeper.exe -I"C:\SFML-3.0.2\include" -L"C:\SFML-3.0.2\lib" -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system. It should run without error.
6. To run the game, type: ./minesweeper.exe

## FILE STRUCTURE
minesweeper project/
├── assets/               # Graphics 
├── audio/                # Sound Effects
├── fonts/                # Digital Display.ttf font
├── Cell.h                # Contains Cell struct
├── MinesweeperGame.h     # Class declaration and constants
├── MinesweeperGame.cpp   # Game logic and event handling
├── main.cpp              # Program entry point
├── README.md             # Contains information on the game set up and functionality
└── best_time.txt         # Auto generated file to save best time

## HOW TO PLAY
1. Left Click: Reveals a cell. If it reveals a mine, the game ends. If it reveals a number, that's how many mines are nearby. If it reveals an empty cell, the neighbouring empty and numbered cells will automatically reveal.
2. Right Click: Flags a cell (assuming there's a mine there)
3. Click Face icon: Starts a new game. 
4. Goal: Reveal all safe cells without hitting a mine.

## UI AND THEIR USES 
- Faces: These indicate game status, i.e. "Happy" is displayed while playing, "Dead" is displayed when the game is lost, "Cool" is displayed when the game is won
- Timer(Top Right): Shows the time that has elapsed during game play.
- Best Time (Top Left): Shows the fastest time the game has been completed. In the instance the player completes the game faster than the best time, the best time gets replaced with the faster time.
- Grid: The play area where the tiles change state - flag, reveal, etc. - based on the user interaction.

## KNOWN ISSUES AND LIMITATIONS
- First Click: In standard minesweeper games, it is impossible to hit a mine on the first click to ensure fairness. With the current code structure, there is no function/variable to check if there's been a first click, meaning there is a possibilty of hitting a mine on the first click. This also affects the timer as it starts running immediately the game is started/reset as opposed to starting after the first click.
- Window Resizing: The User Interface is designed for a fixed resoltion based on cellsize. Resizing the window will distort the grid layout and the gameplay.
- Infinite Flagging: In standard minesweeper there is a limit to how many flags can be placed. However there is no limit to the number of flags that can be placed in the project.

### AUTHORS:
Aminatu Tukur Kabir 20231685
Anjolaoluwa Olatunde 20232463
Maryam Abdullahi 20231533