#include "Sensors/bmm150.h"

static uint8_t BMM150_readByte(uint8_t reg);
static void BMM150_writeByte(uint8_t reg, uint8_t value);
static void BMM150_readRawData(BMM150_RawData* rawData);

// Funkcja do inicjalizacji I2C
void BMM150_init() {
    Wire.begin();
    // Można dodać inicjalizację magnetometru, jeśli potrzebne
}

// Funkcja do odczytu bajtu z rejestru magnetometru
static uint8_t BMM150_readByte(uint8_t reg) {
    Wire.beginTransmission(BMM150_I2C_ADDR);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(BMM150_I2C_ADDR, (uint8_t)1);
    return Wire.read();
}

// Funkcja do zapisu bajtu do rejestru magnetometru
static void BMM150_writeByte(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(BMM150_I2C_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

// Funkcja do odczytu surowych danych z magnetometru
static void BMM150_readRawData(BMM150_RawData* rawData) {
    rawData->x = (int16_t)((BMM150_readByte(BMM150_REG_DATA_X_MSB) << 8) | BMM150_readByte(BMM150_REG_DATA_X_LSB));
    rawData->y = (int16_t)((BMM150_readByte(BMM150_REG_DATA_Y_MSB) << 8) | BMM150_readByte(BMM150_REG_DATA_Y_LSB));
    rawData->z = (int16_t)((BMM150_readByte(BMM150_REG_DATA_Z_MSB) << 8) | BMM150_readByte(BMM150_REG_DATA_Z_LSB));
}

// Funkcja do konwersji surowych danych na kąty w stopniach
void BMM150_readAngle(float* azimuth) {
    BMM150_RawData rawData;
    BMM150_readRawData(&rawData);

    // Debug: Wypisywanie surowych danych
    Serial.print("Raw X: ");
    Serial.print(rawData.x);
    Serial.print(" Raw Y: ");
    Serial.print(rawData.y);
    Serial.print(" Raw Z: ");
    Serial.println(rawData.z);

    // Konwersja na kąty w stopniach
    *azimuth = atan2((float)rawData.y, (float)rawData.x) * 180.0 / PI;
    if (*azimuth < 0) {
        *azimuth += 360;
    }
}