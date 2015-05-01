#include <iostream>
#include "user_drivers/GunControlRegister.h"

#define NO_FIRE_NO_FLYWHEEL "N1"
#define NO_FIRE_FLYWHEEL "N0"
#define FIRE_NO_FLYWHEEL "Y1"
#define FIRE_FLYWHEEL "Y0"

int main()
{
	GunControlRegister control(GunControlRegister::GUN_CTRL_DEV);
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
