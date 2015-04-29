#include <fstream>
#include <iostream>
#include <string>
#include "user_drivers/BluetoothControlRegister.h"

#define UART_DEV "/dev/ttyPS1"

int main()
{
    BluetoothControlRegister control(BluetoothControlRegister::BT_CTRL_DEV);
    std::fstream uart(UART_DEV);
    std::string response;

    control.writeWakeSoftware(1);
    control.writeWakeHardware(1);
    control.writeCommand(1);

    uart << "+\n\r";
    uart << "SF,1\n\r";
    uart << "SS,C0000000\n\r";
    uart << "SR,90000000\n\r";
    uart << "R,1\n\r";
    uart << "LS\n\r";
    uart << "A\n\r";

    // uart >> response;
    // std::cout << response << std::endl;

    // while (true) {
    //     std::cout << "FIRE" << std::flush;
    // }
    uart.close();
    return 0;
}
