#ifndef SSD1306_H
#define SSD1306_H

#include <memory>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "moo-pin/i2c.h"
#include "moo-pin/displays/base_display.h"
#include "moo-pin/displays/font.h"

/////////////////////////////////////////

// Different constants
#define SSD1306_I2C_ADDR            0x3c

#define SSD1306_COMM_CONTROL_BYTE   0x00
#define SSD1306_DATA_CONTROL_BYTE   0x40

#define SSD1306_COMM_DISPLAY_OFF    0xae
#define SSD1306_COMM_DISPLAY_ON     0xaf
#define SSD1306_COMM_HORIZ_NORM     0xa0
#define SSD1306_COMM_HORIZ_FLIP     0xa1
#define SSD1306_COMM_RESUME_RAM     0xa4
#define SSD1306_COMM_IGNORE_RAM     0xa5
#define SSD1306_COMM_DISP_NORM      0xa6
#define SSD1306_COMM_DISP_INVERSE   0xa7
#define SSD1306_COMM_MULTIPLEX      0xa8
#define SSD1306_COMM_VERT_OFFSET    0xd3
#define SSD1306_COMM_CLK_SET        0xd5
#define SSD1306_COMM_PRECHARGE      0xd9
#define SSD1306_COMM_COM_PIN        0xda
#define SSD1306_COMM_DESELECT_LV    0xdb
#define SSD1306_COMM_CONTRAST       0x81
#define SSD1306_COMM_DISABLE_SCROLL 0x2e
#define SSD1306_COMM_ENABLE_SCROLL  0x2f
#define SSD1306_COMM_PAGE_NUMBER    0xb0
// SSD1306_COMM_LOW_COLUMN originally 0x00 moved to 0x01
// that created a "trash" column on the edge, fixed with this?
#define SSD1306_COMM_LOW_COLUMN     0x00
// SSD1306_COMM_LOW_COLUMN originally 0x10 moved to 0x11
// that created a "trash" column on the edge, fixed with this?
#define SSD1306_COMM_HIGH_COLUMN    0x10

#define SSD1306_COMM_START_LINE     0x40

#define SSD1306_COMM_CHARGE_PUMP    0x8d

#define SSD1306_COMM_SCAN_NORM      0xc0
#define SSD1306_COMM_SCAN_REVS      0xc8

#define SSD1306_COMM_MEMORY_MODE    0x20
#define SSD1306_COMM_SET_COL_ADDR   0x21
#define SSD1306_COMM_SET_PAGE_ADDR  0x22

#define SSD1306_HORI_MODE           0x00
#define SSD1306_VERT_MODE           0x01
#define SSD1306_PAGE_MODE           0x02

#define SSD1306_FONT_SMALL          0x00
#define SSD1306_FONT_NORMAL         0x01

#define SSD1306_128_64_LINES        64
#define SSD1306_128_32_LINES        32
#define SSD1306_64_48_LINES         48

#define SSD1306_128_64_COLUMNS      128
#define SSD1306_128_32_COLUMNS      128
#define SSD1306_64_48_COLUMNS       64

/////////////////////////////////////////

namespace moo {

class SSD1306 : public BaseDisplay {
public:
    SSD1306(int i2c_number, const std::string& i2c_base_path) {
        display_i2c = std::make_shared<I2C>(i2c_number, SSD1306_I2C_ADDR, i2c_base_path);
        // Test the connection
        uint8_t control_byte = SSD1306_COMM_CONTROL_BYTE;
        uint8_t control_result = 0;
        display_i2c->write(&control_byte, 1);
        display_i2c->read(&control_result, 1);
        if (control_result == 0) {
            throw std::runtime_error("Failed to initialize display!");
        }
    }

    void close() {
        display_i2c->close();
    }

    void onoff(uint8_t onoff) {
        data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
        if (onoff == 0) {
            data_buf[1] = SSD1306_COMM_DISPLAY_OFF;
        }
        else {
            data_buf[1] = SSD1306_COMM_DISPLAY_ON;
        }
        display_i2c->write(&data_buf[0], 2); // or just data_buf?
    }

    void horizontal_flip(uint8_t flip) {
        data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
        if (flip == 0) {
            data_buf[1] = SSD1306_COMM_HORIZ_NORM;
        }
        else {
            data_buf[1] = SSD1306_COMM_HORIZ_FLIP;
        }
        display_i2c->write(&data_buf[0], 2);
    }

