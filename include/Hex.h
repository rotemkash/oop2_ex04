#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "Consts.h"

enum COLORS { ZERO, ONE, TWO, THREE, FOUR, FIVE, NA };

enum OWNER { FREE, HUMAN, PC };

class Hex {
 private:
  int m_row, m_col;
  float m_radius;
  COLORS m_color;
  OWNER m_owner = OWNER::FREE;
  bool m_visited = false;
  sf::Clock m_clock;
  sf::Time m_flashTimer = sf::seconds(1.f);

  sf::CircleShape m_shape;

  std::vector<Hex*> m_neighbors;

 public:
  const std::vector<Hex*>& getNeighbors() const;

  Hex(int row, int col, COLORS color, float diameter);

  COLORS getColor() const;

  void setColor(COLORS color);

  OWNER getOwner() const;

  void setOwner(OWNER owner);

  void addNeighbor(Hex* hex);

  bool gotVisited() const;

  void setVisited();

  void resetVisited();

  void occupySameColorNieghbors(OWNER newOwner, COLORS NewColor);

  int countSameColorNeighbors(COLORS myColor);

  void drawHex(sf::RenderWindow* target);
};