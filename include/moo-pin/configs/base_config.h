#ifndef BASE_CONFIG_H
#define BASE_CONFIG_H

#include <string>

namespace moo {

class BaseConfig {
public:
    BaseConfig(const std::string& gpio_base_path,
               const std::string& gpio_export_path,
               const std::string& gpio_unexport_path,
               const std::string& i2c_base_path)
        : gpio_base_path(gpio_base_path),
          gpio_export_path(gpio_export_path),
          gpio_unexport_path(gpio_unexport_path),
          i2c_base_path(i2c_base_path) {}

    std::string gpio_base_path;
    std::string gpio_export_path;
    std::string gpio_unexport_path;
    std::string i2c_base_path;
};

} // namespace moo

#endif // BASE_CONFIG_H
