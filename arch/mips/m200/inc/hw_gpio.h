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
#define GPIO_O_FLAG             0x00000050  // PIN flag register
#define GPIO_O_FLAGC            0x00000058  // PIN flag clear register
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
// The following are defines for the bit fields in the GPIO_O_PIN register.
//
//*****************************************************************************
#define GPIO_PIN_PNL31          0x80000000  // 31 PIN level
                                            // 0: Corresponding pin voltage level is low
                                            // 1: Corresponding pin voltage level is high
#define GPIO_PIN_PNL30          0x40000000  // 30 PIN level
#define GPIO_PIN_PNL29          0x20000000  // 29 PIN level
#define GPIO_PIN_PNL28          0x10000000  // 28 PIN level
#define GPIO_PIN_PNL27          0x08000000  // 27 PIN level
#define GPIO_PIN_PNL26          0x04000000  // 26 PIN level
#define GPIO_PIN_PNL25          0x02000000  // 25 PIN level
#define GPIO_PIN_PNL24          0x01000000  // 24 PIN level
#define GPIO_PIN_PNL23          0x00800000  // 23 PIN level
#define GPIO_PIN_PNL22          0x00400000  // 22 PIN level
#define GPIO_PIN_PNL21          0x00200000  // 21 PIN level
#define GPIO_PIN_PNL20          0x00100000  // 20 PIN level
#define GPIO_PIN_PNL19          0x00080000  // 19 PIN level
#define GPIO_PIN_PNL18          0x00040000  // 18 PIN level
#define GPIO_PIN_PNL17          0x00020000  // 17 PIN level
#define GPIO_PIN_PNL16          0x00010000  // 16 PIN level
#define GPIO_PIN_PNL15          0x00008000  // 15 PIN level
#define GPIO_PIN_PNL14          0x00004000  // 14 PIN level
#define GPIO_PIN_PNL13          0x00002000  // 13 PIN level
#define GPIO_PIN_PNL12          0x00001000  // 12 PIN level
#define GPIO_PIN_PNL11          0x00000800  // 11 PIN level
#define GPIO_PIN_PNL10          0x00000400  // 10 PIN level
#define GPIO_PIN_PNL9           0x00000200  // 9 PIN level
#define GPIO_PIN_PNL8           0x00000100  // 8 PIN level
#define GPIO_PIN_PNL7           0x00000080  // 7 PIN level
#define GPIO_PIN_PNL6           0x00000040  // 6 PIN level
#define GPIO_PIN_PNL5           0x00000020  // 5 PIN level
#define GPIO_PIN_PNL4           0x00000010  // 4 PIN level
#define GPIO_PIN_PNL3           0x00000008  // 3 PIN level
#define GPIO_PIN_PNL2           0x00000004  // 2 PIN level
#define GPIO_PIN_PNL1           0x00000002  // 1 PIN level
#define GPIO_PIN_PNL0           0x00000001  // 0 PIN level

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_INT register.
//
//*****************************************************************************
#define GPIO_INT_INT31          0x80000000  // 31 Interrupt enable
                                            // 0: Corresponding pin is used as device functions or normal gpio
                                            // 1: Corresponding pin is used as interrupt
