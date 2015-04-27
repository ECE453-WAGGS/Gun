//============================================================================
// Name        : main.c
// Author      :
// Version     :
// Copyright   :
// Description :
//============================================================================
/* Code taken from Lecture_10_Interrupt_Handlers.pdf by Mark McDermott */

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define DEBUG

#define DEVICE_NAME	"/dev/zed_timer_device"

unsigned int irqcount = 0;

/* This function gets called whenever there is an interrupt. */
void interrupt_callback(int signo)
{
	irqcount++;

#ifdef DEBUG
	printf("Interrupt!\n");
#endif
}


int main()
{
	struct sigaction action;
	int fd = 0;

	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGIO);

	/* Set the function to be called when SIGIO is received */
	action.sa_handler = interrupt_callback;

	action.sa_flags = 0;

	/* Set the action to be taken when SIGIO is received */
	sigaction(SIGIO, &action, NULL);


	/* Tell the kernel to signal us when it gets an interrupt */
	fd = open(DEVICE_NAME, O_RDWR);
	if (fd < 0) {
		printf("Error opening device: %s", strerror(errno));
		return (EXIT_FAILURE);
	}
	if (-1 == fcntl(fd, F_SETOWN, getpid())) {
		printf("Error setting owner: %s", strerror(errno));
		return (EXIT_FAILURE);
	}
	if (-1 == fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_ASYNC)) {
		printf("Error setting flags: %s", strerror(errno));
		return (EXIT_FAILURE);
	}


	printf("Waiting for interrupts...\n\n");


	/* Loop forever, waiting for interrupts */
	while (1) {
		sleep(86400);	/* This will end early when we get an interrupt. */
		printf("Current value of interrupt counter: %u\n", irqcount);
	}

	/* Cleanup */
	close(fd);

	return (EXIT_SUCCESS);
}
