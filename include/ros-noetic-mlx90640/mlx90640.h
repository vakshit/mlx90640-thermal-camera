#pragma once

#include <cstdint>
#include <iostream>
#include <math.h>

#include <ros-noetic-mlx90640/i2c_driver.h>
#include <ros-noetic-mlx90640/params.h>

#define MLX_I2C_ADDR 0x33
#define SENSOR_W 24
#define SENSOR_H 32

class MLX90640 {
private:
  I2CDriver i2cDriver;
  uint32_t pixels[SENSOR_W * SENSOR_H];

  float minTemp = 100.0;
  float maxTemp = 0.0;
  int fps = 32;

  uint16_t eeMLX90640[832];
  float emissivity = 1;
  uint16_t frame[834];
  float image[768];
  float mlx90640To[768];
  float eTa;
  uint16_t data[768 * sizeof(float)];

  thermalParams sensorParams;

  int dumpEE(uint8_t slaveAddr, uint16_t *eeData);
  int synchFrame(uint8_t slaveAddr);
  int triggerMeasurement(uint8_t slaveAddr);
  int getFrameData(uint8_t slaveAddr, uint16_t *frameData);
  int extractParameters(uint16_t *eeData, thermalParams *mlx90640);
  float getVdd(uint16_t *frameData, const thermalParams *params);
  float getTa(uint16_t *frameData, const thermalParams *params);
  void getImage(uint16_t *frameData, const thermalParams *params,
                float *result);
  void calculateTo(uint16_t *frameData, const thermalParams *params,
                   float emissivity, float tr, float *result);
  int setResolution(uint8_t slaveAddr, uint8_t resolution);
  int getCurResolution(uint8_t slaveAddr);
  int setRefreshRate(uint8_t slaveAddr, uint8_t refreshRate);
  int getRefreshRate(uint8_t slaveAddr);
  int getSubPageNumber(uint16_t *frameData);
  int getCurMode(uint8_t slaveAddr);
  int setInterleavedMode(uint8_t slaveAddr);
  int setChessMode(uint8_t slaveAddr);
  void badPixelsCorrection(uint16_t *pixels, float *to, int mode,
                           thermalParams *params);

  void extractVDDParameters(uint16_t *eeData, thermalParams *mlx90640);
  void extractPTATParameters(uint16_t *eeData, thermalParams *mlx90640);
  void extractGainParameters(uint16_t *eeData, thermalParams *mlx90640);
  void extractTgcParameters(uint16_t *eeData, thermalParams *mlx90640);
  void extractResolutionParameters(uint16_t *eeData, thermalParams *mlx90640);
  void extractKsTaParameters(uint16_t *eeData, thermalParams *mlx90640);
  void extractKsToParameters(uint16_t *eeData, thermalParams *mlx90640);
  void extractAlphaParameters(uint16_t *eeData, thermalParams *mlx90640);
  void extractOffsetParameters(uint16_t *eeData, thermalParams *mlx90640);
  void extractKtaPixelParameters(uint16_t *eeData, thermalParams *mlx90640);
  void extractKvPixelParameters(uint16_t *eeData, thermalParams *mlx90640);
  void extractCPParameters(uint16_t *eeData, thermalParams *mlx90640);
  void extractCILCParameters(uint16_t *eeData, thermalParams *mlx90640);
  int extractDeviatingPixels(uint16_t *eeData, thermalParams *mlx90640);
  int checkAdjacentPixels(uint16_t pix1, uint16_t pix2);
  float getMedian(float *values, int n);
  int isPixelBad(uint16_t pixel, thermalParams *params);
  int validateFrameData(uint16_t *frameData);
  int validateAuxData(uint16_t *auxData);

public:
  MLX90640(int fps);

  void see();
  void copyRawImage(float *pointer);

  float getMin();
  float getMax();
};
