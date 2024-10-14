#ifndef MILK_V_DUO_64_BOARD_H
#define MILK_V_DUO_64_BOARD_H

#include "base_board.h"
#include "moo-pin/configs/milkv_duo_64_config.h"

namespace moo {

class MilkVDuo64Board : public BaseBoard {
public:
    MilkVDuo64Board()
        : BaseBoard("MILKV_DUO_64", new MilkVDuo64Config()) {}
    
    // You can override methods or add specific functionalities here
};

} // namespace moo

#endif // MILK_V_DUO_64_BOARD_H
