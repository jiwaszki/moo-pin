#ifndef DISPLAY_H
#define DISPLAY_H

#include <memory>
#include <string>
#include <stdexcept>

#include "moo-pin/displays/font.h"
#include "moo-pin/displays/ssd1306.h"

namespace moo {

// T - display class (BaseDisplay child)
template <typename T>
class Display {
public:
    virtual ~Display() {}

    virtual void close() = 0;
    virtual void set_cursor(int x, int y) = 0;
    virtual void set_font(moo::FontType font) = 0;
    virtual void write(char* message, int x = 0, int y = 0) = 0;
    virtual void write(const std::string& message, int x = 0, int y = 0) = 0;
    virtual void clear() = 0;
    virtual void clear(int line) = 0;

    std::shared_ptr<T> get_underlaying_display() {
        return display;
    }

protected:
    std::shared_ptr<T> display;
    moo::FontType current_font;
};

template<>
class Display<moo::SSD1306> {
public:
    Display(uint8_t lines, uint8_t columns, int i2c_number, const std::string& i2c_base_path)
        : lines(lines), columns(columns) {
        display = std::make_shared<moo::SSD1306>(i2c_number, i2c_base_path);
        display->default_config(lines, columns);
        display->load_resolution();
        display->clear_screen();
        display->set_XY(0, 0);
        current_font = static_cast<uint8_t>(moo::FontType::SMALL);
    }

    void close() {
        display->close();
    }

    void set_cursor(int x, int y) {
        display->set_XY(x, y);
    }

    void set_font(moo::FontType font) {
        current_font = static_cast<uint8_t>(font);
    }

    void write(char* message, int x = 0, int y = 0) {
        if (x != 0  && y != 0) {
            set_cursor(x, y);
        }
        display->write_string(current_font, message);
    }

    void write(std::string& message, int x = 0, int y = 0) {
        if (x != 0  && y != 0) {
            set_cursor(x, y);
        }
        // TODO: think about the string safety a little more
        display->write_string(current_font, const_cast<char*>(message.c_str()));
    }
    void clear() {
        display->clear_screen();
    }

    void clear(int line) {
        display->clear_line(line);
    }

private:
    std::shared_ptr<moo::SSD1306> display;
    uint8_t current_font;
    int8_t lines;
    int8_t columns;
};
}  // namespace moo

#endif
