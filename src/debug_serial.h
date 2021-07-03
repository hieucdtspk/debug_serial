/*
    Debug with serial lib for arduino

    03/07/2021
    HieuNT
*/
#ifndef __ARDUINO_DEBUG_SERIAL_H_
#define __ARDUINO_DEBUG_SERIAL_H_

#include <Arduino.h>

// #ifdef __cplusplus
// extern "C" {
// #endif

#ifdef DEBUG_SERIAL

#define debug_serial_init() user_debug_init(&(DEBUG_SERIAL))
void user_debug_init(HardwareSerial *_pSerial);
void user_debug_print(int level, const char* module, int line, const char* fmt, ...);
void user_debug_print_error(const char* module, int line, int ret);
void user_debug_print_exact(const char* fmt, ...);
void user_debug_native(char *buffer, int len);
void user_debug_native_byte(unsigned char c);

// TODO: check if arduino use esp32 ==> map to ESP_LOG framework!

#if __DEBUG__ > 0
#ifndef __MODULE__
#error "__MODULE__ must be defined"
#endif
#endif

#if __DEBUG__ >= 1
#define error(...) do{ user_debug_print(1, __MODULE__, __LINE__, __VA_ARGS__); }while(0)
#define ERROR(...) do{ user_debug_print(1, __MODULE__, __LINE__, __VA_ARGS__); }while(0)
#else
#define error(...)
#define ERROR(...)  
#endif

#if __DEBUG__ >= 2
#define WARN(...) do{ user_debug_print(2, __MODULE__, __LINE__, __VA_ARGS__); }while(0)
#define warn(...) do{ user_debug_print(2, __MODULE__, __LINE__, __VA_ARGS__); }while(0)
#else
#define WARN(...) 
#define warn(...)
#endif

#if __DEBUG__ >= 3
#define info(...) do{ user_debug_print(3, __MODULE__, __LINE__, __VA_ARGS__); }while(0)
#define INFO(...) do{ user_debug_print(3, __MODULE__, __LINE__, __VA_ARGS__); }while(0)
#define LOG(...) do{ user_debug_print(3, __MODULE__, __LINE__, __VA_ARGS__); }while(0)
// cannot use log due to its dupplicated with log() in math.h
#else
#define info(...) 
#define INFO(...) 
#define LOG(...) 
#endif

#if __DEBUG__ >= 4
#define DEBUG(...) do{ user_debug_print(4, __MODULE__, __LINE__, __VA_ARGS__); }while(0)
#define DEBUGX(...) do{ user_debug_print_exact(__VA_ARGS__); }while(0)
#define DEBUG_NATIVE(str, len) user_debug_native(str, len)
// Backward compatible
#define debug(...) do{ user_debug_print(4, __MODULE__, __LINE__, __VA_ARGS__); }while(0)
#define debugx(...) do{ user_debug_print_exact(__VA_ARGS__); }while(0)
#define debug_native(str, len) user_debug_native(str, len)
#else
#define DEBUG(...) 
#define DEBUGX(...) 
#define DEBUG_NATIVE(str, len) 
// Backward compatible
#define debug(...) 
#define debugx(...) 
#define debug_native(str, len)
#endif

#else // DEBUG_SERIAL

#define DEBUG(...)
#define debug(...)
#define DEBUGX(...) 
#define debugx(...)
#define DEBUG_NATIVE(str, len)
#define debug_native(str, len) 
#define INFO(...)
#define info(...)
#define WARN(...)
#define warn(...)
#define ERROR(...)
#define error(...)

#endif // DEBUG_SERIAL

// #ifdef __cplusplus
// }
// #endif

#endif
