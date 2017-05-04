#ifndef __Sci_H
#define __Sci_H
#ifdef __Sci_C

void Sci(void);

unsigned int AirPressure,AirTemp,RunState,Power;

#else

extern unsigned int AirPressure,AirTemp,RunState,Power;
  
extern void Sci(void);

#endif
#endif


