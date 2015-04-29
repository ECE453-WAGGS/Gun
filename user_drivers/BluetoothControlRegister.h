#ifndef ZEDBOARD_USER_DRIVERS_BLUETOOTH_CONTROL_REGISTER_H
#define ZEDBOARD_USER_DRIVERS_BLUETOOTH_CONTROL_REGISTER_H

#include <stdint.h>
#include "OutputRegister.h"

class BluetoothControlRegister: public OutputRegister<uint8_t>
{
public:
    // The path to the device file corresponding to the bluetooth control register
    static const std::string BT_CTRL_DEV;

    BluetoothControlRegister(std::string filename);

    void write(bool wakeHardware, bool wakeSoftware, bool command);

    void writeWakeHardware(bool value);
    void writeWakeSoftware(bool value);
    void writeCommand(bool value);

private:
    static const unsigned int WAKE_HW_BIT = 0;
    static const unsigned int WAKE_SW_BIT = 5;
    static const unsigned int CMD_BIT = 4;

    // Don't copy
    BluetoothControlRegister(const BluetoothControlRegister&);
    BluetoothControlRegister& operator=(const BluetoothControlRegister&);
};

#endif
