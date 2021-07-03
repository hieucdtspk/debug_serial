/*
    Debug with serial lib for arduino

    03/07/2021
    HieuNT
*/

#include "debug_serial.h"
#include <stdarg.h>
#include <stdio.h>

// Colorize the debug output
#define DBG_COLOR_RED	"\033[0;31m"
#define DBG_COLOR_YELLOW	"\033[0;33m"
#define DBG_COLOR_GREEN	"\033[0;32m"
#define DBG_COLOR_RESET	"\033[0m"

static uint8_t buffer[128];

#ifdef DEBUG_USE_RTOS
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "semphr.h"
// Sem/mutex lock
// SemaphoreHandle_t dbgSem_id;
// osSemaphoreDef(DBGSEM);
SemaphoreHandle_t dbg_semHandler = NULL;
#endif

HardwareSerial *pSerial = NULL;

void user_debug_init(HardwareSerial *_pSerial)
{
	#ifdef DEBUG_USE_RTOS
	dbg_semHandler = xSemaphoreCreateMutex();
	#endif
	pSerial = _pSerial;
}

static void uart_printf(const char *fmt, ...)
{
	if (!pSerial) return;
	uint32_t len;
	va_list vArgs;
	va_start(vArgs, fmt);
	len = vsprintf((char *)&buffer[0], (char const *)fmt, vArgs);
	va_end(vArgs);

	pSerial->write(buffer, len);
}

// chinh sua cai nay
static void uart_vprintf(const char *fmt, va_list vArgs)
{
	if (!pSerial) return;
	uint32_t len;
	len = vsprintf((char *)&buffer[0], (char const *)fmt, vArgs);

	pSerial->write(buffer, len);
}

/* ############### Actual debug redirect ################# */
#define __debug_printf(fmt, ...) uart_printf(fmt, __VA_ARGS__)
#define __debug_vprintf(fmt, vArgs) uart_vprintf(fmt, vArgs)

/* ------------------- MAIN DEBUG I/O -------------- */
void user_debug_print(int level, const char* module, int line, const char* fmt, ...)
{
	if (!pSerial) return;
	#ifdef DEBUG_USE_RTOS
	// osSemaphoreWait(dbgSem_id , osWaitForever);
	xSemaphoreTake( dbg_semHandler, ( TickType_t ) osWaitForever );
	#endif

	switch (level){
		case 1:
			// "->[ERROR](module:line): "
			__debug_printf(DBG_COLOR_RED "[E](%s:%d): ", module, line);
			break;
		case 2:
			// "->[WARN](module:line): "
			__debug_printf(DBG_COLOR_YELLOW "[W](%s:%d): ", module, line);
			break;
		case 3:
			// "->[INFO](module:line): "
			__debug_printf(DBG_COLOR_GREEN "[I](%s:%d): ", module, line);
			break;
		case 4:
		default:
			// Don't need to print DEBUG :P
			__debug_printf("(%s:%d): ", module, line);
			break;
	}

	va_list     vArgs;		    
	va_start(vArgs, fmt);
	__debug_vprintf((char const *)fmt, vArgs);
	va_end(vArgs);
	pSerial->write((const uint8_t* )DBG_COLOR_RESET, strlen(DBG_COLOR_RESET));
	#ifdef DEBUG_USE_RTOS
	// osSemaphoreRelease(dbgSem_id);
	xSemaphoreGive( dbg_semHandler );
	#endif
}

void dbg_error(const char* module, int line, int ret)
{
	if (!pSerial) return;
	#ifdef DEBUG_USE_RTOS
	// osSemaphoreWait(dbgSem_id , osWaitForever);
	xSemaphoreTake( dbg_semHandler, ( TickType_t ) osWaitForever );
	#endif

	__debug_printf("[RC] Module %s - Line %d : Error %d\n", module, line, ret);
	
	#ifdef DEBUG_USE_RTOS
	// osSemaphoreRelease(dbgSem_id);
	xSemaphoreGive( dbg_semHandler );
	#endif
}

void user_debug_print_exact(const char* fmt, ...)
{
	if (!pSerial) return;
	#ifdef DEBUG_USE_RTOS
	// osSemaphoreWait(dbgSem_id , osWaitForever);
	xSemaphoreTake( dbg_semHandler, ( TickType_t ) osWaitForever );
	#endif

	va_list     vArgs;		    
	va_start(vArgs, fmt);
	__debug_vprintf((char const *)fmt, vArgs);
	va_end(vArgs);
	
	#ifdef DEBUG_USE_RTOS
	// osSemaphoreRelease(dbgSem_id);
	xSemaphoreGive( dbg_semHandler );
	#endif
}

void user_debug_native(char *string, int len)
{
	if (!pSerial) return;
	#ifdef DEBUG_USE_RTOS
	// osSemaphoreWait(dbgSem_id , osWaitForever);
	xSemaphoreTake( dbg_semHandler, ( TickType_t ) osWaitForever );
	#endif

	pSerial->write((const uint8_t* )string, len);

	#ifdef DEBUG_USE_RTOS
	// osSemaphoreRelease(dbgSem_id);
	xSemaphoreGive( dbg_semHandler );
	#endif
}

void user_debug_native_byte(unsigned char c)
{
	if (!pSerial) return;
	#ifdef DEBUG_USE_RTOS
	// osSemaphoreWait(dbgSem_id , osWaitForever);
	xSemaphoreTake( dbg_semHandler, ( TickType_t ) osWaitForever );
	#endif

	pSerial->write(c);

	#ifdef DEBUG_USE_RTOS
	// osSemaphoreRelease(dbgSem_id);
	xSemaphoreGive( dbg_semHandler );
	#endif
}
