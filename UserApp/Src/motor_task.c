#include <stdio.h>
#include "cmsis_os.h"
#include "pwm1.h"
#include "key.h"
#include "MotorDirCtl.h"
#include "sample_task.h"
#include "motor_task.h"

osThreadId  MotorTaskThreadHandle;

static void motor_thread(void const *argument);

void motor_task_init(void)
{
	motor_dir_init();
	pwm1_init();
	osThreadDef(Motor_Task,  motor_thread, osPriorityAboveNormal, 0, configMINIMAL_STACK_SIZE);
	MotorTaskThreadHandle = osThreadCreate(osThread(Motor_Task), NULL);
}

static void motor_thread(void const *argument)
{
	(void) argument;
	uint8_t speed = 5;

  for (;;)
  {
		if(osSemaphoreWait(osSemaphore_key0 , 50) == osOK)
		{
//			speed++;
//			if(speed > 9)
//				speed = 9;
			pwm1_stop(PWM_CHANNEL_3);
			pwm1_stop(PWM_CHANNEL_4);
			osDelay(100);
			motor_dir_ctl(MOTOR_LEFT, MOTOR_FORWARD);
			motor_dir_ctl(MOTOR_RIGHT, MOTOR_FORWARD);
			printf("speed: %d  dir: MOTOR_FORWARD\r\n",speed);
			pwm1_start(PWM_CHANNEL_4, speed);
			pwm1_start(PWM_CHANNEL_3, speed);
		}
		if(osSemaphoreWait(osSemaphore_key1 , 50) == osOK)
		{
//			speed--;
//			if(speed < 1)
//				speed = 1;
			pwm1_stop(PWM_CHANNEL_3);
			pwm1_stop(PWM_CHANNEL_4);
			osDelay(100);
			motor_dir_ctl(MOTOR_LEFT, MOTOR_BACK);
			motor_dir_ctl(MOTOR_RIGHT, MOTOR_BACK);
			printf("speed: %d  dir: MOTOR_BACK\r\n",speed);
			pwm1_start(PWM_CHANNEL_4, speed);
			pwm1_start(PWM_CHANNEL_3, speed);
		}
		if(osSemaphoreWait(osSemaphore_wkup , 50) == osOK)
		{
			printf("motor stop\r\n");
			pwm1_stop(PWM_CHANNEL_4);
			pwm1_stop(PWM_CHANNEL_3);
		}
    osDelay(100);

	}

}

