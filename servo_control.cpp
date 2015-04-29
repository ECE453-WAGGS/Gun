#include <iostream>
#include "user_drivers/ServoControlRegister.h"

int main()
{
	ServoControlRegister panServo(ServoControlRegister::PAN_SERVO_CTRL_DEV);
	ServoControlRegister tiltServo(ServoControlRegister::TILT_SERVO_CTRL_DEV);

	double panMilliseconds, tiltMilliseconds;

	while (true) {
		std::cin >> panMilliseconds >> tiltMilliseconds;

		panServo.writeMilliseconds(panMilliseconds);
		tiltServo.writeMilliseconds(tiltMilliseconds);
	}

	return 0;
}
