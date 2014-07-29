#include "sensors.h"

#define MIC 2
#define VIBRO 3
#define DIST1 5
#define LUM 6
#define DIST2 7

Sensors check_all_sensors(){

	Sensors sensor;

	sensor.mic = check_mic();
	sensor.distance1 = check_distance1();
	sensor.distance2 = check_distance2();
	sensor.lumino = check_lumino();
	sensor.vibro = check_vibro();

	return sensor;
}

int check_mic(){

	return (LPC_GPIO2->FIOPIN >> MIC) & 0x01;

}

int check_vibro(){

	return (LPC_GPIO2->FIOPIN >> VIBRO) & 0x01;

}
int check_distance1(){

	return (LPC_GPIO2->FIOPIN >> DIST1) & 0x01;

}
int check_distance2(){

	return (LPC_GPIO2->FIOPIN >> DIST2) & 0x01;

}
int check_lumino(){

	return (LPC_GPIO2->FIOPIN >> LUM) & 0x01;

}
