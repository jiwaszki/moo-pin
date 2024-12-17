# moo-pin
Simple C++ GPIO library designed for working with development boards like Milk-V... thus moo, get it?
Library provides a clean, modern C++ interface for controlling GPIO pins and  I2C (serial communication will come in the future!).

## Features
- **GPIO Control:** Easily read and write to GPIO pins.
- **I2C Control:** Support for communication protocols used in embedded systems.
- **Header-only Library:** Simplified integration with projects by including the headers.
- **Configurable Board Support:** Supports different board configurations (e.g. Milk-V Duo).
- **Object-Oriented Design:** Leverages C++ features such as templates, inheritance, and smart pointers for extensibility.

## Getting Started
### Prerequisites
- C++14 or newer compiler (e.g. GCC for cross-compilation).
- A Milk-V board (or any supported board).
- Linux-based system for accessing GPIO via `/sys/class/gpio`.

### Installation
Since `moo-pin` is a header-only library, you can integrate it directly into your project by including the header files. You can clone the repository and use the `include/` directory in your build system.

```bash
git clone https://github.com/jiwaszki/moo-pin.git
```

### (Cross-)Compilation

```bash
# Example of cross-compilation for Milk-V Duo 64mb boards:
riscv64-unknown-linux-gnu-g++ -march=rv64imafdcv0p7xthead -Os ./examples/ssd1306_text.cpp -I./include -o ssd1306_text -static
```

## Usage
### Basic Example
The following example demonstrates how to set up a `Board` object and control a GPIO pin (code is also available in `examples/simple_blink.cpp`):

```cpp
#include <unistd.h>

#include <moo-pin/moo-pin.h>

// Main function to demonstrate blinking an LED on a specified GPIO pin
int main() {
    // Initialize the Board object
    auto board = moo::use_board(moo::BoardType::MILKV_DUO_64);

    // Set the pin as an output
    board->get_pin(440)->set_mode(moo::PinMode::OUTPUT);

    // Blink the pin a specified number of times
    int blink_count = 5;

    for (int i = 0; i < blink_count; ++i) {
        board->get_pin(440)->write(moo::PinState::HIGH);
        usleep(500000);
        board->get_pin(440)->write(moo::PinState::LOW);
        usleep(500000);
    }

    // It would also happen automatically if not mentioned.
    // Board class destructor will manage it.
    board->release_pin(440);

    return 0;
}
```

### Using I2C
```cpp
#include <moo-pin/i2c.h>

int main() {
    // Create an I2C interface
    moo::I2C i2c("/dev/i2c-1");

    // Write data to a device
    i2c.write(0x40, 0x00, 0xFF);

    return 0;
}
```

### Getting a Display
You can easily get a display instance using the `get_display` function, which supports various types of displays. Here’s an example (extended version of this code is also available in `examples/ssd1306_text.cpp`):

```cpp
#include <moo-pin/moo-pin.h>

// Dimensions of the display (adjust to the connected screen):
#define COLUMNS 128
#define LINES 32

int main() {
    std::string msg_str = " Hello MilkV!\\n This is new line...\\n";

    // Initialize the Board object:
    auto board = moo::use_board(moo::BoardType::MILKV_DUO_64);
    auto display = board->get_display<moo::SSD1306>(LINES, COLUMNS, 1);
    display->write(msg_str, 20, 1);
    display->close();

    return 0;
}
```

## Design
`moo-pin` follows an object-oriented design, using a combination of template classes, inheritance, and polymorphism. It provides a `Board` class to manage GPIO pins and configurations.

### Key Concepts
1. **Board Management:** Uses `BoardManager` to create boards based on their configuration files.
2. **Pin Access:** Provides functions like `get_pin()` for easy access and management.
3. **Display Initialization:** Uses `get_display()` to create various display instances based on template parameters.
4. **Templates and Inheritance:** Supports specialization for different board types while keeping a common interface.

### Board Initialization
Boards are created using a factory approach:
```cpp
auto board = moo::BoardManager::create_board("MILKV_DUO_64");
```

## Future Enhancements
- **Support for more boards:** Add configurations for other common development boards.
- **Better error handling:** Improve exceptions and logging for debugging.

## Contributing
Contributions are welcome! Feel free to submit issues or pull requests.

### Code formatting:

```bash
# Call when in the main project's directory:
find . -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i
```

## License
MIT License.
