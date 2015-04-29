#ifndef ZEDBOARD_USER_DRIVERS_GUN_CONTROL_REGISTER_H
#define ZEDBOARD_USER_DRIVERS_GUN_CONTROL_REGISTER_H

#include <stdint.h>
#include "OutputRegister.h"

class GunControlRegister: public OutputRegister<uint8_t>
{
public:
	// The path to the device file corresponding to the gun control register
	static const std::string GUN_CTRL_DEV;

	GunControlRegister(std::string filename);

	void write(bool fire, bool flywheels);

	void writeFire(bool value);
	void writeFlywheels(bool value);

private:
	// Note that, because the circuit originally meant to supply the fire signal to the gun does
	// not work, the fire signal is now coming from the FLYWHEEL_2 pin
	static const unsigned int FIRE_BIT = 2;
	static const unsigned int FLYWHEELS_BIT = 1;

	// Don't copy
	GunControlRegister(const GunControlRegister&);
	GunControlRegister& operator=(const GunControlRegister&);
};

#endif