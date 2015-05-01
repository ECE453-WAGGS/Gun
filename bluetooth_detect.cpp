#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "user_drivers/BluetoothControlRegister.h"
#include "user_drivers/GunControlRegister.h"

#define UART_DEV "/dev/ttyPS1"

#define NO_FIRE_NO_FLYWHEEL "N1"
#define NO_FIRE_FLYWHEEL "N0"
#define FIRE_NO_FLYWHEEL "Y1"
#define FIRE_FLYWHEEL "Y0"

int main()
{
    FILE *file;
    int fd;

    BluetoothControlRegister bluetoothControl(BluetoothControlRegister::BT_CTRL_DEV);
    GunControlRegister gunControl(GunControlRegister::GUN_CTRL_DEV);

    std::fstream uart(UART_DEV);
    std::string response;

	//read "man 2 open" to learn about O_NONBLOCK and O_NOCTTY
    //int fd = open(UART_DEV, O_RDWR, 0);
    //if (fd == -1)
    //   return -1;
    file = fopen(UART_DEV, "rwb");
    //if (fd == -1)
    //    return -1;
    fd = fileno(file);

	struct termios termInfo;
    if (tcgetattr(fd, &termInfo) == -1)
		return -1;

    // flow control
    //termInfo.c_cflag &= ~CRTSCTS;
    termInfo.c_cflag |= CRTSCTS;

    // BAUD rate
    cfsetispeed(&termInfo, B115200);
    cfsetospeed(&termInfo, B115200);

    if (tcsetattr(fd, TCSANOW, &termInfo) == -1)
      return -1;

    bluetoothControl.writeWakeSoftware(1);

	//uart << "+\n";
    uart << "SF,1\n";
    uart << "SS,00000000\n";
    uart << "SR,32000000\n";
    //uart << "SR,00000000\n";
    std::cout << "Rebooting. Yellow light should turn off, then on.\n\r";
    uart << "R,1\n";
    std::cout << "Done rebooting.\n\r";

    // enter MLDP mode.  use either function
    uart << "I\n";
    //control.writeCommand(1);

    while (true) {
        uart >> response;
        std::cout << response << std::endl;

        if (response == NO_FIRE_NO_FLYWHEEL) {
            std::cout << "stop firing" << std::endl;
            gunControl.write(false, false);
        } else if (response == NO_FIRE_FLYWHEEL) {
            std::cout << "stop firing" << std::endl;
            gunControl.write(false, true);
        } else if (response == FIRE_NO_FLYWHEEL) {
            // Don't do anything, this doesn't make sense
            std::cout << "fire" << std::endl;
            gunControl.write(true, false);
        } else if (response == FIRE_FLYWHEEL) {
            std::cout << "fire" << std::endl;
            gunControl.write(true, true);
        }
    }

    uart.close();
	close(fd);
    return 0;
}
