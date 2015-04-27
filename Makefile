all: bluetooth_detect gun_control

bluetooth_detect: bluetooth_detect.c
	gcc bluetooth_detect.c -o bluetooth_detect

gun_control: gun_control.cpp
	g++ gun_control.cpp -o gun_control

clean:
	rm -f bluetooth_detect gun_control
	rm -f *.o
.PHONY: clean
