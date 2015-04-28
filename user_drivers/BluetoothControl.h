#ifndef ZEDBOARD_USER_DRIVERS_BLUETOOTH_CONTROL_H
#define ZEDBOARD_USER_DRIVERS_BLUETOOTH_CONTROL_H

#include <fstream>
#include <string>
#include <stdint.h>
#include "Register.h"

class BluetoothControl: public Register<uint8_t>
{
public:
    const std::string DEV_FILE;

    BluetoothControl(std::string filename);
    ~BluetoothControl();

    void set(bool wakeHardware, bool wakeSoftware, bool command);

    void setWakeHardware(bool value);
    void setWakeSoftware(bool value);
    void setCommand(bool value);

private:
    static const unsigned int WAKE_HW_BIT = 0;
    static const unsigned int WAKE_SW_BIT = 5;
    static const unsigned int CMD_BIT = 4;

    std::ofstream file;

    // Don't copy
    BluetoothControl(const BluetoothControl&);
    BluetoothControl& operator=(const BluetoothControl&);
};

#endif
