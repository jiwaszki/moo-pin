#ifndef BOARD_MANAGER_H
#define BOARD_MANAGER_H

#include <memory>

#include "moo-pin/boards/base_board.h"
#include "moo-pin/boards/milkv_duo_64_board.h"

namespace moo {

enum class BoardType {
    MILKV_DUO_64,
    // Add more board types here.
};

class BoardManager {
  public:
    static std::unique_ptr<BaseBoard> create_board(const BoardType board_type) {
        if (board_type == BoardType::MILKV_DUO_64) {
            return std::make_unique<MilkVDuo64Board>();
        } else {
            throw std::invalid_argument("Unknown board type");
        }
    }
};

} // namespace moo

#endif // BOARD_MANAGER_H
