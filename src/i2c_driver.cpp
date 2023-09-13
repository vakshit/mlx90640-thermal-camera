#include <fcntl.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifndef I2C_FUNC_I2C
#include <linux/i2c.h>
#define I2C_MSG_FMT __u8
#endif

#include <sys/ioctl.h>

#include <mlx90640_thermal_camera/i2c_driver.h>

I2CDriver::I2CDriver() {}

void I2CDriver::init() {}

int I2CDriver::generalReset(void) {
  this->write(0x33, 0x66, 0x00);
  return 0;
}

int I2CDriver::read(uint8_t slaveAddr, uint16_t startAddress,
                    uint16_t nMemAddressRead, uint16_t *data) {
  if (!this->i2c_fd) {
    this->i2c_fd = open(this->i2c_device, 02);
  }

  int result;
  char cmd[2] = {(char)(startAddress >> 8), (char)(startAddress & 0xFF)};
  char buf[1664];
  uint16_t *p = data;
  struct i2c_msg i2c_messages[2];
  struct i2c_rdwr_ioctl_data i2c_messageset[1];

  i2c_messages[0].addr = slaveAddr;
  i2c_messages[0].flags = 0;
  i2c_messages[0].len = 2;
  i2c_messages[0].buf = (I2C_MSG_FMT *)cmd;
  i2c_messages[1].addr = slaveAddr;
  i2c_messages[1].flags = I2C_M_RD | I2C_M_NOSTART;
  i2c_messages[1].len = nMemAddressRead * 2;
  i2c_messages[1].buf = (I2C_MSG_FMT *)buf;
  i2c_messageset[0].msgs = i2c_messages;
  i2c_messageset[0].nmsgs = 2;

  memset(buf, 0, nMemAddressRead * 2);
  if (ioctl(this->i2c_fd, I2C_RDWR, &i2c_messageset) < 0) {
    ROS_ERROR("I2C read error!");
    return -1;
  }

  for (int count = 0; count < nMemAddressRead; count++) {
    int i = count << 1;
    *p++ = ((uint16_t)buf[i] << 8) | buf[i + 1];
  }

  return 0;
}

int I2CDriver::write(uint8_t slaveAddr, uint16_t writeAddress, uint16_t data) {
  char cmd[4] = {(char)(writeAddress >> 8), (char)(writeAddress & 0x00FF),
                 (char)(data >> 8), (char)(data & 0x00FF)};
  int result;

  struct i2c_msg i2c_messages[1];
  struct i2c_rdwr_ioctl_data i2c_messageset[1];

  i2c_messages[0].addr = slaveAddr;
  i2c_messages[0].flags = 0;
  i2c_messages[0].len = 4;
  i2c_messages[0].buf = (I2C_MSG_FMT *)cmd;
  i2c_messageset[0].msgs = i2c_messages;
  i2c_messageset[0].nmsgs = 1;

  if (ioctl(this->i2c_fd, I2C_RDWR, &i2c_messageset) < 0) {
    ROS_ERROR("I2C write error!");
    return -1;
  }

  return 0;
}
