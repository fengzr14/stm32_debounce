#ifndef BUTTON_APP_H
#define BUTTON_APP_H

#include <stdint.h>

/* Visible in Keil Watch; incremented once per accepted press. */
extern volatile uint32_t g_button_press_count;

void ButtonApp_Init(void);
void ButtonApp_Task(void);

#endif
