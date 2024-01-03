#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

#include "Hex.h"

const int RUNNING = 0;
const int WON = 1;
const int LOST = 2;

class Menu {
 public:
  Menu();

  void checkHover(sf::RenderWindow *window,
                  const sf::Event::MouseMoveEvent &event);

  int chooseComputerPlayer(sf::RenderWindow *window,
                           const sf::Event::MouseButtonEvent &event);

  int chooseColor(sf::RenderWindow *window,
                  const sf::Event::MouseButtonEvent &event);

  void drawMainMenu(sf::RenderWindow *window);

  void blackOutColors(COLORS color1, COLORS color2);

  void drawButtons(sf::RenderWindow *window);

  void resetPercentage();

  int checkTopMenuClick(sf::RenderWindow *window,
                        const sf::Event::MouseButtonEvent &event);

  void updatePercentage(float precent, OWNER owner);

  void drawEndMsg(int state, sf::RenderWindow *window);

 private:
  void initMenu();
  void initNewGameMenu();

  sf::Font m_font;
  //--------Main-Menu-------
  sf::Text m_ChooseTxt;
  sf::Text m_randomlTxt;
  sf::Text m_localGreadyTxt;
  sf::Text m_globalGreadyTxt;
  std::vector<sf::Text *> m_mainMenuTexts = {&m_randomlTxt, &m_localGreadyTxt,
                                             &m_globalGreadyTxt};

  //-----Game-SHapes------
  sf::Text m_newGameButton;
  sf::Text m_exitButton;
  sf::Text m_humanPercentage;
  sf::Text m_computerPercentage;

  sf::RectangleShape m_topMenu;
  sf::RectangleShape m_bottomMenu;
  std::vector<sf::RectangleShape> m_colorBtns;

  //----------END-SCREEN--------
  sf::Text m_endMsg;
};