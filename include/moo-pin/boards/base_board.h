#ifndef BASE_BOARD_H
#define BASE_BOARD_H

#include <memory>
#include <unordered_map>
#include <string>
#include <fstream>
#include <stdexcept>

#include "moo-pin/configs/base_config.h"
#include "moo-pin/displays/display.h"
#include "moo-pin/pin.h"
#include "moo-pin/i2c.h"

namespace moo {

class BaseBoard {
public:
    BaseBoard(const std::string& board_name, BaseConfig* config)
        : board_name(board_name), config(config) {}

    BaseConfig* get_config() {
        return config;
    }

    // TODO:
    // Add connected displays as a way to store them and
    // disconnect when board is closing as well
    template <typename T, typename... Args>
    std::shared_ptr<Display<T>> get_display(Args&&... args) {
        return std::make_shared<Display<T>>(std::forward<Args>(args)..., config->i2c_base_path);
    }

    std::shared_ptr<Pin> get_pin(int pin_number) {
        if (pins.find(pin_number) == pins.end()) {
            // If not found, export pin first:
            export_pin(pin_number);
            // Create a Pin object and store it in the pins map
            pins[pin_number] = std::make_shared<Pin>(pin_number, config->gpio_base_path);
        }
        return pins[pin_number];
    }

    std::shared_ptr<I2C> get_i2c(int i2c_number, int dev_address) {
        if (i2cs.find(i2c_number) == i2cs.end()) {
            // Create a I2C object and store it in the i2cs map
            i2cs[i2c_number] = std::make_shared<I2C>(i2c_number, dev_address, config->i2c_base_path);
        }
        return i2cs[i2c_number];
    }

    void release_pin(int pin_number) {
        if (pins.find(pin_number) != pins.end()) {
            unexport_pin(pin_number);
            pins.erase(pin_number);
        }
    }

    void release_i2c(int i2c_number) {
        if (i2cs.find(i2c_number) != i2cs.end()) {
            i2cs[i2c_number]->close();
            i2cs.erase(i2c_number);
        }
    }

    ~BaseBoard() {
        for (auto &pair : pins) {
            unexport_pin(pair.first);
        }
        for (auto &pair : i2cs) {
            i2cs[pair.first]->close();
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
    std::unordered_map<int, std::shared_ptr<I2C>> i2cs;
};

} // namespace moo

#endif // BASE_BOARD_H
