/* FreeRTOS adaptation sketch. The host C++ queue remains the reference implementation. */
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

typedef struct { uint64_t sequence; int32_t quantity; int32_t price_ticks; } order_t;
static QueueHandle_t order_queue;

static void execution_task(void *arg) {
    (void)arg; order_t o;
    for (;;) {
        if (xQueueReceive(order_queue, &o, portMAX_DELAY) == pdPASS) {
            /* Replace with MCU/network-driver send and ACK handling. */
        }
    }
}

void execcore_freertos_init(void) {
    order_queue = xQueueCreate(64, sizeof(order_t));
    xTaskCreate(execution_task, "exec", 1024, NULL, tskIDLE_PRIORITY + 2, NULL);
}
