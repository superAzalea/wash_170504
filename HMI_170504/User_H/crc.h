/**************************************************************************************************
Project: TTK_ACPR
Filename: CRC.h
Description: time interruput functions for air compressor.
Complier: Keil uVision5.
***************************************************************************************************
Copyright (c) 2014 TTK.
All rights reserved.
***************************************************************************************************
DESCRIPTION:
Revision History:
Original Author:JesonsSun
First edition Time:20140601
Last Review Time:20140701 11:11;V1.0
**************************************************************************************************/
#ifndef __CRC_H
#define __CRC_H
#ifdef __CRC_C	  

u16 crc16(u8 *puchMsg, u16 usDataLen);

#else

extern u16 crc16(u8 *puchMsg, u16 usDataLen);

#endif
#endif


