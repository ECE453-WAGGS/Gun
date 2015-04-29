#ifndef ZEDBOARD_USER_DRIVERS_SERVO_CONTROL_REGISTER_H
#define ZEDBOARD_USER_DRIVERS_SERVO_CONTROL_REGISTER_H

#include <stdint.h>
#include "OutputRegister.h"

class ServoControlRegister: public OutputRegister<uint32_t>
{
public:
	// The paths to the device files corresponding to the servo control registers
	static const std::string PAN_SERVO_CTRL_DEV;
	static const std::string TILT_SERVO_CTRL_DEV;

	// The ZedBoard's PL uses a 100 Mhz clock
	static const unsigned int TICKS_PER_MILLISECOND = 100000;

	ServoControlRegister(std::string filename);

	void writeMilliseconds(double milliseconds);

private:
	// Don't copy
	ServoControlRegister(const ServoControlRegister&);
	ServoControlRegister& operator=(const ServoControlRegister&);
};

#endif