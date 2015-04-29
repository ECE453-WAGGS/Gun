#include "ServoControlRegister.h"

const std::string PAN_SERVO_CTRL_DEV = "foo";
const std::string TILT_SERVO_CTRL_DEV = "foo";

ServoControlRegister::ServoControlRegister(std::string filename):
	OutputRegister(filename)
{
}

void ServoControlRegister::writeMilliseconds(double milliseconds)
{
	writeRegister(milliseconds * TICKS_PER_MILLISECOND);
}