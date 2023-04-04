#ifndef _ADJUSTMENT_TASK_h_
#define _ADJUSTMENT_TASK_h_
#include "sys.h"
#include "task.h"

#define Manual		0
#define Auto			1

void Adjustment_task(void *pvParameters);
void AJ_widget_SW_Save(void);
uint16_t AJ_widget_SW_IsEqual(void);

#endif
