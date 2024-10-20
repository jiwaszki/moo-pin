#include <moo-pin/moo-pin.h>

// Dimensions of the display (adjust to the connected screen):
#define COLUMNS 128
#define LINES 32

int main() {
    // Note: To add new lines with "\\n" due to implementation.
    // Note: Require to add spaces for some reason on some displays...
    char msg[LINES * COLUMNS] = " Hello MilkV!\\n This is new line...\\n";
    std::string msg_str = " Hello MilkV!\\n This is new line...\\n";

    // Initialize the Board object:
    auto board = moo::use_board(moo::BoardType::MILKV_DUO_64);

    /////////////////////////////////////////////////
    // Example of manual use of the display interface
    /////////////////////////////////////////////////

    // auto display = std::make_shared<moo::SSD1306>(1, board->get_config()->i2c_base_path);

    // display->default_config(LINES, COLUMNS);
    // display->load_resolution();
    // display->clear_screen();
    // // Reset the cursor position
    // // NOTE: Clearing the screen moves the cursor!
    // display->set_XY(0, 0);

    // display->write_string(0, &msg[0]);

    // display->close();

    /////////////////////////////////////////////
    // User-friendly use of the display interface
    /////////////////////////////////////////////

    // that should do all of defult config to set_xy
    // use_display or get_display or connect_display?
    auto display = board->get_display<moo::SSD1306>(LINES, COLUMNS, 1);

    // display.set_cursor()
    // display.set_font(moo::FontType::SMALL); // or LARGE

    // Alternative way of using it:
    // display->write(&msg[0]);
    // There is no need to add x and y, this is just for demo.
    // NOTE: It will automatically start from (0,0).
    display->write(msg_str, 20, 1);
    
    // To clear the board use:
    // display->clear();

    display->close();

    return 0;
}
