#include "main.h"
#include "button_app.h"

#define BUTTON_DEBOUNCE_MS 20U

static GPIO_PinState candidate;
static GPIO_PinState stable;
static uint32_t candidate_since;

volatile uint32_t g_button_press_count = 0U;

void ButtonApp_Init(void)
{
    /* Call only AFTER MX_GPIO_Init(). LED is active HIGH. */
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
    candidate = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin);
    stable = candidate;
    candidate_since = HAL_GetTick();
    g_button_press_count = 0U;
    /* A button already held at startup is ignored until release/re-press. */
}

void ButtonApp_Task(void)
{
    uint32_t now = HAL_GetTick();
    GPIO_PinState raw = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin);

    /* Every observed transition restarts the stability timer. */
    if (raw != candidate)
    {
        candidate = raw;
        candidate_since = now;
    }

    /* Accept a new state only after 20 ms with no observed transition. */
    if ((candidate != stable) &&
        ((uint32_t)(now - candidate_since) >= BUTTON_DEBOUNCE_MS))
    {
        stable = candidate;
        /* Pull-up input: RESET means pressed. Release is also debounced. */
        if (stable == GPIO_PIN_RESET)
        {
            HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
            g_button_press_count++;
        }
    }
}
