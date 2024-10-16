#ifndef MILK_V_DUO_64_CONFIG_H
#define MILK_V_DUO_64_CONFIG_H

#include "base_config.h"
// #include <optional>

namespace moo {

class MilkVDuo64Config : public BaseConfig {
public:
    MilkVDuo64Config()
        : BaseConfig("/sys/class/gpio/gpio",
                     "/sys/class/gpio/export",
                     "/sys/class/gpio/unexport",
                     "/dev/i2c-") {}
          // , group_a_start(1234) {} // Specific to Milk-V Duo 64

    // std::optional<int> group_a_start; // Optional parameter for specific boards
};

} // namespace moo

#endif // MILK_V_DUO_64_CONFIG_H
