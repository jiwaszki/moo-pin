#ifndef BOARD_MANAGER_H
#define BOARD_MANAGER_H

#include <memory>
#include <string>

#include "moo-pin/boards/base_board.h"
#include "moo-pin/boards/milkv_duo_64_board.h"

namespace moo {

class BoardManager {
public:
    static std::unique_ptr<BaseBoard> create_board(const std::string& board_type) {
        if (board_type == "MILKV_DUO_64") {
            return std::make_unique<MilkVDuo64Board>();
        } else {
            throw std::invalid_argument("Unknown board type");
        }
    }
};

} // namespace moo

#endif // BOARD_MANAGER_H
