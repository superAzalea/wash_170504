#ifndef __buzz_H
#define __buzz_H

#ifdef __buzz_C	  

u8 flag_buzz = 0;

void sub_buzz(void);

#else

extern u8 flag_buzz;

extern void sub_buzz(void);

#endif
#endif

