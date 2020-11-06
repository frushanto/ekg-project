/* Copyright (C) 1999-2001 Quality Quorum, Inc.
 
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
 
     1. Redistributions of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.
     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
     3. The name of the author may not be used to endorse or promote products
        derived from this software without specific prior written permission.
 
   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
   EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
   OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
   ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 
   QQI can be contacted as qqi@world.std.com
  
   Definitions of the serial connection on the target side.
   
  
   $Id: serial.h,v 1.2 2003/12/11 12:42:23 XHKJAMES Exp $ */

#ifndef _SERIAL_H_
#define _SERIAL_H_

#if defined(__cplusplus)
extern "C"
{
#endif
    
/* Check whather baud rate is acceptable */
int  serial_check_baud(int baud);

/* Open connection */
int  serial_open(char *dev, int baud, int parity, int bits);

/* Close connection */
void serial_close(int port);

/* Receive character with timeout in milliseconds */
int serial_getchar(int port, int timeout);

/* Receive character with timeout in milliseconds */
int serial_read(int port, char *buf, size_t len, int timeout);

/* Return values for serial_read_char */
#define RP_VAL_SERIALGETCHARRET_TMOUT (-2)
#define RP_VAL_SERIALGETCHARRET_ERR   (-1)

/* Send character */
void serial_putchar(int port, int c);

/* Send buffer */
int serial_write(int port, char *buf, size_t len);

/* Flush input */
void serial_flushinput(int port);

/* Send break */
void serial_sendbreak(int port);

#if defined(__cplusplus)
};
#endif

#endif /* _SERIAL_H_ */
