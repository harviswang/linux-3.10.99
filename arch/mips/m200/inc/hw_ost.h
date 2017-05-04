/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   hw_ost.h
 * Author: ingenic
 *
 * Created on December 19, 2016, 1:55 PM
 */

#ifndef HW_OST_H
#define HW_OST_H

#ifdef __cplusplus
extern "C" {
#endif

#define OST 0xB0002000 /* Address base of OST TODO */
#define OST_DR   0x0E0 /* OST Data Reigster */
#define OST_CNTL 0x0E4 /* OST Counter Lower 32 Bits */
#define OST_CNTH 0x0E8 /* OST Counter Higher 32 Bits */
#define OST_CSR  0x0EC /* OST Control Register */
#define OST_CNTHBUF 0x0FC /* OST Counter Higher 32 Bits Buffer */

/* function protocol */
void ost_time_init(void);

#ifdef __cplusplus
}
#endif

#endif /* HW_OST_H */

