#include <fstream>
#include <iostream>
#include <string>
#include "user_drivers/BluetoothControlRegister.h"

#define UART_DEV "/dev/ttyPS1"

int main()
{
    BluetoothControlRegister control(BluetoothControlRegister::BT_CTRL_DEV);
    std::fstream devFile(UART_DEV);
    std::string response;

    control.writeWakeSoftware(1);
    control.writeWakeHardware(1);
    control.writeCommand(1);

    devFile << "+\n";

    devFile << "SF,1\n";

    devFile << "SS,C0000000\n";

    devFile << "SR,90000000\n";

    devFile << "R,1\n";

    devFile << "LS\n";

    // devFile >> response;
    // std::cout << response << std::endl;

    devFile << "A\n";

    // while (true) {
    //     std::cout << "FIRE" << std::flush;
    // }
    devFile.close();
    return 0;
}
