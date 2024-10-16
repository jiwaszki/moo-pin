#include <moo-pin/moo-pin.h>

int main() {
    // Initialize the Board object
    auto board = moo::use_board(moo::BoardType::MILKV_DUO_64);

    auto display = std::make_shared<moo::SSD1306>(1, board->get_config()->i2c_base_path);

    display->default_config(32, 128);
    display->load_resolution();
    display->clear_screen();
    // Reset the cursor position
    // NOTE: Clearing the screen moves the cursor!
    display->set_XY(0, 0);

    // msg[200]?
    // Need to add new lines with "\\n" due to implementation
    // Require to add spaces for some reason on some displays
    char msg[200] = " abc\\n yo"; // "Hello MilkV\nIt is me jiwszki\n";
    display->write_string(0, &msg[0]);

    display->close();

    return 0;
}