    void display_flip(uint8_t flip) {
        data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
        if (flip == 0) {
            data_buf[1] = SSD1306_COMM_DISP_NORM;
        }
        else {
            data_buf[1] = SSD1306_COMM_DISP_INVERSE;
        }
        display_i2c->write(&data_buf[0], 2);
    }

    void multiplex(uint8_t row) {
        data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
        data_buf[1] = SSD1306_COMM_MULTIPLEX;
        data_buf[2] = row - 1;

        display_i2c->write(&data_buf[0], 3);
    }

    void vert_shift(uint8_t offset) {
        data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
        data_buf[1] = SSD1306_COMM_VERT_OFFSET;
        data_buf[2] = offset;

        display_i2c->write(&data_buf[0], 3);
    }

    void set_clock(uint8_t clk) {
        data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
        data_buf[1] = SSD1306_COMM_CLK_SET;
        data_buf[2] = clk;

        display_i2c->write(&data_buf[0], 3);
    }

    void set_precharge(uint8_t precharge) {
        data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
        data_buf[1] = SSD1306_COMM_PRECHARGE;
        data_buf[2] = precharge;

        display_i2c->write(&data_buf[0], 3);
    }

    void set_deselect(uint8_t voltage) {
        data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
        data_buf[1] = SSD1306_COMM_DESELECT_LV;
        data_buf[2] = voltage;

        display_i2c->write(&data_buf[0], 3);
    }

    void set_com_pin(uint8_t value) {
        data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
        data_buf[1] = SSD1306_COMM_COM_PIN;
        data_buf[2] = value;

        display_i2c->write(&data_buf[0], 3);
    }

    void set_mem_mode(uint8_t mode) {
        data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
        data_buf[1] = SSD1306_COMM_MEMORY_MODE;
        data_buf[2] = mode;

        display_i2c->write(&data_buf[0], 3);
    }

    void set_col(uint8_t start, uint8_t end) {
        data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
        data_buf[1] = SSD1306_COMM_SET_COL_ADDR;
        data_buf[2] = start;
        data_buf[3] = end;

        display_i2c->write(&data_buf[0], 4);
    }

    void set_page(uint8_t start, uint8_t end) {
        data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
        data_buf[1] = SSD1306_COMM_SET_PAGE_ADDR;
        data_buf[2] = start;
        data_buf[3] = end;

        display_i2c->write(&data_buf[0], 4);
    }

    void set_contrast(uint8_t value) {
        data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
        data_buf[1] = SSD1306_COMM_CONTRAST;
        data_buf[2] = value;

        display_i2c->write(&data_buf[0], 3);
    }

    void scroll_onoff(uint8_t onoff) {
        data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
        if (onoff == 0) {
            data_buf[1] = SSD1306_COMM_DISABLE_SCROLL;
        }
        else {
            data_buf[1] = SSD1306_COMM_ENABLE_SCROLL;
        }
        display_i2c->write(&data_buf[0], 2);
    }

    void set_X(uint8_t x) {
        if (x >= max_columns) {
            return;
        }

        global_x = x;

        data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
        data_buf[1] = SSD1306_COMM_LOW_COLUMN | (x & 0x0f);
        data_buf[2] = SSD1306_COMM_HIGH_COLUMN | ((x >> 4) & 0x0f);

        display_i2c->write(&data_buf[0], 3);
    }

    void set_Y(uint8_t y) {
        if (y >= (max_lines / 8)) {
            return;
        }

        global_y = y;

        data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
        data_buf[1] = SSD1306_COMM_PAGE_NUMBER | (y & 0x0f);

        display_i2c->write(&data_buf[0], 2);
    }

    void set_XY(uint8_t x, uint8_t y) {
        if (x >= max_columns || y >= (max_lines / 8)) {
            return;
        }

        global_x = x;
        global_y = y;

        data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
        data_buf[1] = SSD1306_COMM_PAGE_NUMBER | (y & 0x0f);
        data_buf[2] = SSD1306_COMM_LOW_COLUMN | (x & 0x0f);
        data_buf[3] = SSD1306_COMM_HIGH_COLUMN | ((x >> 4) & 0x0f);

        display_i2c->write(&data_buf[0], 4);
    }

