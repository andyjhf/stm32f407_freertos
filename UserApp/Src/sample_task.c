#include <stdio.h>
#include "sample_task.h"
#include "key.h"

osThreadId  SampleTaskThreadHandle;
osSemaphoreId osSemaphore_key0, osSemaphore_key1, osSemaphore_wkup;
osMessageQId 	osMessage_key_status;
static void Sample_Task_Thread(void const *argument);


void sample_task_init(void)
{
	
	KEY_Init();
	  /* Define used semaphore */
  osSemaphoreDef(KEY0_SEM);
	osSemaphoreDef(KEY1_SEM);
	osSemaphoreDef(WKUP_SEM);
	
	osMessageQDef(Key_status, 10, uint32_t);
  
	osMessage_key_status = osMessageCreate(osMessageQ(Key_status), 0);
  /* Create the semaphore . */
  osSemaphore_key0 = osSemaphoreCreate(osSemaphore(KEY0_SEM) , 1);
  osSemaphore_key1 = osSemaphoreCreate(osSemaphore(KEY1_SEM) , 1);
  osSemaphore_wkup = osSemaphoreCreate(osSemaphore(WKUP_SEM) , 1);
	osSemaphoreWait(osSemaphore_key0 , 0);
	osSemaphoreWait(osSemaphore_key1 , 0);
	osSemaphoreWait(osSemaphore_wkup , 0);
  
	osThreadDef(Sample_Task,  Sample_Task_Thread, osPriorityHigh, 0, configMINIMAL_STACK_SIZE);
	SampleTaskThreadHandle = osThreadCreate(osThread(Sample_Task), NULL);
	

}

static void Sample_Task_Thread(void const *argument)
{
  (void) argument;
	uint8_t key_value = 0;
	osStatus status;
	for(;;)
	{
		/**********key sample*************/
		key_value = KEY_Scan(0);
		if(key_value != 0)
			status = osMessagePut(osMessage_key_status, key_value, 0);
		status = 0;
/*		switch(key_value)
		{
			case KEY0_PRES:
				printf("Press KEY0\r\n");
				osSemaphoreRelease(osSemaphore_key0);
			break;
			case KEY1_PRES:
				printf("Press KEY1\r\n");
				osSemaphoreRelease(osSemaphore_key1);
			break;
			case WKUP_PRES:
				printf("Press WKUP\r\n");
				osSemaphoreRelease(osSemaphore_wkup);
			break;
			case KEY_RELEASE:
				printf("Release KEY\r\n");
				osSemaphoreRelease(osSemaphore_wkup);
			break;
		
		
		}*/
		/**********key sample*************/
	
	
		osDelay(30);
	}
	
	
}

static uint8_t KEY_Scan(uint8_t mode)
{	 
	static uint8_t key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
		osDelay(10);//去抖动 
		key_up=0;
		if(KEY0==0)
		{
			key_up=0;
			return KEY0_PRES;
		}
		else if(KEY1==0)
		{
			key_up=0;
			return KEY1_PRES;
		}
		else if(WK_UP==1)
		{
			key_up=0;
			return WKUP_PRES;
		}
	}else if(key_up==0&&KEY0==1&&KEY1==1&&WK_UP==0)
	{
		key_up=1;
		return KEY_RELEASE;
	}
 	return 0;// 无按键按下
}
