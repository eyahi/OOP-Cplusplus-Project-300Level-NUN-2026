===============================================================================
                      CHESS GAME - C++/Qt APPLICATION
===============================================================================

PROJECT TITLE: Chess Game
DESCRIPTION: A complete chess game implementation with graphical user interface
             built using C++ and Qt framework. Features both player vs player
             and player vs AI modes with full chess rules implementation.

DEVELOPERS: Abubakar Lawal Aliyu, Almuktar Musa Sani, Baqir Muhammmad Ali
COURSE: CPE 303- Object Oriented Programming and Design II  
CONTACT:   albaqeer5@gmail.com
DATE:      January 2026


===============================================================================
                           QUICK START (FOR TESTING)
===============================================================================

FOR THE LECTURER / QUICK TESTING:
1. Navigate to the `portable_build` folder in the submission
2. Double-click `ChessGame.exe` to launch the game immediately
   - No Qt installation required
   - No compilation needed
   - All dependencies are included

Location: `portable_build/ChessGame.exe`

===============================================================================
                           COMPILATION INSTRUCTIONS
===============================================================================

This project uses Qt 6 with qmake build system.

PREREQUISITES:
1. Qt 6.10.1 or later (with Qt Creator IDE)
2. MinGW 64-bit compiler (Windows) or g++ (Linux/Mac)
3. Qt Creator IDE (recommended)

COMPILATION USING QT CREATOR:
1. Open Qt Creator
2. File → Open File or Project → Select "Chess.pro"
3. Select the appropriate kit (Desktop Qt 6.x.x MinGW 64-bit)
4. Click the green hammer icon (Build) or press Ctrl+B

MANUAL COMPILATION (command line):
1. Navigate to project directory:
   e.g cd C:\Users\albaq\Documents\Chess\
2. Run qmake:
   qmake Chess.pro
3. Build:
   mingw32-make (Windows) or make (Linux/Mac)

===============================================================================
                           EXECUTION INSTRUCTIONS
===============================================================================

AFTER SUCCESSFUL COMPILATION:
1. In Qt Creator: Click green play button (Ctrl+R)
2. From command line:
   .\debug\Chess.exe (Windows)
   ./Chess (Linux/Mac)

===============================================================================
                               FEATURES LIST
===============================================================================

CORE GAMEPLAY:
✅ Complete chess rules implementation
✅ Player vs Player mode
✅ Player vs AI mode (random move AI)
✅ Visual 8x8 chessboard with piece images
✅ Piece movement validation
✅ Check and checkmate detection
✅ Stalemate detection
✅ Pawn movement (except promotion)
✅ Castling, en passant (if implemented in core logic)

USER INTERFACE:
✅ Home screen with background image
✅ Fullscreen/maximized window support
✅ Dynamic board resizing
✅ Player labels (Player 1/Player 2 or AI Opponent)
✅ Game status display (turn, check, checkmate)
✅ Visual piece selection highlighting

GAME CONTROLS:
✅ Pause menu with:
   - Resume game
   - Restart game
   - Return to main menu
   - Quit game
✅ Undo last move
✅ Restart game

GRAPHICS:
✅ Custom chess piece images (PNG format)
✅ Chessboard with alternating square colors
✅ Professional UI styling

===============================================================================
                         INPUT FILE REQUIREMENTS
===============================================================================

1. CHESS PIECE IMAGES:
   - Location: images/ folder in project directory
   - Required files: wk.png, wq.png, wr.png, wb.png, wn.png, wp.png
                     bk.png, bq.png, br.png, bb.png, bn.png, bp.png
   - Format: PNG, transparent background recommended
   - Size: Any square aspect ratio, will be scaled automatically

2. BACKGROUND IMAGE:
   - Location: images/homebg.png
   - Used for home screen background

3. SOURCE CODE STRUCTURE:
   - src/core/ : Chess logic files (Board, Piece, Game classes)
   - Chess.pro : Qt project file
   - .cpp/.h files in project root: Qt GUI implementation

===============================================================================
                            SAMPLE USAGE
===============================================================================

1. LAUNCH APPLICATION:
   - Program starts with home screen showing "Player vs Player", 
     "Player vs AI", and "Exit" buttons

2. PLAYER VS PLAYER MODE:
   - Click "Player vs Player"
   - White pieces (bottom) are Player 1
   - Black pieces (top) are Player 2
   - Click a piece to select it (yellow highlight)
   - Click destination square to move
   - Status shows current turn and check status

