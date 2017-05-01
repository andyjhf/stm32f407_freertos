#include "cmsis_os.h"

#define KEY0_PRES 	1
#define KEY1_PRES		2
#define WKUP_PRES   3
#define KEY_RELEASE	4

extern osSemaphoreId osSemaphore_key0, osSemaphore_key1, osSemaphore_wkup;
extern osMessageQId 	osMessage_key_status;
void sample_task_init(void);
