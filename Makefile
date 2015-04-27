all: bluetooth_detect gun_control

bluetooth_detect: bluetooth_detect.c
	g++ bluetooth_detect.cpp -o bluetooth_detect

gun_control: gun_control.cpp
	g++ gun_control.cpp -o gun_control

clean:
	rm -f bluetooth_detect gun_control
	rm -f *.o
.PHONY: clean
