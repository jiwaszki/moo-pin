#include <moo-pin/moo-pin.h>

// Dimensions of the display:
#define COLUMNS 128
#define LINES 32

int main() {
    // Initialize the Board object
    auto board = moo::use_board(moo::BoardType::MILKV_DUO_64);

    auto display = std::make_shared<moo::SSD1306>(1, board->get_config()->i2c_base_path);

    display->default_config(LINES, COLUMNS);
    display->load_resolution();
    display->clear_screen();
    // Reset the cursor position
    // NOTE: Clearing the screen moves the cursor!
    display->set_XY(0, 0);

    // Note: To add new lines with "\\n" due to implementation.
    // Note: Require to add spaces for some reason on some displays...
    char msg[LINES * COLUMNS] = " Hello MilkV!\\n This is new line...\\n";
    display->write_string(0, &msg[0]);

    display->close();

    return 0;
}
