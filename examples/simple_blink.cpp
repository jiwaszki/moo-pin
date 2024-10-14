#include <iostream>
#include <unistd.h>

#include <moo-pin/board_manager.h> // think about changing to "moo.h" like "pybind11" that will encapsulates all important commands

// Main function to demonstrate blinking an LED on a specified GPIO pin
int main() {
    // Initialize the Board object
    auto board = moo::BoardManager::create_board(moo::BoardType::MILKV_DUO_64);

    // Set the pin as an output
    // (pin export/initalization happens under the hood)
    // pin_at or get_pin?
    board->get_pin(440)->set_mode(moo::PinMode::OUTPUT);

    // Blink the pin a specified number of times
    int blink_count = 5;

    for (int i = 0; i < blink_count; ++i) {
        board->get_pin(440)->write(moo::PinState::HIGH);
        usleep(500000); // Sleep for 500ms
        board->get_pin(440)->write(moo::PinState::LOW);
        usleep(500000); // Sleep for 500ms
    }

    // It would also happen automatically if not mentioned.
    // Board class destructor will manage it.
    board->release_pin(440);

    return 0;
}
