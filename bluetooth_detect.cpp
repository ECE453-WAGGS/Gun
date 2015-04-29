#include <fstream>
#include <iostream>
#include <string>
#include "user_drivers/BluetoothControlRegister.h"

#define UART_DEV "/dev/ttyPS1"

int main()
{
    BluetoothControlRegister control(BluetoothControlRegister::BT_CTRL_DEV);
    std::fstream devFile(UART_DEV);

    devFile << "HI THERE\n\r";

    while (true) {
        std::cout << "FIRE" << std::flush;
    }

    return 0;
}