    void set_rotate(uint8_t degree) {
        // only degree 0 and 180
        if (degree == 0) {
            data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
            data_buf[1] = SSD1306_COMM_HORIZ_FLIP;
            data_buf[2] = SSD1306_COMM_SCAN_REVS;

            display_i2c->write(&data_buf[0], 3);
        }
        else if (degree == 180) {
            data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
            data_buf[1] = SSD1306_COMM_HORIZ_NORM;
            data_buf[2] = SSD1306_COMM_SCAN_NORM;

            display_i2c->write(&data_buf[0], 3);
        }
    }

    void default_config(uint8_t lines, uint8_t columns)
    {
        if (lines != SSD1306_128_64_LINES && lines != SSD1306_128_32_LINES && SSD1306_64_48_LINES)
            lines = SSD1306_128_64_LINES;
            
        if (columns != SSD1306_128_64_COLUMNS && lines != SSD1306_128_32_COLUMNS && SSD1306_64_48_COLUMNS)
            columns = SSD1306_128_64_COLUMNS;
            
        max_lines = lines;
        max_columns = columns;
        global_x = 0;
        global_y = 0;

        save_resolution(max_columns, max_lines);

        uint16_t i = 0;
        data_buf[i++] = SSD1306_COMM_CONTROL_BYTE;  //command control byte
        data_buf[i++] = SSD1306_COMM_DISPLAY_OFF;   //display off
        data_buf[i++] = SSD1306_COMM_DISP_NORM;     //Set Normal Display (default)
        data_buf[i++] = SSD1306_COMM_CLK_SET;       //SETDISPLAYCLOCKDIV
        data_buf[i++] = 0x80;                       // the suggested ratio 0x80
        data_buf[i++] = SSD1306_COMM_MULTIPLEX;     //SSD1306_SETMULTIPLEX
        data_buf[i++] = lines - 1;             // height is 32 or 64 (always -1)
        data_buf[i++] = SSD1306_COMM_VERT_OFFSET;   //SETDISPLAYOFFSET
        data_buf[i++] = 0;                          //no offset
        data_buf[i++] = SSD1306_COMM_START_LINE;    //SETSTARTLINE
        data_buf[i++] = SSD1306_COMM_CHARGE_PUMP;   //CHARGEPUMP
        data_buf[i++] = 0x14;                       //turn on charge pump
        data_buf[i++] = SSD1306_COMM_MEMORY_MODE;   //MEMORYMODE
        data_buf[i++] = SSD1306_PAGE_MODE;          // page mode
        data_buf[i++] = SSD1306_COMM_HORIZ_NORM;    //SEGREMAP  Mirror screen horizontally (A0)
        data_buf[i++] = SSD1306_COMM_SCAN_NORM;     //COMSCANDEC Rotate screen vertically (C0)
        data_buf[i++] = SSD1306_COMM_COM_PIN;       //HARDWARE PIN 
        if (lines == 32)
            data_buf[i++] = 0x02;                       // for 32 lines
        else
            data_buf[i++] = 0x12;                       // for 64 lines or 48 lines
        data_buf[i++] = SSD1306_COMM_CONTRAST;      //SETCONTRAST
        data_buf[i++] = 0x7f;                       // default contract value
        data_buf[i++] = SSD1306_COMM_PRECHARGE;     //SETPRECHARGE
        data_buf[i++] = 0xf1;                       // default precharge value
        data_buf[i++] = SSD1306_COMM_DESELECT_LV;   //SETVCOMDETECT                
        data_buf[i++] = 0x40;                       // default deselect value
        data_buf[i++] = SSD1306_COMM_RESUME_RAM;    //DISPLAYALLON_RESUME
        data_buf[i++] = SSD1306_COMM_DISP_NORM;     //NORMALDISPLAY
        data_buf[i++] = SSD1306_COMM_DISPLAY_ON;    //DISPLAY ON             
        data_buf[i++] = SSD1306_COMM_DISABLE_SCROLL;//Stop scroll
        
        display_i2c->write(&data_buf[0], i);
    }

