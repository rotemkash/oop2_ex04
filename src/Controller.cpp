#include "Controller.h"

#include <stdlib.h>

Controller::Controller()
    : m_window(
          sf::VideoMode(SCREEN_SIZE, SCREEN_SIZE + TOP_HEIGHT + BOTTOM_HEIGHT),
          "6-Colors", sf::Style::Close | sf::Style::Titlebar) {
  initGame();
}

/*ints the games needed variables*/
void Controller::initGame() {
  m_graph = Graph(ROWS, SCREEN_SIZE);
  m_gameState = RUNNING;
  m_menu.resetPercentage();

  m_humanCell.row = ROWS - 1;
  m_compCell.col = ROWS - 2;

  m_humanHex =
      m_graph.p2Player(m_humanCell.row, m_humanCell.col, m_humanCell.symbol);
  m_computerHex =
      m_graph.p2Player(m_compCell.row, m_compCell.col, m_compCell.symbol);
}

/*this function is called when a click is done and calls the function needed to
 * handle the click according to the game state*/
void Controller::handleClick(const sf::Event::MouseButtonEvent &event) {
  auto clickCoords = m_window.mapPixelToCoords({event.x, event.y});

  // if we are in the ne game menu
  if (m_newGame) {
    int i = m_menu.chooseComputerPlayer(&m_window, event);
    if (i > -1) {
      m_computer.setState(i);
      m_newGame = false;
    }

  }
  // otherwise we are in game and we need to check if the player chose a color
  // or clicked one of the top menu buttons
  else {
    int i;
    if (m_gameState == RUNNING) {
      i = m_menu.chooseColor(&m_window, event);
      if (-1 < i) {
        auto color = COLORS(i);
        if (color != m_humanHex->getColor() &&
            color != m_computerHex->getColor()) {
          m_graph.BFSAndColor(m_humanHex, color);

          float tmpPercent = m_graph.getPlayerPercent(m_humanHex->getOwner());
          m_menu.updatePercentage(tmpPercent, m_humanHex->getOwner());
          if (50.f < tmpPercent) {
            m_gameState = WON;
            return;
          }
          auto compChoice = m_computer.getComputerChoice(
              m_computerHex, m_humanHex->getColor());
          m_graph.resetAllVisited();

          m_graph.BFSAndColor(m_computerHex, compChoice);
          tmpPercent = m_graph.getPlayerPercent(m_computerHex->getOwner());
          m_menu.updatePercentage(tmpPercent, m_computerHex->getOwner());
          if (50.f < tmpPercent) {
            m_gameState = LOST;
            return;
          }
        }
      }
    }

    // check if the user clicked one of the top menu buttons
    i = m_menu.checkTopMenuClick(&m_window, event);
    if (i == 1) {
      initGame();
      m_newGame = true;
    } else if (i == 2) {
      m_window.close();
    }
  }
}

/*this function runs the game loop*/
void Controller::run() {
  int newColor = 0;
  COLORS compChoice;
  int turn = 0;

  while (m_window.isOpen()) {
    if (auto event = sf::Event{}; m_window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          m_window.close();
          break;
        case sf::Event::MouseButtonReleased:
          handleClick(event.mouseButton);
          break;
        case sf::Event::MouseMoved:
          m_menu.checkHover(&m_window, event.mouseMove);
          break;
        default:
          break;
      }
    }

    m_window.clear(sf::Color(128, 128, 128));

    if (m_newGame) {
      m_menu.drawMainMenu(&m_window);

      // ask user for input
    } else {
      // update the color of the avalible buttons
      m_menu.blackOutColors(m_humanHex->getColor(), m_computerHex->getColor());

      m_graph.DrawGraph(&m_window);
      m_menu.drawButtons(&m_window);
    }
    if (m_gameState != RUNNING) {
      m_menu.drawEndMsg(m_gameState, &m_window);
    }

    m_window.display();
  }
}
