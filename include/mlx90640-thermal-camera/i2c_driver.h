#pragma once

#include <stdint.h>

#include <ros/ros.h>

class I2CDriver {
private:
  int i2c_fd = 0;
  const char *i2c_device = "/dev/i2c-1";

  void init(void);

public:
  I2CDriver();

  int read(uint8_t slaveAddr, uint16_t startAddress, uint16_t nMemAddressRead,
           uint16_t *data);
  int write(uint8_t slaveAddr, uint16_t writeAddress, uint16_t data);
  int generalReset(void);
};