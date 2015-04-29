#include "GunControlRegister.h"

const std::string GunControlRegister::GUN_CTRL_DEV = "foo";

GunControlRegister::GunControlRegister(std::string filename):
	OutputRegister(filename)
{
}

void GunControlRegister::write(bool fire, bool flywheels)
{
	setBit(FIRE_BIT, fire);
	setBit(FLYWHEELS_BIT, flywheels);

	flush();
}

void GunControlRegister::writeFire(bool value)
{
	writeBit(FIRE_BIT, value);
}

void GunControlRegister::writeFlywheels(bool value)
{
	writeBit(FLYWHEELS_BIT, value);
}