#define GPIO_INT_INT30          0x40000000  // 30 Interrupt enable
#define GPIO_INT_INT29          0x20000000  // 29 Interrupt enable
#define GPIO_INT_INT28          0x10000000  // 28 Interrupt enable
#define GPIO_INT_INT27          0x08000000  // 27 Interrupt enable
#define GPIO_INT_INT26          0x04000000  // 26 Interrupt enable
#define GPIO_INT_INT25          0x02000000  // 25 Interrupt enable
#define GPIO_INT_INT24          0x01000000  // 24 Interrupt enable
#define GPIO_INT_INT23          0x00800000  // 23 Interrupt enable
#define GPIO_INT_INT22          0x00400000  // 22 Interrupt enable
#define GPIO_INT_INT21          0x00200000  // 21 Interrupt enable
#define GPIO_INT_INT20          0x00100000  // 20 Interrupt enable
#define GPIO_INT_INT19          0x00080000  // 19 Interrupt enable
#define GPIO_INT_INT18          0x00040000  // 18 Interrupt enable
#define GPIO_INT_INT17          0x00020000  // 17 Interrupt enable
#define GPIO_INT_INT16          0x00010000  // 16 Interrupt enable
#define GPIO_INT_INT15          0x00008000  // 15 Interrupt enable
#define GPIO_INT_INT14          0x00004000  // 14 Interrupt enable
#define GPIO_INT_INT13          0x00002000  // 13 Interrupt enable
#define GPIO_INT_INT12          0x00001000  // 12 Interrupt enable
#define GPIO_INT_INT11          0x00000800  // 11 Interrupt enable
#define GPIO_INT_INT10          0x00000400  // 10 Interrupt enable
#define GPIO_INT_INT9           0x00000200  // 9 Interrupt enable
#define GPIO_INT_INT8           0x00000100  // 8 Interrupt enable
#define GPIO_INT_INT7           0x00000080  // 7 Interrupt enable
#define GPIO_INT_INT6           0x00000040  // 6 Interrupt enable
#define GPIO_INT_INT5           0x00000020  // 5 Interrupt enable
#define GPIO_INT_INT4           0x00000010  // 4 Interrupt enable
#define GPIO_INT_INT3           0x00000008  // 3 Interrupt enable
#define GPIO_INT_INT2           0x00000004  // 2 Interrupt enable
#define GPIO_INT_INT1           0x00000002  // 1 Interrupt enable
#define GPIO_INT_INT0           0x00000001  // 0 Interrupt enable

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_INTS register.
//
//*****************************************************************************
#define GPIO_INTS_INTS31        0x80000000  // 31 Interrupt enable set
#define GPIO_INTS_INTS30        0x40000000  // 30 Interrupt enable set
#define GPIO_INTS_INTS29        0x20000000  // 29 Interrupt enable set
#define GPIO_INTS_INTS28        0x10000000  // 28 Interrupt enable set
#define GPIO_INTS_INTS27        0x08000000  // 27 Interrupt enable set
#define GPIO_INTS_INTS26        0x04000000  // 26 Interrupt enable set
#define GPIO_INTS_INTS25        0x02000000  // 25 Interrupt enable set
#define GPIO_INTS_INTS24        0x01000000  // 24 Interrupt enable set
#define GPIO_INTS_INTS23        0x00800000  // 23 Interrupt enable set
#define GPIO_INTS_INTS22        0x00400000  // 22 Interrupt enable set
#define GPIO_INTS_INTS21        0x00200000  // 21 Interrupt enable set
#define GPIO_INTS_INTS20        0x00100000  // 20 Interrupt enable set
#define GPIO_INTS_INTS19        0x00080000  // 19 Interrupt enable set
#define GPIO_INTS_INTS18        0x00040000  // 18 Interrupt enable set
#define GPIO_INTS_INTS17        0x00020000  // 17 Interrupt enable set
#define GPIO_INTS_INTS16        0x00010000  // 16 Interrupt enable set
#define GPIO_INTS_INTS15        0x00008000  // 15 Interrupt enable set
#define GPIO_INTS_INTS14        0x00004000  // 14 Interrupt enable set
#define GPIO_INTS_INTS13        0x00002000  // 13 Interrupt enable set
#define GPIO_INTS_INTS12        0x00001000  // 12 Interrupt enable set
#define GPIO_INTS_INTS11        0x00000800  // 11 Interrupt enable set
#define GPIO_INTS_INTS10        0x00000400  // 10 Interrupt enable set
#define GPIO_INTS_INTS9         0x00000200  // 9 Interrupt enable set
#define GPIO_INTS_INTS8         0x00000100  // 8 Interrupt enable set
#define GPIO_INTS_INTS7         0x00000080  // 7 Interrupt enable set
#define GPIO_INTS_INTS6         0x00000040  // 6 Interrupt enable set
#define GPIO_INTS_INTS5         0x00000020  // 5 Interrupt enable set
#define GPIO_INTS_INTS4         0x00000010  // 4 Interrupt enable set
#define GPIO_INTS_INTS3         0x00000008  // 3 Interrupt enable set
#define GPIO_INTS_INTS2         0x00000004  // 2 Interrupt enable set
#define GPIO_INTS_INTS1         0x00000002  // 1 Interrupt enable set
#define GPIO_INTS_INTS0         0x00000001  // 0 Interrupt enable set

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_INTC register.
//
//*****************************************************************************
#define GPIO_INTC_INTC31        0x80000000  // 31 Interrupt enable clear
#define GPIO_INTC_INTC30        0x40000000  // 30 Interrupt enable clear
#define GPIO_INTC_INTC29        0x20000000  // 29 Interrupt enable clear
#define GPIO_INTC_INTC28        0x10000000  // 28 Interrupt enable clear
#define GPIO_INTC_INTC27        0x08000000  // 27 Interrupt enable clear
#define GPIO_INTC_INTC26        0x04000000  // 26 Interrupt enable clear
#define GPIO_INTC_INTC25        0x02000000  // 25 Interrupt enable clear
#define GPIO_INTC_INTC24        0x01000000  // 24 Interrupt enable clear
#define GPIO_INTC_INTC23        0x00800000  // 23 Interrupt enable clear
#define GPIO_INTC_INTC22        0x00400000  // 22 Interrupt enable clear
#define GPIO_INTC_INTC21        0x00200000  // 21 Interrupt enable clear
#define GPIO_INTC_INTC20        0x00100000  // 20 Interrupt enable clear
#define GPIO_INTC_INTC19        0x00080000  // 19 Interrupt enable clear
#define GPIO_INTC_INTC18        0x00040000  // 18 Interrupt enable clear
#define GPIO_INTC_INTC17        0x00020000  // 17 Interrupt enable clear
#define GPIO_INTC_INTC16        0x00010000  // 16 Interrupt enable clear
#define GPIO_INTC_INTC15        0x00008000  // 15 Interrupt enable clear
#define GPIO_INTC_INTC14        0x00004000  // 14 Interrupt enable clear
#define GPIO_INTC_INTC13        0x00002000  // 13 Interrupt enable clear
#define GPIO_INTC_INTC12        0x00001000  // 12 Interrupt enable clear
#define GPIO_INTC_INTC11        0x00000800  // 11 Interrupt enable clear
#define GPIO_INTC_INTC10        0x00000400  // 10 Interrupt enable clear
#define GPIO_INTC_INTC9         0x00000200  // 9 Interrupt enable clear
#define GPIO_INTC_INTC8         0x00000100  // 8 Interrupt enable clear
#define GPIO_INTC_INTC7         0x00000080  // 7 Interrupt enable clear
#define GPIO_INTC_INTC6         0x00000040  // 6 Interrupt enable clear
#define GPIO_INTC_INTC5         0x00000020  // 5 Interrupt enable clear
#define GPIO_INTC_INTC4         0x00000010  // 4 Interrupt enable clear
#define GPIO_INTC_INTC3         0x00000008  // 3 Interrupt enable clear
#define GPIO_INTC_INTC2         0x00000004  // 2 Interrupt enable clear
#define GPIO_INTC_INTC1         0x00000002  // 1 Interrupt enable clear
#define GPIO_INTC_INTC0         0x00000001  // 0 Interrupt enable clear

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_MASK register.
// When GPIO_INT_XXX=1
// 0: Enable interrupt function
// 1: Disable interrupt function or
// When GPIO_INT_XXX=0
// 0: Enable device function
// 1: Disable device function, as normal gpio
//
//*****************************************************************************
#define GPIO_MASK_MASK31        0x80000000  // 31 PIN function mask
#define GPIO_MASK_MASK30        0x40000000  // 30 PIN function mask
#define GPIO_MASK_MASK29        0x20000000  // 29 PIN function mask
#define GPIO_MASK_MASK28        0x10000000  // 28 PIN function mask
#define GPIO_MASK_MASK27        0x08000000  // 27 PIN function mask
#define GPIO_MASK_MASK26        0x04000000  // 26 PIN function mask
#define GPIO_MASK_MASK25        0x02000000  // 25 PIN function mask
#define GPIO_MASK_MASK24        0x01000000  // 24 PIN function mask
#define GPIO_MASK_MASK23        0x00800000  // 23 PIN function mask
#define GPIO_MASK_MASK22        0x00400000  // 22 PIN function mask
#define GPIO_MASK_MASK21        0x00200000  // 21 PIN function mask
#define GPIO_MASK_MASK20        0x00100000  // 20 PIN function mask
#define GPIO_MASK_MASK19        0x00080000  // 19 PIN function mask
#define GPIO_MASK_MASK18        0x00040000  // 18 PIN function mask
#define GPIO_MASK_MASK17        0x00020000  // 17 PIN function mask
#define GPIO_MASK_MASK16        0x00010000  // 16 PIN function mask
#define GPIO_MASK_MASK15        0x00008000  // 15 PIN function mask
#define GPIO_MASK_MASK14        0x00004000  // 14 PIN function mask
#define GPIO_MASK_MASK13        0x00002000  // 13 PIN function mask
#define GPIO_MASK_MASK12        0x00001000  // 12 PIN function mask
#define GPIO_MASK_MASK11        0x00000800  // 11 PIN function mask
#define GPIO_MASK_MASK10        0x00000400  // 10 PIN function mask
#define GPIO_MASK_MASK9         0x00000200  // 9 PIN function mask
#define GPIO_MASK_MASK8         0x00000100  // 8 PIN function mask
#define GPIO_MASK_MASK7         0x00000080  // 7 PIN function mask
#define GPIO_MASK_MASK6         0x00000040  // 6 PIN function mask
#define GPIO_MASK_MASK5         0x00000020  // 5 PIN function mask
#define GPIO_MASK_MASK4         0x00000010  // 4 PIN function mask
#define GPIO_MASK_MASK3         0x00000008  // 3 PIN function mask
#define GPIO_MASK_MASK2         0x00000004  // 2 PIN function mask
#define GPIO_MASK_MASK1         0x00000002  // 1 PIN function mask
#define GPIO_MASK_MASK0         0x00000001  // 0 PIN function mask

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_MASKS register.
//
//*****************************************************************************
#define GPIO_MASKS_MASKS31      0x80000000  // 31 PIN function mask set
#define GPIO_MASKS_MASKS30      0x40000000  // 30 PIN function mask set
#define GPIO_MASKS_MASKS29      0x20000000  // 29 PIN function mask set
#define GPIO_MASKS_MASKS28      0x10000000  // 28 PIN function mask set
#define GPIO_MASKS_MASKS27      0x08000000  // 27 PIN function mask set
#define GPIO_MASKS_MASKS26      0x04000000  // 26 PIN function mask set
#define GPIO_MASKS_MASKS25      0x02000000  // 25 PIN function mask set
#define GPIO_MASKS_MASKS24      0x01000000  // 24 PIN function mask set
#define GPIO_MASKS_MASKS23      0x00800000  // 23 PIN function mask set
#define GPIO_MASKS_MASKS22      0x00400000  // 22 PIN function mask set
#define GPIO_MASKS_MASKS21      0x00200000  // 21 PIN function mask set
#define GPIO_MASKS_MASKS20      0x00100000  // 20 PIN function mask set
#define GPIO_MASKS_MASKS19      0x00080000  // 19 PIN function mask set
#define GPIO_MASKS_MASKS18      0x00040000  // 18 PIN function mask set
#define GPIO_MASKS_MASKS17      0x00020000  // 17 PIN function mask set
#define GPIO_MASKS_MASKS16      0x00010000  // 16 PIN function mask set
#define GPIO_MASKS_MASKS15      0x00008000  // 15 PIN function mask set
#define GPIO_MASKS_MASKS14      0x00004000  // 14 PIN function mask set
#define GPIO_MASKS_MASKS13      0x00002000  // 13 PIN function mask set
#define GPIO_MASKS_MASKS12      0x00001000  // 12 PIN function mask set
#define GPIO_MASKS_MASKS11      0x00000800  // 11 PIN function mask set
#define GPIO_MASKS_MASKS10      0x00000400  // 10 PIN function mask set
#define GPIO_MASKS_MASKS9       0x00000200  // 9 PIN function mask set
#define GPIO_MASKS_MASKS8       0x00000100  // 8 PIN function mask set
#define GPIO_MASKS_MASKS7       0x00000080  // 7 PIN function mask set
#define GPIO_MASKS_MASKS6       0x00000040  // 6 PIN function mask set
#define GPIO_MASKS_MASKS5       0x00000020  // 5 PIN function mask set
#define GPIO_MASKS_MASKS4       0x00000010  // 4 PIN function mask set
#define GPIO_MASKS_MASKS3       0x00000008  // 3 PIN function mask set
#define GPIO_MASKS_MASKS2       0x00000004  // 2 PIN function mask set
#define GPIO_MASKS_MASKS1       0x00000002  // 1 PIN function mask set
#define GPIO_MASKS_MASKS0       0x00000001  // 0 PIN function mask set

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_MASKC register.
//
//*****************************************************************************
#define GPIO_MASKC_MASKC31      0x80000000  // 31 PIN function mask clear
#define GPIO_MASKC_MASKC30      0x40000000  // 30 PIN function mask clear
#define GPIO_MASKC_MASKC29      0x20000000  // 29 PIN function mask clear
#define GPIO_MASKC_MASKC28      0x10000000  // 28 PIN function mask clear
#define GPIO_MASKC_MASKC27      0x08000000  // 27 PIN function mask clear
#define GPIO_MASKC_MASKC26      0x04000000  // 26 PIN function mask clear
#define GPIO_MASKC_MASKC25      0x02000000  // 25 PIN function mask clear
#define GPIO_MASKC_MASKC24      0x01000000  // 24 PIN function mask clear
#define GPIO_MASKC_MASKC23      0x00800000  // 23 PIN function mask clear
#define GPIO_MASKC_MASKC22      0x00400000  // 22 PIN function mask clear
#define GPIO_MASKC_MASKC21      0x00200000  // 21 PIN function mask clear
#define GPIO_MASKC_MASKC20      0x00100000  // 20 PIN function mask clear
#define GPIO_MASKC_MASKC19      0x00080000  // 19 PIN function mask clear
#define GPIO_MASKC_MASKC18      0x00040000  // 18 PIN function mask clear
#define GPIO_MASKC_MASKC17      0x00020000  // 17 PIN function mask clear
#define GPIO_MASKC_MASKC16      0x00010000  // 16 PIN function mask clear
#define GPIO_MASKC_MASKC15      0x00008000  // 15 PIN function mask clear
#define GPIO_MASKC_MASKC14      0x00004000  // 14 PIN function mask clear
#define GPIO_MASKC_MASKC13      0x00002000  // 13 PIN function mask clear
#define GPIO_MASKC_MASKC12      0x00001000  // 12 PIN function mask clear
#define GPIO_MASKC_MASKC11      0x00000800  // 11 PIN function mask clear
#define GPIO_MASKC_MASKC10      0x00000400  // 10 PIN function mask clear
#define GPIO_MASKC_MASKC9       0x00000200  // 9 PIN function mask clear
#define GPIO_MASKC_MASKC8       0x00000100  // 8 PIN function mask clear
#define GPIO_MASKC_MASKC7       0x00000080  // 7 PIN function mask clear
#define GPIO_MASKC_MASKC6       0x00000040  // 6 PIN function mask clear
#define GPIO_MASKC_MASKC5       0x00000020  // 5 PIN function mask clear
#define GPIO_MASKC_MASKC4       0x00000010  // 4 PIN function mask clear
#define GPIO_MASKC_MASKC3       0x00000008  // 3 PIN function mask clear
#define GPIO_MASKC_MASKC2       0x00000004  // 2 PIN function mask clear
#define GPIO_MASKC_MASKC1       0x00000002  // 1 PIN function mask clear
#define GPIO_MASKC_MASKC0       0x00000001  // 0 PIN function mask clear

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_PAT1 register.
// When GPIO_INT_XXX=1
// 0: Level trigger interrupt function
// 1: Edge trigger interrupt function
// When GPIO_INT_XXX=0 and GPIO_MASK_XXX=0
// 0: Corresponding pin is used as device 0 or device 1 function
// 1: Corresponding pin is used as device 2 or device 3 function
// When GPIO_INT_XXX=0 and GPIO_MASK_XXX=1
// 0: Corresponding pin is used as gpio output
// 1: Corresponding pin is used as gpio input
//*****************************************************************************
#define GPIO_PAT1_PAT131        0x80000000  // 31 PIN pattern 1 status
#define GPIO_PAT1_PAT130        0x40000000  // 30 PIN pattern 1 status
#define GPIO_PAT1_PAT129        0x20000000  // 29 PIN pattern 1 status
#define GPIO_PAT1_PAT128        0x10000000  // 28 PIN pattern 1 status
#define GPIO_PAT1_PAT127        0x08000000  // 27 PIN pattern 1 status
#define GPIO_PAT1_PAT126        0x04000000  // 26 PIN pattern 1 status
#define GPIO_PAT1_PAT125        0x02000000  // 25 PIN pattern 1 status
#define GPIO_PAT1_PAT124        0x01000000  // 24 PIN pattern 1 status
#define GPIO_PAT1_PAT123        0x00800000  // 23 PIN pattern 1 status
#define GPIO_PAT1_PAT122        0x00400000  // 22 PIN pattern 1 status
#define GPIO_PAT1_PAT121        0x00200000  // 21 PIN pattern 1 status
#define GPIO_PAT1_PAT120        0x00100000  // 20 PIN pattern 1 status
#define GPIO_PAT1_PAT119        0x00080000  // 19 PIN pattern 1 status
#define GPIO_PAT1_PAT118        0x00040000  // 18 PIN pattern 1 status
#define GPIO_PAT1_PAT117        0x00020000  // 17 PIN pattern 1 status
#define GPIO_PAT1_PAT116        0x00010000  // 16 PIN pattern 1 status
#define GPIO_PAT1_PAT115        0x00008000  // 15 PIN pattern 1 status
#define GPIO_PAT1_PAT114        0x00004000  // 14 PIN pattern 1 status
#define GPIO_PAT1_PAT113        0x00002000  // 13 PIN pattern 1 status
#define GPIO_PAT1_PAT112        0x00001000  // 12 PIN pattern 1 status
#define GPIO_PAT1_PAT111        0x00000800  // 11 PIN pattern 1 status
#define GPIO_PAT1_PAT110        0x00000400  // 10 PIN pattern 1 status
#define GPIO_PAT1_PAT19         0x00000200  // 9 PIN pattern 1 status
#define GPIO_PAT1_PAT18         0x00000100  // 8 PIN pattern 1 status
#define GPIO_PAT1_PAT17         0x00000080  // 7 PIN pattern 1 status
#define GPIO_PAT1_PAT16         0x00000040  // 6 PIN pattern 1 status
#define GPIO_PAT1_PAT15         0x00000020  // 5 PIN pattern 1 status
#define GPIO_PAT1_PAT14         0x00000010  // 4 PIN pattern 1 status
#define GPIO_PAT1_PAT13         0x00000008  // 3 PIN pattern 1 status
#define GPIO_PAT1_PAT12         0x00000004  // 2 PIN pattern 1 status
#define GPIO_PAT1_PAT11         0x00000002  // 1 PIN pattern 1 status
#define GPIO_PAT1_PAT10         0x00000001  // 0 PIN pattern 1 status

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_PAT1S register.
//
//*****************************************************************************
#define GPIO_PAT1S_PAT1S31      0x80000000  // 31 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S30      0x40000000  // 30 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S29      0x20000000  // 29 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S28      0x10000000  // 28 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S27      0x08000000  // 27 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S26      0x04000000  // 26 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S25      0x02000000  // 25 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S24      0x01000000  // 24 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S23      0x00800000  // 23 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S22      0x00400000  // 22 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S21      0x00200000  // 21 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S20      0x00100000  // 20 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S19      0x00080000  // 19 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S18      0x00040000  // 18 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S17      0x00020000  // 17 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S16      0x00010000  // 16 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S15      0x00008000  // 15 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S14      0x00004000  // 14 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S13      0x00002000  // 13 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S12      0x00001000  // 12 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S11      0x00000800  // 11 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S10      0x00000400  // 10 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S9       0x00000200  // 9 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S8       0x00000100  // 8 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S7       0x00000080  // 7 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S6       0x00000040  // 6 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S5       0x00000020  // 5 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S4       0x00000010  // 4 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S3       0x00000008  // 3 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S2       0x00000004  // 2 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S1       0x00000002  // 1 PIN pattern 1 config set
#define GPIO_PAT1S_PAT1S0       0x00000001  // 0 PIN pattern 1 config set

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_PAT1C register.
//
//*****************************************************************************
#define GPIO_PAT1C_PAT1C31      0x80000000  // 31 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C30      0x40000000  // 30 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C29      0x20000000  // 29 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C28      0x10000000  // 28 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C27      0x08000000  // 27 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C26      0x04000000  // 26 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C25      0x02000000  // 25 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C24      0x01000000  // 24 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C23      0x00800000  // 23 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C22      0x00400000  // 22 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C21      0x00200000  // 21 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C20      0x00100000  // 20 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C19      0x00080000  // 19 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C18      0x00040000  // 18 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C17      0x00020000  // 17 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C16      0x00010000  // 16 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C15      0x00008000  // 15 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C14      0x00004000  // 14 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C13      0x00002000  // 13 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C12      0x00001000  // 12 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C11      0x00000800  // 11 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C10      0x00000400  // 10 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C9       0x00000200  // 9 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C8       0x00000100  // 8 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C7       0x00000080  // 7 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C6       0x00000040  // 6 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C5       0x00000020  // 5 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C4       0x00000010  // 4 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C3       0x00000008  // 3 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C2       0x00000004  // 2 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C1       0x00000002  // 1 PIN pattern 1 config clear
#define GPIO_PAT1C_PAT1C0       0x00000001  // 0 PIN pattern 1 config clear

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_PAT0 register.
// When GPIO_INT_XXX=1 and GPIO_PAT1_XXX=0
// 0: Pin is low level interrupt input
// 1: Pin is high level interrupt input
// When GPIO_INT_XXX=1 and GPIO_PAT1_XXX=1
// 0: Pin is falling edge triggered interrupt input
// 1: Pin is rising edge triggered interrupt input
// When GPIO_INT_XXX=0 and GPIO_MASK_XXX=0 and GPIO_PAT1_XXX=0
// 0: Pin is used as device 0 function
// 1: Pin is used as device 1 function
// When GPIO_INT_XXX=0 and GPIO_MASK_XXX=0 and GPIO_PAT1_XXX=1
// 0: Corresponding pin is used as device 2 function
// 1: Corresponding pin is used as device 3 function
// When GPIO_INT_XXX=0 and GPIO_MASK_XXX=1 and GPIO_PAT1_XXX=0
// 0: Corresponding pin is used as gpio output 0
// 1: Corresponding pin is used as gpio output 1
//*****************************************************************************
#define GPIO_PAT0_PAT031        0x80000000  // 31 PIN pattern 0 config
#define GPIO_PAT0_PAT030        0x40000000  // 30 PIN pattern 0 config
#define GPIO_PAT0_PAT029        0x20000000  // 29 PIN pattern 0 config
#define GPIO_PAT0_PAT028        0x10000000  // 28 PIN pattern 0 config
#define GPIO_PAT0_PAT027        0x08000000  // 27 PIN pattern 0 config
#define GPIO_PAT0_PAT026        0x04000000  // 26 PIN pattern 0 config
#define GPIO_PAT0_PAT025        0x02000000  // 25 PIN pattern 0 config
#define GPIO_PAT0_PAT024        0x01000000  // 24 PIN pattern 0 config
#define GPIO_PAT0_PAT023        0x00800000  // 23 PIN pattern 0 config
#define GPIO_PAT0_PAT022        0x00400000  // 22 PIN pattern 0 config
#define GPIO_PAT0_PAT021        0x00200000  // 21 PIN pattern 0 config
#define GPIO_PAT0_PAT020        0x00100000  // 20 PIN pattern 0 config
#define GPIO_PAT0_PAT019        0x00080000  // 19 PIN pattern 0 config
#define GPIO_PAT0_PAT018        0x00040000  // 18 PIN pattern 0 config
#define GPIO_PAT0_PAT017        0x00020000  // 17 PIN pattern 0 config
#define GPIO_PAT0_PAT016        0x00010000  // 16 PIN pattern 0 config
#define GPIO_PAT0_PAT015        0x00008000  // 15 PIN pattern 0 config
#define GPIO_PAT0_PAT014        0x00004000  // 14 PIN pattern 0 config
#define GPIO_PAT0_PAT013        0x00002000  // 13 PIN pattern 0 config
#define GPIO_PAT0_PAT012        0x00001000  // 12 PIN pattern 0 config
#define GPIO_PAT0_PAT011        0x00000800  // 11 PIN pattern 0 config
#define GPIO_PAT0_PAT010        0x00000400  // 10 PIN pattern 0 config
#define GPIO_PAT0_PAT09         0x00000200  // 9 PIN pattern 0 config
#define GPIO_PAT0_PAT08         0x00000100  // 8 PIN pattern 0 config
#define GPIO_PAT0_PAT07         0x00000080  // 7 PIN pattern 0 config
#define GPIO_PAT0_PAT06         0x00000040  // 6 PIN pattern 0 config
#define GPIO_PAT0_PAT05         0x00000020  // 5 PIN pattern 0 config
#define GPIO_PAT0_PAT04         0x00000010  // 4 PIN pattern 0 config
#define GPIO_PAT0_PAT03         0x00000008  // 3 PIN pattern 0 config
#define GPIO_PAT0_PAT02         0x00000004  // 2 PIN pattern 0 config
#define GPIO_PAT0_PAT01         0x00000002  // 1 PIN pattern 0 config
#define GPIO_PAT0_PAT00         0x00000001  // 0 PIN pattern 0 config

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_PAT0S register.
//
//*****************************************************************************
#define GPIO_PAT0S_PAT0S31      0x80000000  // 31 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S30      0x40000000  // 30 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S29      0x20000000  // 29 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S28      0x10000000  // 28 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S27      0x08000000  // 27 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S26      0x04000000  // 26 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S25      0x02000000  // 25 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S24      0x01000000  // 24 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S23      0x00800000  // 23 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S22      0x00400000  // 22 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S21      0x00200000  // 21 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S20      0x00100000  // 20 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S19      0x00080000  // 19 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S18      0x00040000  // 18 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S17      0x00020000  // 17 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S16      0x00010000  // 16 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S15      0x00008000  // 15 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S14      0x00004000  // 14 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S13      0x00002000  // 13 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S12      0x00001000  // 12 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S11      0x00000800  // 11 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S10      0x00000400  // 10 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S9       0x00000200  // 9 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S8       0x00000100  // 8 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S7       0x00000080  // 7 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S6       0x00000040  // 6 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S5       0x00000020  // 5 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S4       0x00000010  // 4 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S3       0x00000008  // 3 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S2       0x00000004  // 2 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S1       0x00000002  // 1 PIN pattern 0 config set
#define GPIO_PAT0S_PAT0S0       0x00000001  // 0 PIN pattern 0 config set

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_PAT0C register.
//
//*****************************************************************************
#define GPIO_PAT0C_PAT0C31      0x80000000  // 31 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C30      0x40000000  // 30 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C29      0x20000000  // 29 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C28      0x10000000  // 28 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C27      0x08000000  // 27 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C26      0x04000000  // 26 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C25      0x02000000  // 25 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C24      0x01000000  // 24 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C23      0x00800000  // 23 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C22      0x00400000  // 22 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C21      0x00200000  // 21 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C20      0x00100000  // 20 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C19      0x00080000  // 19 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C18      0x00040000  // 18 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C17      0x00020000  // 17 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C16      0x00010000  // 16 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C15      0x00008000  // 15 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C14      0x00004000  // 14 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C13      0x00002000  // 13 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C12      0x00001000  // 12 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C11      0x00000800  // 11 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C10      0x00000400  // 10 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C9       0x00000200  // 9 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C8       0x00000100  // 8 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C7       0x00000080  // 7 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C6       0x00000040  // 6 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C5       0x00000020  // 5 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C4       0x00000010  // 4 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C3       0x00000008  // 3 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C2       0x00000004  // 2 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C1       0x00000002  // 1 PIN pattern 0 config clear
#define GPIO_PAT0C_PAT0C0       0x00000001  // 0 PIN pattern 0 config clear

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_FLAG register.
//
//*****************************************************************************
#define GPIO_FLAG_FLAG31        0x80000000  // 31 PIN interrupt flag bit
#define GPIO_FLAG_FLAG30        0x40000000  // 30 PIN interrupt flag bit
#define GPIO_FLAG_FLAG29        0x20000000  // 29 PIN interrupt flag bit
#define GPIO_FLAG_FLAG28        0x10000000  // 28 PIN interrupt flag bit
#define GPIO_FLAG_FLAG27        0x08000000  // 27 PIN interrupt flag bit
#define GPIO_FLAG_FLAG26        0x04000000  // 26 PIN interrupt flag bit
#define GPIO_FLAG_FLAG25        0x02000000  // 25 PIN interrupt flag bit
#define GPIO_FLAG_FLAG24        0x01000000  // 24 PIN interrupt flag bit
#define GPIO_FLAG_FLAG23        0x00800000  // 23 PIN interrupt flag bit
#define GPIO_FLAG_FLAG22        0x00400000  // 22 PIN interrupt flag bit
#define GPIO_FLAG_FLAG21        0x00200000  // 21 PIN interrupt flag bit
#define GPIO_FLAG_FLAG20        0x00100000  // 20 PIN interrupt flag bit
#define GPIO_FLAG_FLAG19        0x00080000  // 19 PIN interrupt flag bit
#define GPIO_FLAG_FLAG18        0x00040000  // 18 PIN interrupt flag bit
#define GPIO_FLAG_FLAG17        0x00020000  // 17 PIN interrupt flag bit
#define GPIO_FLAG_FLAG16        0x00010000  // 16 PIN interrupt flag bit
#define GPIO_FLAG_FLAG15        0x00008000  // 15 PIN interrupt flag bit
#define GPIO_FLAG_FLAG14        0x00004000  // 14 PIN interrupt flag bit
#define GPIO_FLAG_FLAG13        0x00002000  // 13 PIN interrupt flag bit
#define GPIO_FLAG_FLAG12        0x00001000  // 12 PIN interrupt flag bit
#define GPIO_FLAG_FLAG11        0x00000800  // 11 PIN interrupt flag bit
#define GPIO_FLAG_FLAG10        0x00000400  // 10 PIN interrupt flag bit
#define GPIO_FLAG_FLAG9         0x00000200  // 9 PIN interrupt flag bit
#define GPIO_FLAG_FLAG8         0x00000100  // 8 PIN interrupt flag bit
#define GPIO_FLAG_FLAG7         0x00000080  // 7 PIN interrupt flag bit
#define GPIO_FLAG_FLAG6         0x00000040  // 6 PIN interrupt flag bit
#define GPIO_FLAG_FLAG5         0x00000020  // 5 PIN interrupt flag bit
#define GPIO_FLAG_FLAG4         0x00000010  // 4 PIN interrupt flag bit
#define GPIO_FLAG_FLAG3         0x00000008  // 3 PIN interrupt flag bit
#define GPIO_FLAG_FLAG2         0x00000004  // 2 PIN interrupt flag bit
#define GPIO_FLAG_FLAG1         0x00000002  // 1 PIN interrupt flag bit
#define GPIO_FLAG_FLAG0         0x00000001  // 0 PIN interrupt flag bit

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_FLAGC register.
//
//*****************************************************************************
#define GPIO_FLAGC_FLAGC31      0x80000000  // 31 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC30      0x40000000  // 30 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC29      0x20000000  // 29 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC28      0x10000000  // 28 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC27      0x08000000  // 27 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC26      0x04000000  // 26 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC25      0x02000000  // 25 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC24      0x01000000  // 24 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC23      0x00800000  // 23 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC22      0x00400000  // 22 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC21      0x00200000  // 21 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC20      0x00100000  // 20 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC19      0x00080000  // 19 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC18      0x00040000  // 18 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC17      0x00020000  // 17 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC16      0x00010000  // 16 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC15      0x00008000  // 15 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC14      0x00004000  // 14 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC13      0x00002000  // 13 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC12      0x00001000  // 12 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC11      0x00000800  // 11 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC10      0x00000400  // 10 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC9       0x00000200  // 9 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC8       0x00000100  // 8 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC7       0x00000080  // 7 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC6       0x00000040  // 6 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC5       0x00000020  // 5 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC4       0x00000010  // 4 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC3       0x00000008  // 3 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC2       0x00000004  // 2 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC1       0x00000002  // 1 PIN interrupt flag bit clear
#define GPIO_FLAGC_FLAGC0       0x00000001  // 0 PIN interrupt flag bit clear

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_PEN register.
//
//*****************************************************************************
#define GPIO_PEN_PULL31         0x80000000  // 31 PIN internal pull up or pull down
#define GPIO_PEN_PULL30         0x40000000  // 30 PIN internal pull up or pull down
#define GPIO_PEN_PULL29         0x20000000  // 29 PIN internal pull up or pull down
#define GPIO_PEN_PULL28         0x10000000  // 28 PIN internal pull up or pull down
#define GPIO_PEN_PULL27         0x08000000  // 27 PIN internal pull up or pull down
#define GPIO_PEN_PULL26         0x04000000  // 26 PIN internal pull up or pull down
#define GPIO_PEN_PULL25         0x02000000  // 25 PIN internal pull up or pull down
#define GPIO_PEN_PULL24         0x01000000  // 24 PIN internal pull up or pull down
#define GPIO_PEN_PULL23         0x00800000  // 23 PIN internal pull up or pull down
#define GPIO_PEN_PULL22         0x00400000  // 22 PIN internal pull up or pull down
#define GPIO_PEN_PULL21         0x00200000  // 21 PIN internal pull up or pull down
#define GPIO_PEN_PULL20         0x00100000  // 20 PIN internal pull up or pull down
#define GPIO_PEN_PULL19         0x00080000  // 19 PIN internal pull up or pull down
#define GPIO_PEN_PULL18         0x00040000  // 18 PIN internal pull up or pull down
#define GPIO_PEN_PULL17         0x00020000  // 17 PIN internal pull up or pull down
#define GPIO_PEN_PULL16         0x00010000  // 16 PIN internal pull up or pull down
#define GPIO_PEN_PULL15         0x00008000  // 15 PIN internal pull up or pull down
#define GPIO_PEN_PULL14         0x00004000  // 14 PIN internal pull up or pull down
#define GPIO_PEN_PULL13         0x00002000  // 13 PIN internal pull up or pull down
#define GPIO_PEN_PULL12         0x00001000  // 12 PIN internal pull up or pull down
#define GPIO_PEN_PULL11         0x00000800  // 11 PIN internal pull up or pull down
#define GPIO_PEN_PULL10         0x00000400  // 10 PIN internal pull up or pull down
#define GPIO_PEN_PULL9          0x00000200  // 9 PIN internal pull up or pull down
#define GPIO_PEN_PULL8          0x00000100  // 8 PIN internal pull up or pull down
#define GPIO_PEN_PULL7          0x00000080  // 7 PIN internal pull up or pull down
#define GPIO_PEN_PULL6          0x00000040  // 6 PIN internal pull up or pull down
#define GPIO_PEN_PULL5          0x00000020  // 5 PIN internal pull up or pull down
#define GPIO_PEN_PULL4          0x00000010  // 4 PIN internal pull up or pull down
#define GPIO_PEN_PULL3          0x00000008  // 3 PIN internal pull up or pull down
#define GPIO_PEN_PULL2          0x00000004  // 2 PIN internal pull up or pull down
#define GPIO_PEN_PULL1          0x00000002  // 1 PIN internal pull up or pull down
#define GPIO_PEN_PULL0          0x00000001  // 0 PIN internal pull up or pull down

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_PENS register.
//
//*****************************************************************************
#define GPIO_PENS_PULLS31       0x80000000  // 31 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS30       0x40000000  // 30 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS29       0x20000000  // 29 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS28       0x10000000  // 28 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS27       0x08000000  // 27 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS26       0x04000000  // 26 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS25       0x02000000  // 25 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS24       0x01000000  // 24 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS23       0x00800000  // 23 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS22       0x00400000  // 22 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS21       0x00200000  // 21 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS20       0x00100000  // 20 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS19       0x00080000  // 19 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS18       0x00040000  // 18 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS17       0x00020000  // 17 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS16       0x00010000  // 16 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS15       0x00008000  // 15 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS14       0x00004000  // 14 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS13       0x00002000  // 13 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS12       0x00001000  // 12 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS11       0x00000800  // 11 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS10       0x00000400  // 10 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS9        0x00000200  // 9 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS8        0x00000100  // 8 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS7        0x00000080  // 7 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS6        0x00000040  // 6 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS5        0x00000020  // 5 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS4        0x00000010  // 4 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS3        0x00000008  // 3 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS2        0x00000004  // 2 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS1        0x00000002  // 1 PIN internal pull up or pull down set
#define GPIO_PENS_PULLS0        0x00000001  // 0 PIN internal pull up or pull down set

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_PENC register.
//
//*****************************************************************************
#define GPIO_PENC_PULLC31       0x80000000  // 31 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC30       0x40000000  // 30 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC29       0x20000000  // 29 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC28       0x10000000  // 28 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC27       0x08000000  // 27 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC26       0x04000000  // 26 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC25       0x02000000  // 25 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC24       0x01000000  // 24 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC23       0x00800000  // 23 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC22       0x00400000  // 22 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC21       0x00200000  // 21 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC20       0x00100000  // 20 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC19       0x00080000  // 19 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC18       0x00040000  // 18 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC17       0x00020000  // 17 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC16       0x00010000  // 16 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC15       0x00008000  // 15 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC14       0x00004000  // 14 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC13       0x00002000  // 13 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC12       0x00001000  // 12 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC11       0x00000800  // 11 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC10       0x00000400  // 10 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC9        0x00000200  // 9 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC8        0x00000100  // 8 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC7        0x00000080  // 7 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC6        0x00000040  // 6 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC5        0x00000020  // 5 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC4        0x00000010  // 4 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC3        0x00000008  // 3 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC2        0x00000004  // 2 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC1        0x00000002  // 1 PIN internal pull up or pull down clear
#define GPIO_PENC_PULLC0        0x00000001  // 0 PIN internal pull up or pull down clear

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_PGZIDLD register.
//
//*****************************************************************************
#define GPIO_GPZIDLD_PORTG      0x00000006  // GPIO Port G load ID
#define GPIO_GPZIDLD_PORTF      0x00000005  // GPIO Port F load ID
#define GPIO_GPZIDLD_PORTE      0x00000004  // GPIO Port E load ID
#define GPIO_GPZIDLD_PORTD      0x00000003  // GPIO Port D load ID
#define GPIO_GPZIDLD_PORTC      0x00000002  // GPIO Port C load ID
#define GPIO_GPZIDLD_PORTB      0x00000001  // GPIO Port B load ID
#define GPIO_GPZIDLD_PORTA      0x00000000  // GPIO Port A load ID

#endif /* __HW_GPIO_H__ */

