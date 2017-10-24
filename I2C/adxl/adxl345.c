#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <sys/ioctl.h>

//The arduino board i2c address (slave)
#define ADDRESS 0x53
//The I2C bus:This is for V2 pi's. For V1 Model B you need i2c-0
static const char *devName = "/dev/i2c-1";

void adxl_init(int file);
int convert(char a, char b);

int main(int argc, char** argv){
	int file,num;
	printf("I2C: Connecting\n");
	if((file = open(devName,O_RDWR))<0){
		fprintf(stderr,"I2C: Failed to access %d\n",devName);
		exit(1);
	}
	printf("I2C: acquiring buss to 0x%x\n",ADDRESS);
	if(ioctl(file,I2C_SLAVE,ADDRESS)<0){
		fprintf(stderr,"I2C: Failed to acquire bus access/talk to slave 0x%x\n",ADDRESS);
		exit(1);
	}
        adxl_init(file);
	while(1){
		
		usleep(50000);
		char reg[1] = {0x32};
		write(file, reg, 1);
		char data[6] = {0};
		if(read(file, data, 6) != 6){
			printf("Error : Input/output Error \n");
			exit(1);
		}else{
	 		int xAccl = convert(data[1], data[0]);
			int yAccl = convert(data[3], data[2]);
			int zAccl = convert(data[5], data[4]);
			printf("X: %d \n", xAccl);
			printf("Y: %d \n", yAccl);
			printf("Z: %d \n", zAccl);
		}
	}
}

void adxl_init(int file){
	char config[2] = {0};
	config[0] = 0x2C;
	config[1] = 0x0A;
	write(file, config, 2);

	config[0] = 0x2D;
	config[1] = 0x08;
	write(file, config, 2);

	config[0] = 0x31;
	config[1] = 0x08;
	write(file, config, 2);
	sleep(1);
}

int convert(char a, char b){
	//printf("a = %i	a&0x03 = %d",(int)a, a&0x03);
	int Accl = ((a&0x03)*256+(b&0xFF));
	if(Accl > 511){
		Accl -= 1024;
	}
	return Accl;
}
