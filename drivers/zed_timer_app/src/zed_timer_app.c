#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define DEV_FILE "/dev/zed_timer_device"

int main(int argc, char* argv[])
{
	char buffer[64];
	int  numBytes = 0;
	int fd;

	/* read "man 2 open" to learn about O_NONBLOCK and O_NOCTTY */
	fd = open(DEV_FILE, O_RDONLY, 0);
	if (fd == -1)
		goto error;


	while(atoi(buffer) < 10)
	{
		memset(buffer, '\0', sizeof(buffer));
		
		numBytes = read(fd, buffer, 64);
		if( numBytes > 0)
		{
			printf("Num Interrupts:%s\n",buffer);
		}
		else
		{
			printf("Zed device was empty\n");
		}
		sleep(1);
	}
	close(fd);

	printf("Exiting...\n");
	return 0;

error:
	perror(DEV_FILE);
	return 1;
}

