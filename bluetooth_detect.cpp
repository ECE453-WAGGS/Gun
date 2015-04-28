#include <fstream>
#include <iostream>
#include "user_drivers/BluetoothControl.h"

#define UART_DEV "/dev/ttyPS1"

int main()
{
    BluetoothControl control("foo");
    std::fstream devFile(UART_DEV);

    devFile << "HI THERE\n\r";

    while (true) {
        std::cout << "FIRE" << std::flush;
    }

    return 0;
}
