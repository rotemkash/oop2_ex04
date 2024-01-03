#include "Computer.h"

Computer::Computer() : m_state(std::make_unique<LocalGreedyState>()) {}

/* set the State of the COmputer according to the player's choice*/
void Computer::setState(int i) {
  switch (i) {
    case 1:
      m_state.reset(new RandomState());
      break;
    case 2:
      m_state.reset(new LocalGreedyState());
      break;
    case 3:
      m_state.reset(new GlobalGreedyState());
      break;
    default:
      break;
  }
}

/*returns the computer's choice accoring to the State chosen by the player*/
COLORS Computer::getComputerChoice(Hex *playerHex, COLORS otherColor) {
  return m_state->getChoice(playerHex, otherColor);
}
