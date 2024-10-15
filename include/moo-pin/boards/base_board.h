#ifndef BASE_BOARD_H
#define BASE_BOARD_H

#include <memory>
#include <unordered_map>
#include <string>
#include <fstream>
#include <stdexcept>

#include "moo-pin/configs/base_config.h"
#include "moo-pin/pin.h"

namespace moo {

class BaseBoard {
public:
    BaseBoard(const std::string& board_name, BaseConfig* config)
        : board_name(board_name), config(config) {}

    std::shared_ptr<Pin> get_pin(int pin_number) {
        if (pins.find(pin_number) == pins.end()) {
            // If not found, export pin first:
            export_pin(pin_number);
            // Construct the paths for the GPIO pin
            std::string direction_path = config->gpio_base_path + std::to_string(pin_number) + "/direction";
            std::string value_path = config->gpio_base_path + std::to_string(pin_number) + "/value";
            // Create a Pin object and store it in the pins map
            pins[pin_number] = std::make_shared<Pin>(pin_number, direction_path, value_path);
        }
        return pins[pin_number];
    }

    void release_pin(int pin_number) {
        unexport_pin(pin_number);
        if (pins.find(pin_number) != pins.end()) {
            pins.erase(pin_number);
        }
    }

    ~BaseBoard() {
        for (auto &pair : pins) {
            unexport_pin(pair.first);
        }
    }

protected:
    virtual void export_pin(int pin_number) {
        // Export the pin number
        std::ofstream export_file(config->gpio_export_path);
        if (!export_file) {
            throw std::runtime_error("Failed to open GPIO export file");
        }
        export_file << pin_number;
        export_file.close();
    }

    virtual void unexport_pin(int pin_number) {
        // Unexport the pin number
        std::ofstream unexport_file(config->gpio_unexport_path);
        if (!unexport_file) {
            throw std::runtime_error("Failed to open GPIO unexport file");
        }
        unexport_file << pin_number;
        unexport_file.close();
    }

    std::string board_name; // Name of the board TODO?
    BaseConfig* config; // Pointer to configuration object
    std::unordered_map<int, std::shared_ptr<Pin>> pins;
};

} // namespace moo

#endif // BASE_BOARD_H
