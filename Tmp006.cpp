#include "Tmp006.h"
#include <math.h>

// Configuration helpers:
uint16_t Tmp006_sensorCfg(int numSamples) {
	switch (numSamples) {
	case 16:
		return TMP006_CFG_16SAMPLE;
	case 8:
		return TMP006_CFG_8SAMPLE;
	case 4:
		return TMP006_CFG_4SAMPLE;
	case 2:
		return TMP006_CFG_2SAMPLE;
	default:
		return TMP006_CFG_1SAMPLE;
	}
}

int Tmp006_sensorDelay(int numSamples) {
	switch (numSamples) {
	case 16:
	case 8:
	case 4:
	case 2:
		return 250 * numSamples;
	default:
		return 250; // Delay in ms.
	}
}

// Configures sensor, use before reading from it
void Tmp006_config(uint8_t addr, uint16_t samples)
{
	Wire.begin();
	i2c_write16(addr, TMP006_CONFIG, samples | TMP006_CFG_MODEON | TMP006_CFG_DRDYEN);
}

// Read raw sensor temperature
int16_t Tmp006_readRawDieTemperature(uint8_t addr)
{
	int16_t raw = i2c_read16(addr, TMP006_TAMB);

	raw >>= 2;
	return raw;
}

// Read raw thermopile voltage
int16_t Tmp006_readRawVoltage(uint8_t addr)
{
	int16_t raw = i2c_read16(addr, TMP006_VOBJ);
	return raw;
}

// Calculate object temperature based on raw sensor temp and thermopile voltage
double Tmp006_readObjTempC(uint8_t addr)
{
	double Tdie = Tmp006_readRawDieTemperature(addr);
	double Vobj = Tmp006_readRawVoltage(addr);
	Vobj *= 156.25;  // 156.25 nV per LSB
	Vobj /= 1000000000; // nV -> V
	Tdie *= 0.03125; // convert to celsius
	Tdie += 273.15; // convert to kelvin

					// Equations for calculating temperature found in section 5.1 in the user guide
	double tdie_tref = Tdie - TMP006_TREF;
	double S = (1 + TMP006_A1*tdie_tref +
		TMP006_A2*tdie_tref*tdie_tref);
	S *= TMP006_S0;
	S /= 10000000;
	S /= 10000000;

	double Vos = TMP006_B0 + TMP006_B1*tdie_tref +
		TMP006_B2*tdie_tref*tdie_tref;

	double fVobj = (Vobj - Vos) + TMP006_C2*(Vobj - Vos)*(Vobj - Vos);

	double Tobj = sqrt(sqrt(Tdie * Tdie * Tdie * Tdie + fVobj / S));

	Tobj -= 273.15; // Kelvin -> *C
	return Tobj;
}

// Caculate sensor temperature based on raw reading
double Tmp006_readDieTempC(uint8_t addr)
{
	double Tdie = Tmp006_readRawDieTemperature(addr);
	Tdie *= 0.03125; // convert to celsius
	return Tdie;
}