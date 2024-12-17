#include <chrono>
#include <thread>

#include <moo-pin/moo-pin.h>

// Dimensions of the display (adjust to the connected screen):
#define COLUMNS 128
#define LINES 32

int main() {
    // NOTE: To add new lines with "\\n" due to implementation.
    // NOTE: Required to add spaces on some displays...
    std::string first_msg = " Hello MilkV!\\n This is new line...\\n";

    // Initialize the Board object:
    auto board = moo::use_board(moo::BoardType::MILKV_DUO_64);

    /////////////////////////////////////////////////
    // Example of manual use of the display interface
    /////////////////////////////////////////////////

    // auto display = std::make_shared<moo::SSD1306>(1,
    // board->get_config()->i2c_base_path);

    // display->default_config(LINES, COLUMNS);
    // display->load_resolution();
    // display->clear_screen();
    // // Reset the cursor position
    // // NOTE: Clearing the screen moves the cursor!
    // display->set_XY(0, 0);

    // char msg[LINES * COLUMNS] = " Hello MilkV!\\n This is new line...\\n";
    // display->write_string(0, &msg[0]);

    // display->close();

    /////////////////////////////////////////////
    // User-friendly use of the display interface
    /////////////////////////////////////////////

    // that should do all of defult config to set_xy
    // use_display or get_display or connect_display?
    auto display = board->get_display<moo::SSD1306>(LINES, COLUMNS, 1);

    // There is no need to add x and y, this is just for demo.
    // NOTE: It will automatically start from (0,0).
    // To clear the board use:
    display->clear();
    // Write first message:
    display->write(first_msg, 20, 1);

    // Showcase the Display on and off capabilites.
    std::this_thread::sleep_for(std::chrono::seconds(3));
    display->off();

    std::this_thread::sleep_for(std::chrono::seconds(3));
    display->on();
    display->clear();
    std::string second_msg = " Goodbye MilkV!";
    display->write(second_msg, 20, 1);

    std::this_thread::sleep_for(std::chrono::seconds(3));
    // NOTE: display will stay turned on with the last message
    //       if this is not required explicitly. 
    // To manually clean-up, turn display off and close connection:
    // display->clear();
    // display->off();
    // display->close();
    // Or use the short version:
    display->close(true);

    return 0;
}
