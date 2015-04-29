#include "ServoControlRegister.h"

const std::string ServoControlRegister::PAN_SERVO_CTRL_DEV = "/sys/kernel/ece453_pulse_width_register_6f200000/write";
const std::string ServoControlRegister::TILT_SERVO_CTRL_DEV = "/sys/kernel/ece453_pulse_width_register_6f220000/write";

ServoControlRegister::ServoControlRegister(std::string filename):
	OutputRegister(filename)
{
}

void ServoControlRegister::writeMilliseconds(double milliseconds)
{
	writeRegister(milliseconds * TICKS_PER_MILLISECOND);
}
