#pragma once

#include "Computer.h"
#include "Graph.h"
#include "Menu.h"

const int ROWS = 30;

struct Player {
  int row;
  int col;
  OWNER symbol;
};

class Controller {
 private:
  bool m_newGame = true;
  int m_gameState = RUNNING;
  sf::RenderWindow m_window;

  Graph m_graph;

  Menu m_menu;

  Player m_humanCell = {0, 0, OWNER::HUMAN};
  Player m_compCell = {0, 0, OWNER::PC};
  Hex *m_humanHex;
  Hex *m_computerHex;

  Computer m_computer;

  void initGame();

  void handleClick(const sf::Event::MouseButtonEvent &event);

 public:
  Controller();

  void run();
};
