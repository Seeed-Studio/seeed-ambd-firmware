#ifndef TEST_FREERTOS_H
#define TEST_FREERTOS_H
#include <stdint.h>
#include <stddef.h>
typedef uint32_t TickType_t;
typedef int32_t BaseType_t;
typedef struct { BaseType_t xOverflowCount; TickType_t xTimeOnEntering; } TimeOut_t;
#define configTICK_RATE_HZ 1000U
#define portMAX_DELAY UINT32_MAX
#define pdTRUE 1
void test_enter_critical(void);
void test_exit_critical(void);
void *pvPortMalloc(size_t length);
void vPortFree(void *memory);
#define taskENTER_CRITICAL() test_enter_critical()
#define taskEXIT_CRITICAL() test_exit_critical()
#endif
