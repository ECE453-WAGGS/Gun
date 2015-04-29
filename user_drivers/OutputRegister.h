#ifndef ZEDBOARD_USER_DRIVERS_OUTPUT_REGISTER_H
#define ZEDBOARD_USER_DRIVERS_OUTPUT_REGISTER_H

#include <fstream>
#include <string>
#include <fstream>

template <typename T>
class OutputRegister
{
public:
	OutputRegister(std::string filename);
    virtual ~OutputRegister();

protected:
    void flush();
    void setBit(unsigned int bit, bool value);
    void writeBit(unsigned int bit, bool value);

private:
    T reg;
    std::ofstream file;

    // Don't copy
    OutputRegister(const OutputRegister&);
    OutputRegister& operator=(const OutputRegister&);
};

template <typename T>
OutputRegister<T>::OutputRegister(std::string filename):
	reg(0),
	file(filename.c_str())
{
}

template <typename T>
OutputRegister<T>::~OutputRegister()
{
	file.close();
}

template <typename T>
void OutputRegister<T>::flush()
{
	file << reg << std::flush;
}

template <typename T>
void OutputRegister<T>::setBit(unsigned int bit, bool value)
{
    if (value) {
        reg |= (1 << bit);
    } else {
        reg &= ~(1 << bit);
    }
}

template <typename T>
void OutputRegister<T>::writeBit(unsigned int bit, bool value)
{
	setBit(bit, value);
	flush();
}

#endif
