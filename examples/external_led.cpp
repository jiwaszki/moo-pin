#include <unistd.h>
#include <moo-pin/moo-pin.h>

#define POWER_PIN 495

// Main function to demonstrate blinking an external LED
// on a specified GPIO pin connected
int main() {
    // Initialize the Board object
    auto board = moo::use_board(moo::BoardType::MILKV_DUO_64);

    // Set the pin as an output
    // (pin export/initalization happens under the hood)
    // pin_at or get_pin?
    board->get_pin(POWER_PIN)->set_mode(moo::PinMode::OUTPUT);

    // Blink the pin forever
    while (true) {
        board->get_pin(POWER_PIN)->write(moo::PinState::HIGH);
        usleep(1000000); // Sleep for 1000ms
        board->get_pin(POWER_PIN)->write(moo::PinState::LOW);
        usleep(1000000); // Sleep for 1000ms
    }

    // Board class destructor will manage it.
    // board->release_pin(POWER_PIN);

    return 0;
}
