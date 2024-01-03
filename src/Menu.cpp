#include "Menu.h"

Menu::Menu() {
  m_font.loadFromFile("C:/Windows/Fonts/Arial.ttf");
  initNewGameMenu();
  initMenu();
}

/*checks if the mouse is hovering over a text and changes it color*/
void Menu::checkHover(sf::RenderWindow *window,
                      const sf::Event::MouseMoveEvent &event) {
  auto clickCoords = window->mapPixelToCoords({event.x, event.y});

  for (int i = 0; i < m_mainMenuTexts.size(); i++) {
    if (m_mainMenuTexts[i]->getGlobalBounds().contains(clickCoords)) {
      m_mainMenuTexts[i]->setFillColor(sf::Color::Red);
    } else {
      m_mainMenuTexts[i]->setFillColor(sf::Color::White);
    }
  }
  if (m_newGameButton.getGlobalBounds().contains(clickCoords)) {
    m_newGameButton.setFillColor(sf::Color::Red);
  } else {
    m_newGameButton.setFillColor(sf::Color::White);
  }

  if (m_exitButton.getGlobalBounds().contains(clickCoords)) {
    m_exitButton.setFillColor(sf::Color::Red);
  } else {
    m_exitButton.setFillColor(sf::Color::White);
  }
}

/* chooses a computer player according to the player's choice*/
int Menu::chooseComputerPlayer(sf::RenderWindow *window,
                               const sf::Event::MouseButtonEvent &event) {
  auto clickCoords = window->mapPixelToCoords({event.x, event.y});

  for (int i = 0; i < m_mainMenuTexts.size(); i++) {
    if (m_mainMenuTexts[i]->getGlobalBounds().contains(clickCoords)) {
      return i;
    }
  }
  return -1;  // no button was clicked
}

/* checks if the player clicked the colors button on the bottom*/
int Menu::chooseColor(sf::RenderWindow *window,
                      const sf::Event::MouseButtonEvent &event) {
  auto clickCoords = window->mapPixelToCoords({event.x, event.y});

  for (int i = 0; i < m_colorBtns.size(); i++) {
    if (m_colorBtns[i].getGlobalBounds().contains(clickCoords)) {
      return i;
    }
  }
  return -1;  // no button was clicked
}

/* draws the menu onto the screen*/
void Menu::drawMainMenu(sf::RenderWindow *window) {
  window->draw(m_ChooseTxt);
  window->draw(m_randomlTxt);
  window->draw(m_localGreadyTxt);
  window->draw(m_globalGreadyTxt);
}

///* changes the buttons that are not availib;e this round to black*/
void Menu::blackOutColors(COLORS color1, COLORS color2) {
  for (int i = 0; i < m_colorBtns.size(); i++) {
    if (color1 == COLORS(i) || color2 == COLORS(i)) {
      m_colorBtns[i].setFillColor(sf::Color::Black);
    } else {
      m_colorBtns[i].setFillColor(m_sfColors[i]);
    }
  }
}

/* draws the bottom buttons ontp the screen*/
void Menu::drawButtons(sf::RenderWindow *window) {
  window->draw(m_topMenu);

  window->draw(m_bottomMenu);
  for (auto i = m_colorBtns.begin(); i != m_colorBtns.end(); i++) {
    window->draw(*i);
  }
  window->draw(m_newGameButton);
  window->draw(m_exitButton);
  window->draw(m_humanPercentage);
  window->draw(m_computerPercentage);
}

/* resets the percentage of the players to 0%*/
void Menu::resetPercentage() {
  m_humanPercentage.setString("0.000%");
  m_computerPercentage.setString("0.000%");
}

/* checks if the player clicked on the top menu*/
int Menu::checkTopMenuClick(sf::RenderWindow *window,
                            const sf::Event::MouseButtonEvent &event) {
  auto clickCoords = window->mapPixelToCoords({event.x, event.y});
  if (m_newGameButton.getGlobalBounds().contains(clickCoords)) {
    return 1;
  }
  if (m_exitButton.getGlobalBounds().contains(clickCoords)) {
    return 2;
  }
  return -1;
}

