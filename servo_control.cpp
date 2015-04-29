#include <iostream>
#include "user_drivers/ServoControlRegister.h"

int main()
{
	ServoControlRegister panServo("/dev/stdout");
	ServoControlRegister tiltServo("/dev/stdout");

	double panMilliseconds, tiltMilliseconds;

	while (true) {
		std::cin >> panMilliseconds >> tiltMilliseconds;

		panServo.writeMilliseconds(panMilliseconds);
		tiltServo.writeMilliseconds(tiltMilliseconds);
	}

	return 0;
}