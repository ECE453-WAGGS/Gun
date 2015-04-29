#include "BluetoothControlRegister.h"

const std::string BluetoothControlRegister::BT_CTRL_DEV = "foo";

BluetoothControlRegister::BluetoothControlRegister(std::string filename):
	OutputRegister(filename)
{
}

void BluetoothControlRegister::write(bool wakeHardware, bool wakeSoftware, bool command)
{
	setBit(WAKE_HW_BIT, wakeHardware);
	setBit(WAKE_SW_BIT, wakeSoftware);
	setBit(CMD_BIT, command);

	flush();
}

void BluetoothControlRegister::writeWakeHardware(bool value)
{
	writeBit(WAKE_HW_BIT, value);
}

void BluetoothControlRegister::writeWakeSoftware(bool value)
{
	writeBit(WAKE_SW_BIT, value);
}

void BluetoothControlRegister::writeCommand(bool value)
{
	writeBit(CMD_BIT, value);
}