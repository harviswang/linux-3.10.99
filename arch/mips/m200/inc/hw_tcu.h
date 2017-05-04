/* 
 * File:   hw_tcu.h
 * Author: Harvis Wang <jiankangshiye@aliyun.com>
 *
 * Created on December 19, 2016, 1:57 PM
 */

#ifndef HW_TCU_H
#define HW_TCU_H

#ifdef __cplusplus
extern "C" {
#endif

#define TCU 0xB0002000 /* Address base of TCU */
#define TCU_TESR 0x014 /* Timer Counter Enable Set Register */
#define TCU_TECR 0x018 /* Timer Counter Enable Clear Register */
#define TCU_TSR 0x01C /* Timer Stop Register */
#define TCU_TFR 0x020 /* Timer Flag Register */
#define TCU_TFSR 0x024 /* Time Flag Set Register */
#define TCU_TFCR 0x028 /* Timer Flag Clear Register */
#define TCU_TMR 0x030 /* Timer Mask Register */
#define TCU_TMSR 0x034 /* Timer Mask Set Register */
#define TCU_TMCR 0x038 /* Timer Mask Clear Register */
#define TCU_TFSR 0x024 /* Timer Flag Set Register */

#ifdef __cplusplus
}
#endif

#endif /* HW_TCU_H */

