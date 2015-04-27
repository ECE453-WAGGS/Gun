#ifndef ZEDBOARD_USER_DRIVERS_BLUETOOTH_CONTROL_H
#define ZEDBOARD_USER_DRIVERS_BLUETOOTH_CONTROL_H

#include <fstream>
#include <string>
#include <cstdint>

class BluetoothControl
{
public:
    const string DEV_FILE;

    BluetoothControl(std::string filename);
    ~BluetoothControl();

    void set(bool wakeHardware, bool wakeSoftware, bool command);

    void setWakeHardware(bool value);
    void setWakeSoftware(bool value);
    void setCommand(bool value);

private:
    std::ofstream file;
    uint8_t reg;

    // Don't copy
    BluetoothControl(const BluetoothControl&);
    Bluetooth& operator=(const BluetoothControl&);
};

#endif
