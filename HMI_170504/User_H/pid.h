#ifndef __PID_H
#define __PID_H
#ifdef __PID_C

int16 PidCtrl_ps1(u16 Xref,u16 Xact,u16 Kp, u16 Ki,int16 UpperLim,int16 LowerLim);

#else

extern int16 PidCtrl_ps1(u16 Xref,u16 Xact,u16 Kp, u16 Ki,int16 UpperLim,int16 LowerLim);

#endif		 				    
#endif

