#ifndef ZEDBOARD_USER_DRIVERS_REGISTER_H
#define ZEDBOARD_USER_DRIVERS_REGISTER_H

template <typename T>
class Register
{
public:
    virtual ~Register();

protected:
    T reg;

    void setBit(unsigned int bit, bool value);
};

template <typename T>
Register<T>::~Register()
{
}

template <typename T>
void Register<T>::setBit(unsigned int bit, bool value)
{
    if (value) {
        reg |= 1 << bit;
    } else {
        reg &= ~(1 << bit);
    }
}

#endif
