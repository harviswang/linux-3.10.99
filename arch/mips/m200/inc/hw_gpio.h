//*****************************************************************************
//
// hw_gpio.h - Macros used when accessing the GPIO hardware.
//
// Copyright (c) 2017 Harvis Wang.  All rights reserved.
// Software License Agreement
// 
//   Redistribution and use in source and binary forms, with or without
//   modification, are permitted provided that the following conditions
//   are met:
// 
//   Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the  
//   distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************

#ifndef __HW_GPIO_H__
#define __HW_GPIO_H__

//*****************************************************************************
//
// The following are defines for the GPIO register offsets.
//
//*****************************************************************************
#define GPIO_O_PIN              0x00000000  // PIN level register
#define GPIO_O_INT              0x00000010  // PIN interrupt register
#define GPIO_O_INTS             0x00000014  // PIN interrupt set register
#define GPIO_O_INTC             0x00000018  // PIN interrupt clear register
#define GPIO_O_MASK             0x00000020  // PIN interrupt mask register
#define GPIO_O_MASKS            0x00000024  // PIN interrupt mask set register
#define GPIO_O_MASKC            0x00000028  // PIN interrupt mask clear register
#define GPIO_O_PAT1             0x00000030  // PIN patten 1 register
#define GPIO_O_PAT1S            0x00000034  // PIN patten 1 set register
#define GPIO_O_PAT1C            0x00000038  // PIN patten 1 clear register
#define GPIO_O_PAT0             0x00000040  // PIN patten 0 register
#define GPIO_O_PAT0S            0x00000044  // PIN patten 0 set register
#define GPIO_O_PAT0C            0x00000048  // PIN patten 0 clear register
#define GPIO_O_FLG              0x00000050  // PIN flag register
#define GPIO_O_FLGC             0x00000058  // PIN flag clear register
#define GPIO_O_PEN              0x00000070  // PIN pull(down or up not both) disable register
#define GPIO_O_PENS             0x00000074  // PIN pull disable set register
#define GPIO_O_PENC             0x00000078  // PIN pull disable clear register
#define GPIO_O_PGZIDLD          0x000000B0  // Group ID to be load
#define GPIO_O_PZACFG0          0x000000C0  // Glitch filter configure register 0 for port PA
#define GPIO_O_PZBCFG0          0x000000D0  // Glitch filter configure register 0 for port PB
#define GPIO_O_PZCCFG0          0x000000E0  // Glitch filter configure register 0 for port PC
#define GPIO_O_PZDCFG0          0x000000F0  // Glitch filter configure register 0 for port PD
#define GPIO_O_PZECFG0          0x0000000C  // Glitch filter configure register 0 for port PE
#define GPIO_O_PZFCFG0          0x0000004C  // Glitch filter configure register 0 for port PF

#define GPIO_O_PZACFG1          0x000000C4  // Glitch filter configure register 1 for port PA
#define GPIO_O_PZBCFG1          0x000000D4  // Glitch filter configure register 1 for port PB
#define GPIO_O_PZCCFG1          0x000000E4  // Glitch filter configure register 1 for port PC
#define GPIO_O_PZDCFG1          0x000000F4  // Glitch filter configure register 1 for port PD
#define GPIO_O_PZECFG1          0x0000001C  // Glitch filter configure register 1 for port PE
#define GPIO_O_PZFCFG1          0x0000005C  // Glitch filter configure register 1 for port PF

#define GPIO_O_PZACFG2          0x000000C8  // Glitch filter configure register 2 for port PA
#define GPIO_O_PZBCFG2          0x000000D8  // Glitch filter configure register 2 for port PB
#define GPIO_O_PZCCFG2          0x000000E8  // Glitch filter configure register 2 for port PC
#define GPIO_O_PZDCFG2          0x000000F8  // Glitch filter configure register 2 for port PD
#define GPIO_O_PZECFG2          0x0000002C  // Glitch filter configure register 2 for port PE
#define GPIO_O_PZFCFG2          0x0000006C  // Glitch filter configure register 2 for port PF

#define GPIO_O_PZACFG3          0x000000CC  // Glitch filter configure register 3 for port PA
#define GPIO_O_PZBCFG3          0x000000DC  // Glitch filter configure register 3 for port PB
#define GPIO_O_PZCCFG3          0x000000EC  // Glitch filter configure register 3 for port PC
#define GPIO_O_PZDCFG3          0x000000FC  // Glitch filter configure register 3 for port PD
#define GPIO_O_PZECFG3          0x0000003C  // Glitch filter configure register 3 for port PE
#define GPIO_O_PZFCFG3          0x0000007C  // Glitch filter configure register 3 for port PF



//*****************************************************************************
//
// The following are defines for the bit fields in the _O_ register.
//
//*****************************************************************************

#endif /* __HW_GPIO_H__ */

