#include "stm32f4xx_hal.h"

#define MOTOR_RIGHT			1
#define MOTOR_LEFT			2

#define MOTOR_FORWARD		1
#define MOTOR_BACK			2

void motor_dir_init(void);
void motor_dir_ctl(char motor, char dir);


