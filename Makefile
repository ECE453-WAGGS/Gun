SRC_FILES = $(wildcard *.cpp) $(wildcard user_drivers/*.cpp)
OBJ_FILES = $(patsubst %.cpp, %.o, $(SRC_FILES))

all: bluetooth_detect gun_control

bluetooth_detect: bluetooth_detect.o user_drivers/BluetoothControlRegister.o
	g++ -Wall $^ -o bluetooth_detect

gun_control: gun_control.o
	g++ -Wall $^ -o gun_control

clean:
	rm -f $(OBJ_FILES) bluetooth_detect gun_control
.PHONY: clean
