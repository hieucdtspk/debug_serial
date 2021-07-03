/*
    Test debug serial lib.

    03/07/2021
    HieuNT
*/
#define __MODULE__ __FILE__
#define __DEBUG__ 4
#define DEBUG_SERIAL Serial
#include <debug_serial.h>

char nativeString[] = "Hello DEBUG for string/buffer with len style\n";

void setup()
{
    
    Serial.begin(115200);
    debug_serial_init();
    Serial.println("Hello print\n");
    DEBUG("Hello DEBUG\n");
    INFO("Hello INFO\n");
    WARN("Hello WARN\n");
    ERROR("Hello ERROR\n");

    DEBUGX("Hello DEBUG without extra infomation: %u\n", millis());
    DEBUG_NATIVE(nativeString, strlen(nativeString));
}

void loop()
{
    static uint32_t tm = 0;
    if (millis() - tm >= 1000){
        tm = millis();
        DEBUG("Tick: %u\n", tm);
    }
}
