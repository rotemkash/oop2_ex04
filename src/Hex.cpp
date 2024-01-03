#include "Hex.h"

/*
 * Constructor
 */
Hex::Hex(int row, int col, COLORS color, float diameter)
    : m_row(row), m_col(col), m_color(color), m_radius(diameter) {
  // Calculate the size and position of the hexagonal shape according to the
  // diameter given
  float radius = (diameter / 2.f) * (2.f / std::sqrt(3.f));
  float pos_X = float(m_col) * (diameter) - (radius - diameter / 2);
  float pos_Y = float(m_row) * (radius * std::sqrt(3.f)) - (radius) + 25.f;

  // shift the odd rows to the left
  if (m_row % 2 == 1) {
    pos_X -= (diameter * 0.5);
  }

  m_shape = sf::CircleShape(radius, 6);
  m_shape.setFillColor(m_sfColors[int(m_color)]);
  m_shape.setPosition(pos_X, pos_Y);
}

/* return the color of the hexagonal*/
COLORS Hex::getColor() const { return m_color; }

/*change the color of the hexagonal shape to the new color*/
void Hex::setColor(COLORS color) {
  m_color = color;

  m_shape.setFillColor(m_sfColors[int(m_color)]);
}

/* return the OWNER of the Hexagonal cell*/
OWNER Hex::getOwner() const { return m_owner; }

/* set the owner*/
void Hex::setOwner(OWNER owner) { m_owner = owner; }

/* add a hexagonal shape to the neighbors list*/
void Hex::addNeighbor(Hex* hex) { m_neighbors.push_back(hex); }

/* return the neighbors list*/
const std::vector<Hex*>& Hex::getNeighbors() const { return m_neighbors; }

/* check whether we visited this cell or not*/
bool Hex::gotVisited() const { return m_visited; }

/* mark the hexagonal as visited*/
void Hex::setVisited() { m_visited = true; }

/* reset the cell to be not visited*/
void Hex::resetVisited() { m_visited = false; }

/*
 * travel on the hexagonal shapes next to this cell and change their owner
 * since we don't have direct access to the hexagonal shapes not directly next
 * to this we travel through the neighbors list and their neighbors to find
 * cells with the same color
 */
void Hex::occupySameColorNieghbors(OWNER newOwner, COLORS NewColor) {
  setOwner(newOwner);
  setVisited();
  for (auto adjTile = m_neighbors.begin(); adjTile < m_neighbors.end();
       adjTile++) {
    if (!(*adjTile)->gotVisited() && (*adjTile)->getColor() == NewColor) {
      (*adjTile)->occupySameColorNieghbors(newOwner, NewColor);
    }
  }
}

/* counts the number of all the hexagonals connected to the starting hexagonal
 * shape*/
int Hex::countSameColorNeighbors(COLORS myColor) {
  int count = 1;
  setVisited();
  for (auto adjTile = m_neighbors.begin(); adjTile < m_neighbors.end();
       adjTile++) {
    if (!(*adjTile)->gotVisited() && (*adjTile)->getColor() == myColor) {
      count += (*adjTile)->countSameColorNeighbors(myColor);
    }
  }
  return count;
}

/*draw the hexagonal shape on the screen*/
void Hex::drawHex(sf::RenderWindow* target) {
  m_shape.setFillColor(m_sfColors[int(m_color)]);

  // make the hexagonals "flash" to mark the occupied ones
  if (m_clock.getElapsedTime() > sf::seconds(0.5) && m_owner != OWNER::FREE) {
    switch (m_owner) {
      case HUMAN:
        m_shape.setFillColor(sf::Color::White);
        break;
      case PC:
        m_shape.setFillColor(sf::Color::Black);
        break;
      default:
        break;
    }
  }
  if (m_clock.getElapsedTime() > sf::seconds(1.f)) {
    m_clock.restart();
  }
  target->draw(m_shape);
}
