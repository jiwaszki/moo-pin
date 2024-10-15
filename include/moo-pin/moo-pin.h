#ifndef MOO_PIN_H
#define MOO_PIN_H

#include "moo-pin/board_manager.h"
#include "moo-pin/pin.h"
// Add headers here

namespace moo {

// Alias for the existing create_board function
// TODO: think about good name for it...
// use_board / create_board / make_board / new_board / init_board
constexpr auto use_board = &BoardManager::create_board;

} // namespace moo

#endif // MOO_PIN_H
