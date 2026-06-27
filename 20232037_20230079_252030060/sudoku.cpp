#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>

const int SIZE = 9;
const int CELL_SIZE = 60;
const int GRID_SIZE = CELL_SIZE * SIZE;

int puzzle[SIZE][SIZE] = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0}, {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},

    {8, 0, 0, 0, 6, 0, 0, 0, 3}, {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},

    {0, 6, 0, 0, 0, 0, 2, 8, 0}, {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9}};

bool fixedCell[SIZE][SIZE];

bool valid(int row, int col, int num) {
  for (int i = 0; i < SIZE; i++)
    if (puzzle[row][i] == num || puzzle[i][col] == num)
      return false;

  int sr = (row / 3) * 3;
  int sc = (col / 3) * 3;

  for (int r = 0; r < 3; r++)
    for (int c = 0; c < 3; c++)
      if (puzzle[sr + r][sc + c] == num)
        return false;

  return true;
}

int main() {
  sf::RenderWindow window(sf::VideoMode(sf::Vector2u{GRID_SIZE, GRID_SIZE}),
                          "Sudoku - SFML 3");

  sf::Font font;
  if (!font.openFromFile("arial.ttf"))
    return 0;

  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++)
      fixedCell[r][c] = (puzzle[r][c] != 0);

  int selectedRow = -1, selectedCol = -1;

  while (window.isOpen()) {
    while (auto eventOpt = window.pollEvent()) {
      const sf::Event &event = *eventOpt;

      if (event.is<sf::Event::Closed>())
        window.close();

      if (auto mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        int x = mouse->position.x / CELL_SIZE;
        int y = mouse->position.y / CELL_SIZE;
        selectedCol = x;
        selectedRow = y;
      }

      if (auto textEvt = event.getIf<sf::Event::TextEntered>()) {
        if (selectedRow == -1)
          continue;

        char ch = textEvt->unicode;
        if (ch >= '1' && ch <= '9' && !fixedCell[selectedRow][selectedCol]) {
          int val = ch - '0';
          if (valid(selectedRow, selectedCol, val))
            puzzle[selectedRow][selectedCol] = val;
        }
        if (ch == '0' && !fixedCell[selectedRow][selectedCol])
          puzzle[selectedRow][selectedCol] = 0;
      }
    }

    window.clear(sf::Color::White);

    // Grid lines
    for (int i = 0; i <= SIZE; i++) {
      sf::RectangleShape line(
          sf::Vector2f(GRID_SIZE, (i % 3 == 0) ? 3.f : 1.f));
      line.setPosition(sf::Vector2f(0.f, i * CELL_SIZE));
      line.setFillColor(sf::Color::Black);
      window.draw(line);

      sf::RectangleShape vline(
          sf::Vector2f((i % 3 == 0) ? 3.f : 1.f, GRID_SIZE));
      vline.setPosition(sf::Vector2f(i * CELL_SIZE, 0.f));
      vline.setFillColor(sf::Color::Black);
      window.draw(vline);
    }

    // Numbers
    for (int r = 0; r < SIZE; r++) {
      for (int c = 0; c < SIZE; c++) {
        if (puzzle[r][c] != 0) {
          sf::Text text(font, std::to_string(puzzle[r][c]), 32);
          text.setFillColor(fixedCell[r][c] ? sf::Color::Black
                                            : sf::Color::Blue);
          text.setPosition(
              sf::Vector2f(c * CELL_SIZE + 20, r * CELL_SIZE + 10));
          window.draw(text);
        }
      }
    }

    // Highlight selected
    if (selectedRow != -1) {
      sf::RectangleShape rect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
      rect.setPosition(
          sf::Vector2f(selectedCol * CELL_SIZE, selectedRow * CELL_SIZE));
      rect.setFillColor(sf::Color(200, 200, 0, 80));
      window.draw(rect);
    }

    window.display();
  }

  return 0;
}
