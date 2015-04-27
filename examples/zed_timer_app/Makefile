CC = arm-linux-gnueabihf-gcc
CFLAGS = -g

all : zed_timer_app

led_blink.o: zed_timer_app.c
	${CC} –c $@ 

led_blink : zed_timer_app.o
	${CC} –o $@  $^ ${CFLAGS} 

clean : 
	rm  -fv *.o
	rm  -fv zed_timer_app

 .PHONY : clean
