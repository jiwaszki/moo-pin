#ifndef MO_PIN_H // MO_PIN_H to avoid conflicts with MOO_PIN_H in moo-pin.h
#define MO_PIN_H

#include <fstream>
#include <stdexcept>

namespace moo {

enum class PinMode {
    NOT_SET,
    INPUT,
    OUTPUT,
    // INTERRUPT,
    // Add more modes? Are there any?
};

enum class PinState {
    LOW,
    HIGH,
};

namespace details {
inline std::string get_mode(PinMode mode) {
    switch (mode) {
    case PinMode::NOT_SET:
        throw std::runtime_error("Pin mode cannot be unset!");
    case PinMode::INPUT:
        return "in";
    case PinMode::OUTPUT:
        return "out";
    // case PinMode::INTERRUPT:
    //     return "...";
    default:
        throw std::runtime_error("Unknown PinMode!");
    }
}

inline std::string get_state(PinState state) {
    switch (state) {
    case PinState::LOW:
        return "0";
    case PinState::HIGH:
        return "1";
    default:
        throw std::runtime_error("Unknown PinState!");
    }
}
} // namespace details

class Pin {
  public:
    Pin(int number, const std::string &gpio_base_path) : pin_number(number) {
        mode_path = gpio_base_path + std::to_string(pin_number) + "/direction";
        state_path = gpio_base_path + std::to_string(pin_number) + "/value";
    }

    void set_mode(PinMode mode) {
        // Implementation for setting the pin mode
        std::string mode_str = details::get_mode(mode);
        std::ofstream direction_file(mode_path);
        if (!direction_file) {
            throw std::runtime_error("Failed to open GPIO direction file.");
        }
        direction_file << mode_str;
        direction_file.close(); // Should destructor of ofstream handle it?
    }

    void write(PinState state) {
        // Implementation for writing to the GPIO pin
        std::ofstream state_file(state_path);
        if (!state_file.is_open()) {
            throw std::runtime_error(
                "Failed to open GPIO value file for writing.");
        }
        state_file << details::get_state(state);
        state_file.close(); // Should destructor of ofstream handle it?
    }

    int read() {
        // Implementation for reading the current value of the GPIO pin
        std::ifstream state_file(state_path);
        std::string value;
        if (!state_file.is_open()) {
            throw std::runtime_error(
                "Failed to open GPIO value file for reading.");
        }
        std::getline(state_file, value);
        state_file.close(); // Should destructor of ifstream handle it?
        return value == "1";
    }

  private:
    int pin_number;
    std::string mode_path;
    std::string state_path;
};

} // namespace moo

#endif // MO_PIN_H
