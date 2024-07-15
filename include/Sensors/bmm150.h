#ifndef BMM150_H
#define BMM150_H

#include <Arduino.h>
#include <Wire.h>

// Adres I2C magnetometru BMM150
#define BMM150_I2C_ADDR 0x10

// Rejestry BMM150
#define BMM150_REG_CHIP_ID 0x40
#define BMM150_REG_DATA_X_LSB 0x42
#define BMM150_REG_DATA_X_MSB 0x43
#define BMM150_REG_DATA_Y_LSB 0x44
#define BMM150_REG_DATA_Y_MSB 0x45
#define BMM150_REG_DATA_Z_LSB 0x46
#define BMM150_REG_DATA_Z_MSB 0x47

// Struktura do przechowywania surowych danych magnetometru
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} BMM150_RawData;

// Funkcje do obsługi magnetometru BMM150
void BMM150_init();
void BMM150_readAngle(float* azimuth);

#endif // BMM150_H
