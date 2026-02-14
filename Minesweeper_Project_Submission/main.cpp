#include "MinesweeperGame.h"
#include <ctime>
#include <cstdlib>

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    //Creates an instance of the game and starts the run loop
    MinesweeperGame game;
    game.run();
    return 0;
}