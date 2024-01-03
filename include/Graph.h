#pragma once

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

#include "Hex.h"

const int ROW_STEPS[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int COL_STEPS[8] = {-1, 0, 1, -1, 1, -1, 0, 1};


class Graph {
 private:
  std::vector<std::vector<Hex *>> m_board;
  int m_rows, m_screenSize;

  void createNeighbors();

  bool cellIsValid(int row, int col, int rowsCount, int colCount) const;

 public:
  Graph() = default;

  Graph(int rows, int screenSize);

  void BFSAndColor(Hex *hex, COLORS newColor);

  Hex *p2Player(int row, int col, OWNER owner) const;

  void DrawGraph(sf::RenderWindow *window);

  void resetAllVisited() const;

  float getPlayerPercent(OWNER owner) const;
};