#include <iostream>
#include "user_drivers/GunControlRegister.h"

int main()
{
	GunControlRegister control("/dev/stdout");
    char input;

    while (true) {
        std::cin >> input;

        switch (input) {
        	case 'F':
        		control.writeFire(true);
        		break;
    		case 'N':
    			control.writeFire(false);
    			break;
			case 'W':
				control.writeFlywheels(true);
				break;
			case 'S':
				control.writeFlywheels(false);
				break;
        }
    }

    return 0;
}
