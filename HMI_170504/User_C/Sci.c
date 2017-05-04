#ifndef __Sci_C
#define __Sci_C
#include "stm32f10x.h"
#include "Define.h" 
#include "Sci.h"

void Sci(void)//1ms
{   
    AirPressure++;
    if(AirPressure>=99)
    {
        AirPressure = 0; 
        RunState ^= 1;
    }
    AirPressure = 12;
    Power = AirTemp = AirPressure; 
/*    
    AirPressure = 28;
    AirTemp = 25;
    RunState = 1;
    Power = 12;
*/    
}
#endif




