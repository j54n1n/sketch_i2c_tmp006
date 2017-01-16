// External libraries:
#include <Wire.h>

// Read 16 bit int from I2C address addr and register reg
uint16_t i2c_read16(uint8_t addr, uint8_t reg);

// Write data to I2C address addr, register reg
void i2c_write16(uint8_t addr, uint8_t reg, uint16_t data);