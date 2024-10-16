#ifndef MOO_I2C_H
#define MOO_I2C_H

#include <stdexcept>
#include <string>

namespace moo {

namespace details {

extern "C" {
    #include <stdint.h>
    #include <stdlib.h>
    #include <inttypes.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <fcntl.h>
    #include <sys/ioctl.h>
    #include <linux/i2c-dev.h>

    int i2c_init_helper(int i2c_number, int dev_address, const char* i2c_path) {
        int i2c_c_file = 0;
        if (i2c_c_file == 0)
        {
            i2c_c_file = open(i2c_path, O_RDWR);
            if (i2c_c_file < 0)
            {
                return -1;
            }
            if (ioctl(i2c_c_file, I2C_SLAVE, dev_address) < 0) // Set slave address.
            {
                close(i2c_c_file);
                return -1;
            }
        }
        return i2c_c_file;
    }

    int i2c_close_helper(int i2c_c_file) {
        if (i2c_c_file != 0)
        {
            close(i2c_c_file);
            return 0;
        }
        return 1;
    }

    void i2c_write_helper(int i2c_c_file, uint8_t* ptr, int16_t len) {
        write(i2c_c_file, ptr, len);
    }

    void i2c_read_helper(int i2c_c_file, uint8_t* ptr, int16_t len) {
        read(i2c_c_file, ptr, len);
    }
}

} // namespace details


class I2C {
public:
    I2C(int i2c_number, int dev_address, const std::string& i2c_base_path)
        : i2c_number(i2c_number), dev_address(dev_address) {
        i2c_path = i2c_base_path + std::to_string(i2c_number);
        int result = details::i2c_init_helper(i2c_number, dev_address, i2c_path.c_str());
        if (result == -1) {
            throw std::runtime_error("Failed to open I2C file.");
        }
        i2c_c_file = result;
    }

    void close() {
        int result = details::i2c_close_helper(i2c_c_file);
        if (result != 0) {
            throw std::runtime_error("Failed to close I2C file.");
        }
        i2c_c_file = 0;
    }

    void write(uint8_t* ptr, int16_t len) {
        if (i2c_c_file == 0 || ptr == 0 || len <= 0) {
            throw std::runtime_error("Cannot write to I2C file.");
        }

        details::i2c_write_helper(i2c_c_file, ptr, len);
    }

    void read(uint8_t* ptr, int16_t len) {
        if (i2c_c_file == 0 || ptr == 0 || len <= 0) {
            throw std::runtime_error("Cannot read from I2C file.");
        }

        details::i2c_read_helper(i2c_c_file, ptr, len);
    }

private:
    int i2c_number;
    int dev_address;
    std::string i2c_path;
    int i2c_c_file = 0;
};

} // namespace moo

#endif // MOO_I2C_H