3. PLAYER VS AI MODE:
   - Click "Player vs AI"
   - You play as White (Player 1)
   - AI plays as Black (AI Opponent)
   - Make your move, AI responds after 0.5 second delay

4. GAME CONTROLS DURING PLAY:
   - ⏸️ Pause button: Open pause menu
   - ↩️ Undo button: Undo last move
   - Status label: Shows game state

5. PAUSE MENU OPTIONS:
   - Resume: Return to game
   - Restart Game: Reset board to initial position
   - Back to Menu: Return to home screen
   - Quit Game: Exit application

===============================================================================
                      KNOWN ISSUES & LIMITATIONS
===============================================================================

1. PAWN PROMOTION:
   - Pawns reaching the opposite side cannot promote
   - They become stuck and cannot move backward

2. AI LIMITATIONS:
   - AI makes completely random legal moves
   - No strategic planning or difficulty levels
   - May make obviously bad moves

3. GAME LOGIC:
   - Some special moves (en passant) may not be fully implemented
   - Threefold repetition and 50-move rules not implemented
   - No draw offering mechanism

4. PERFORMANCE:
   - No move caching or optimization for AI
   - Board redraws on every mouse move (minor performance impact)

5. UI/UX:
   - No move history display
   - No game clock/timer
   - No sound effects
   - Cannot resize window arbitrarily (only maximized/fullscreen)

6. SAVE/LOAD:
   - No save/load game functionality
   - No game recording/PGN export

===============================================================================
                             DEPENDENCIES
===============================================================================

REQUIRED LIBRARIES:
1. Qt 6 Core, GUI, and Widgets modules
2. Standard C++17 library

REQUIRED TOOLS:
1. Qt Creator IDE (recommended) or qmake command line tool
2. MinGW 64-bit compiler (Windows) or g++ (Linux/Mac)
3. Image viewer (for modifying piece images)

OPTIONAL:
1. Image editing software (to customize piece images)
2. Git (for version control)

===============================================================================
                            PROJECT STRUCTURE
===============================================================================

ChessGame_Submission/
├── portable_build/               # READY-TO-RUN PORTABLE VERSION
│   ├── ChessGame.exe            # Main executable
│   ├── Qt6Core.dll              # Qt Core library
│   ├── Qt6Gui.dll               # Qt GUI library
│   ├── Qt6Widgets.dll           # Qt Widgets library
│   ├── platforms/               # Qt platform plugins
│   │   └── qwindows.dll         # Windows platform plugin
│   ├── imageformats/            # Image format plugins
│   │   ├── qjpeg.dll
│   │   └── qpng.dll
│   └── images/                  # Game graphics
│       ├── wk.png, wq.png, ...  # Chess piece images
│       └── homebg.png           # Home screen background
│
├── ChessGame.pro                # Qt project file (2 KB)
├── Readme.txt                   # Project documentation (8 KB)
├── images/                      # Original graphics assets
│   ├── wk.png, wq.png, ...      # Chess piece images
│   └── homebg.png               # Background image
├── ui/                          # Qt Designer UI files (.ui)
│   ├── mainwindow.ui            # Main window layout
│   ├── homescreen.ui            # Home screen layout
│   └── pausedialog.ui           # Pause menu layout
└── src/                         # All C++ source code
    ├── main.cpp                 # Application entry point
    ├── mainwindow.h/cpp         # Main game window class
    ├── chesswidget.h/cpp        # Chess board widget class
    ├── homescreen.h/cpp         # Home screen class
    ├── pausedialog.h/cpp        # Pause menu class
    ├── ChessBoard.h/cpp         # Core chess logic - board
    ├── Piece.h/cpp              # Base piece class
    ├── King.h/cpp               # King piece implementation
    ├── Queen.h/cpp              # Queen piece implementation
    ├── Rook.h/cpp               # Rook piece implementation
    ├── Bishop.h/cpp             # Bishop piece implementation
    ├── Knight.h/cpp             # Knight piece implementation
    └── Pawn.h/cpp               # Pawn piece implementation


===============================================================================
                            LICENSE & CREDITS
===============================================================================

This project was developed as part of a C++/Qt learning exercise.
Chess piece images should be replaced with legally licensed alternatives
for distribution.

Educational Use: Free
Commercial Use: Requires proper licensing of chess piece images

===============================================================================