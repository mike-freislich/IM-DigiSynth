#include <Arduino.h>
#include "Compat.h"
#include "init.h"

FLASHMEM void setup()
{
    initParameters();
    initModules();
}

FLASHMEM void loop()
{
    Modules.update();
}