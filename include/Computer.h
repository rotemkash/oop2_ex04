#pragma once

#include <memory>
#include <queue>

#include "Hex.h"

class State {
 public:
  virtual COLORS getChoice(Hex *playerHex, COLORS otherColor) = 0;
  virtual ~State() = default;
};

class RandomState : public State {
 public:
  COLORS getChoice(Hex *playerHex, COLORS otherColor) override {
    COLORS randColor = COLORS(rand() % 6);

    while (randColor == playerHex->getColor() || randColor == otherColor) {
      randColor = COLORS(rand() % 6);
    }

    return randColor;
  }
};

class LocalGreedyState : public State {
 public:
  COLORS getChoice(Hex *playerHex, COLORS otherColor) override {
    std::queue<Hex *> queue;

    std::vector<int> colorCount = {0, 0, 0, 0, 0, 0};

    int myColor = playerHex->getColor();
    OWNER myOwner = playerHex->getOwner();

    queue.push(playerHex);
    playerHex->setVisited();
    Hex *currHex;

    while (!queue.empty()) {
      currHex = queue.front();

      for (auto it = currHex->getNeighbors().begin();
           it != currHex->getNeighbors().end(); it++) {
        if (!(*it)->gotVisited()) {
          if ((*it)->getOwner() == OWNER::FREE &&
              (*it)->getColor() != otherColor) {
            (*it)->setVisited();
            colorCount[int((*it)->getColor())] += 1;
          }
        }
      }

      queue.pop();

      for (auto firstIt = currHex->getNeighbors().begin();
           firstIt != currHex->getNeighbors().end(); firstIt++) {
        if (!(*firstIt)->gotVisited()) {
          (*firstIt)->setVisited();
          if ((*firstIt)->getOwner() == myOwner) {

            queue.push(*firstIt);
          }
        }
      }
    }

    int maxVal = -1, index = 0;
    std::cout << "\n";
    for (int i = 0; i < colorCount.size(); i++) {
      if (colorCount[i] > maxVal && i != int(otherColor) && i != int(myColor)) {
        maxVal = colorCount[i];
        index = i;
      }
    }

    return COLORS(index);
  }
};

class GlobalGreedyState : public State {
 public:
  COLORS getChoice(Hex *playerHex, COLORS otherColor) override {
    std::queue<Hex *> queue;

    std::vector<int> colorCount = {0, 0, 0, 0, 0, 0};

    int myColor = playerHex->getColor();
    OWNER myOwner = playerHex->getOwner();

    queue.push(playerHex);
    playerHex->setVisited();
    Hex *currHex;

    while (!queue.empty()) {
      currHex = queue.front();

      if (currHex->getOwner() == OWNER::FREE) {
        colorCount[int(currHex->getColor())] +=
            currHex->countSameColorNeighbors(currHex->getColor());
      }

      queue.pop();

      for (auto firstIt = currHex->getNeighbors().begin();
           firstIt != currHex->getNeighbors().end(); firstIt++) {
        if (!(*firstIt)->gotVisited()) {
          (*firstIt)->setVisited();
          if ((*firstIt)->getOwner() == myOwner) {
            queue.push(*firstIt);
          } else if ((*firstIt)->getOwner() == OWNER::FREE &&
                     (*firstIt)->getColor() != otherColor) {
            queue.push(*firstIt);
          }
        }
      }
    }

    int maxVal = -1, index = 0;
    for (int i = 0; i < colorCount.size(); i++) {
      if (colorCount[i] > maxVal && i != int(otherColor) && i != int(myColor)) {
        maxVal = colorCount[i];
        index = i;
      }
    }
    return COLORS(index);
  }
};

class Computer {
 private:
  std::unique_ptr<State> m_state;

 public:
  Computer();
  void setState(int i);

  COLORS getComputerChoice(Hex *playerHex, COLORS otherColor);
};