    // Add version for std::string
    void write_line(uint8_t font_size, char* ptr)
    {
        uint16_t i = 0;
        uint16_t index = 0;
        uint8_t* font_table = 0;
        uint8_t font_table_width = 0;
        
        if (ptr == 0) {
            throw std::runtime_error("String cannot be empty!");
        }
        
        if (font_size == SSD1306_FONT_SMALL) // 5x7
        {
            font_table = (uint8_t*)font5x7;
            font_table_width = 5;
        }
        else if (font_size == SSD1306_FONT_NORMAL) // 8x8
        {
            font_table = (uint8_t*)font8x8;
            font_table_width = 8;
        }
        else {
            throw std::runtime_error("Unkown font!");
        }
        
        data_buf[i++] = SSD1306_DATA_CONTROL_BYTE;
        
        // font table range in ascii table is from 0x20(space) to 0x7e(~)
        while (ptr[index] != 0 && i <= 1024)
        {
            if ((ptr[index] < ' ') || (ptr[index] > '~')) {
                throw std::runtime_error("ASCII error!");
            }

            uint8_t* font_ptr = &font_table[(ptr[index] - 0x20) * font_table_width];
            uint8_t j = 0;
            for (j = 0; j < font_table_width; j++)
            {
                data_buf[i++] = font_ptr[j];
                if (i > 1024) {
                    throw std::runtime_error("data_buf out of range access!");
                }
            }
            // insert 1 col space for small font size)
            if (font_size == SSD1306_FONT_SMALL) {
                data_buf[i++] = 0x00;
            }
            index++;
        }
        
        display_i2c->write(&data_buf[0], i);
    }

    void write_string(uint8_t font_size, char* ptr)
    {        
        if (ptr == 0) {
            throw std::runtime_error("String cannot be empty!");
        }
        
        char* line = 0;
        char* cr = 0;
        char buf[20];

        line = ptr;
        do {
            memset(buf, 0, 20);
            cr = strstr(line, "\\n");
            if (cr != NULL)
            {
                strncpy(buf, line, cr - line);
            }
            else
            {
                strcpy(buf, line);
            }

            // set cursor position
            set_XY(global_x, global_y);
            write_line(font_size, buf);
            
            if (cr != NULL)
            {
                line = &cr[2];
                global_x = 0;
                global_y++;
                if (global_y >= (max_lines / 8))
                    global_y = 0;
            }
            else
                line = NULL;
                    
        }while (line != NULL);
    }

    void clear_line(uint8_t row)
    {
        if (row > (max_lines / 8)) {
            throw std::runtime_error("Row number is not in the range!");
        }
            
        set_XY(0, row);

        data_buf[0] = SSD1306_DATA_CONTROL_BYTE;
        for (uint8_t i = 0; i < max_columns; i++) {
            data_buf[i+1] = 0x00;
        }
            
        display_i2c->write(&data_buf[0], 1 + max_columns);
    }

    void clear_screen()
    {        
        for (uint8_t i = 0; i <= (max_lines / 8); i++)
        {
            clear_line(i);
        }        
    }

    void save_resolution(uint8_t column, uint8_t row)
    {
        std::ofstream file(init_type_file);
        if (!file) {
            throw std::runtime_error("Failed to open OLED file");
        }
        file << std::to_string(column) << "x" << std::to_string(row);
        file.close();
    }

    void load_resolution()
    {
        // TODO: Consider using C version of reading here.
        // It could simplify reading of max_columns/max_lines a lot.
        std::ifstream file(init_type_file);        
        if (!file) {
            throw std::runtime_error("Failed to open OLED file");
        }        

        std::string line;
        if (!std::getline(file, line)) {
            throw std::runtime_error("Failed to read OLED file");
        }

        file.close();

        size_t delimiterPos = line.find('x');
        if (delimiterPos == std::string::npos) {
            throw std::runtime_error("Failed to read OLED file");
        }

        max_columns = static_cast<uint8_t>(std::stoi(line.substr(0, delimiterPos)));
        max_lines = static_cast<uint8_t>(std::stoi(line.substr(delimiterPos + 1)));
    }

private:
    std::shared_ptr<I2C> display_i2c;
    // TODO think about this a little bit more, maybe static/const?
    std::string init_type_file = "/tmp/.ssd1306_oled_type";
    uint8_t data_buf[1024];
    uint8_t max_lines = 0;
    uint8_t max_columns = 0;
    uint8_t global_x = 0;
    uint8_t global_y = 0;
};

} // namespace moo

#endif // SSD1306_H
