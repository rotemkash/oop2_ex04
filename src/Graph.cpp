#include "Graph.h"

Graph::Graph(int rows, int screenSize)
    : m_rows(rows), m_screenSize(screenSize) {
  // resize the rows/cols
  m_board.resize(m_rows);
  for (int row = 0; row < m_rows; row++) {
    m_board[row].resize(m_rows);
  }

  float radius = (float(screenSize) / float(m_rows - 1));

  // color cells
  for (int row = 0; row < m_rows; row++) {
    for (int col = 0; col < m_rows; col++) {
      if (col == m_rows - 1) {
        if (row % 2 == 0) {
          m_board[row][col] = new Hex(row, col, COLORS::NA, radius);
          continue;
        }
      }
      m_board[row][col] = new Hex(row, col, COLORS(rand() % 6), radius);
    }
  }

  createNeighbors();
}

/* goes over the cells and creates a neighbors list for each hxagonal*/
void Graph::createNeighbors() {
  for (int row = 0; row < m_rows; row++) {
    for (int col = 0; col < m_rows; col++) {
      // go around the cell to find the neighbors
      for (int rowShift = -1; rowShift <= 1; rowShift++) {
        for (int colShift = -1; colShift <= 1; colShift++) {
          // if it's the cell itself
          if (rowShift == 0 && colShift == 0) {
            continue;
          }
          // since we shift the rows there's two cells that are not neighbors
          // anymore
          if (row % 2 == 0) {
            if ((rowShift == -1 && colShift == -1) ||
                (rowShift == 1 && colShift == -1)) {
              continue;
            }
          } else {
            if ((rowShift == -1 && colShift == 1) ||
                (rowShift == 1 && colShift == 1)) {
              continue;
            }
          }
          if (cellIsValid(row + rowShift, col + colShift, m_rows, m_rows)) {
            if (m_board[row + rowShift][col + colShift]->getColor() !=
                COLORS::NA) {
              m_board[row][col]->addNeighbor(
                  m_board[row + rowShift][col + colShift]);
            }
          }
        }
      }
    }
  }
}

/*receives a starting point and a row/col shift values
and it checks if the shift done is still within bounds*/
bool Graph::cellIsValid(int row, int col, int rowsCount, int colCount) const {
  return row >= 0 && row < rowsCount && col >= 0 && col < colCount;
}

/* starts from the given cell and starts to traverse using BFS algorithm and
while traversing it changes the old colors to the new and ocuupies the new cells
that have the new color*/
void Graph::BFSAndColor(Hex *hex, COLORS newColor) {
  std::queue<Hex *> queue;

  int startColor = hex->getColor();
  OWNER startOwner = hex->getOwner();

  queue.push(hex);
  hex->setVisited();
  hex->setColor(COLORS(newColor));
  Hex *currHex;

  while (!queue.empty()) {
    currHex = queue.front();
    if (currHex->getOwner() == OWNER::FREE) {
      currHex->occupySameColorNieghbors(startOwner, newColor);
    }
    queue.pop();

    for (auto adjTile = currHex->getNeighbors().begin();
         adjTile != currHex->getNeighbors().end(); adjTile++) {
      if (!(*adjTile)->gotVisited()) {
        (*adjTile)->setVisited();

        if ((*adjTile)->getOwner() == startOwner) {
          (*adjTile)->setColor(newColor);
          queue.push(*adjTile);

        } else if ((*adjTile)->getOwner() == OWNER::FREE &&
                   (*adjTile)->getColor() == newColor) {
          queue.push(*adjTile);
        }
      }
    }
  }

  for (int row = 0; row < m_rows; row++) {
    for (int col = 0; col < m_rows; col++) {
      m_board[row][col]->resetVisited();
    }
  }
}

/* returns a pointer to the cell*/
Hex *Graph::p2Player(int row, int col, OWNER owner) const {
  m_board[row][col]->setOwner(owner);
  return m_board[row][col];
}

/* Draws the graph onto the screen*/
void Graph::DrawGraph(sf::RenderWindow *window) {
  for (int row = 0; row < m_rows; row++) {
    for (int col = 0; col < m_board[row].size(); col++) {
      if (col == m_rows - 1) {
        if (row % 2 == 0) {
          continue;
        }
      }

      m_board[row][col]->drawHex(window);
    }
  }
}

/* resets all the cells to be not visited*/
void Graph::resetAllVisited() const {
  for (int row = 0; row < m_rows; row++) {
    for (int col = 0; col < m_rows; col++) {
      m_board[row][col]->resetVisited();
    }
  }
}

/*given an owner it calculates how much that player has ocuppied from the
 * board*/
float Graph::getPlayerPercent(OWNER owner) const {
  // since the board isn't semitrical there's some cells that we don't have to
  // count
  int activeCells = m_rows * m_rows - (m_rows / 2);
  float count = 0;
  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_rows; j++) {
      if (m_board[i][j]->getOwner() == owner) {
        ++count;
      }
    }
  }

  return (count / activeCells) * 100.f;
}