/* updates the percentage of the given player*/
void Menu::updatePercentage(float percent, OWNER owner) {
  float roundedPercent = std::roundf(percent * 100) / 100;
  switch (owner) {
    case HUMAN:
      m_humanPercentage.setString(std::to_string(roundedPercent) + "%");
      break;
    case PC:
      m_computerPercentage.setString(std::to_string(roundedPercent) + "%");
      break;
    default:
      break;
  }
}

/* displays a message on the screen when one of the players wins*/
void Menu::drawEndMsg(int state, sf::RenderWindow *window) {
  switch (state) {
    case WON:
      m_endMsg.setString(
          "     Congrats you won     \n"
          "choose from the menu above");
      break;
    case LOST:
      m_endMsg.setString(
          "     you've lost     \n"
          "choose from the menu above");
      break;
    default:
      break;
  }
  window->draw(m_endMsg);
}

/*init all the shapes and texts of the game*/
void Menu::initMenu() {
  m_topMenu.setSize(sf::Vector2f(SCREEN_SIZE, TOP_HEIGHT));
  m_topMenu.setPosition(0.f, 0.f);
  m_topMenu.setFillColor(sf::Color(128, 128, 128));

  m_bottomMenu.setSize(sf::Vector2f(SCREEN_SIZE, BOTTOM_HEIGHT));
  m_bottomMenu.setPosition(0.f, SCREEN_SIZE + TOP_HEIGHT);
  m_bottomMenu.setFillColor(sf::Color(128, 128, 128));

  for (float i = 0; i < m_sfColors.size() - 1; i++) {
    auto tmp = sf::RectangleShape(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    tmp.setFillColor(m_sfColors[i]);
    tmp.setPosition(250.f + i * (BUTTON_SIZE + BUTTON_PADDING),
                    SCREEN_SIZE + TOP_HEIGHT + 10.f);
    m_colorBtns.push_back(tmp);
  }

  m_newGameButton = sf::Text("New Game", m_font, 20);
  m_newGameButton.setPosition(100.f, 0.f);

  m_exitButton = sf::Text("Exit", m_font, 20);
  m_exitButton.setPosition(600.f, 0.f);

  m_humanPercentage = sf::Text("0.000%", m_font, 20);
  m_humanPercentage.setPosition(50.f, SCREEN_SIZE + TOP_HEIGHT + 10.f);

  m_computerPercentage = sf::Text("0.000%", m_font, 20);
  m_computerPercentage.setPosition(650.f, SCREEN_SIZE + TOP_HEIGHT + 10.f);
}

/*inits the texts of the Main Menu screen*/
void Menu::initNewGameMenu() {
  m_ChooseTxt = sf::Text("!! Choose opponent Type !!", m_font, 40);
  m_ChooseTxt.setStyle(sf::Text::Bold | sf::Text::Underlined);
  m_ChooseTxt.setPosition(150.f, 150.f);

  m_randomlTxt = sf::Text("Random opponent", m_font, 30);
  m_randomlTxt.setPosition(230.f, 300.f);

  m_localGreadyTxt = sf::Text("Local gready opponent", m_font, 30);
  m_localGreadyTxt.setPosition(210.f, 400.f);

  m_globalGreadyTxt = sf::Text("Global Gready opponent", m_font, 30);
  m_globalGreadyTxt.setPosition(200.f, 500.f);

  m_endMsg = sf::Text("", m_font, 50);
  m_endMsg.setPosition(50.f, 300.f);
  m_endMsg.setStyle(sf::Text::Bold | sf::Text::Underlined);
  m_endMsg.setOutlineColor(sf::Color::Black);
  m_endMsg.setOutlineThickness(5.f);
}
