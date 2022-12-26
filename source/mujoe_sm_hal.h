/*
 * mujoe_sm_hal.h
 *
 *      Author: Joe Corteo
 *      Source: https://github.com/muJoseph/mujoe_sm
 */

#ifndef MUJOE_SM_HAL_H_
#define MUJOE_SM_HAL_H_

//////////////////////////////////////////////////////////////////////////////////////////////
//  INCLUDE
/////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
//  TYPEDEFS
/////////////////////////////////////////////////////////////////////////////////////////////

// NOTE: It is recommended to NOT modify the following typedefs
typedef unsigned char     mujoe_sm_s_index_t;
typedef unsigned short    mujoe_sm_s_code_t;

//////////////////////////////////////////////////////////////////////////////////////////////
//  CONSTANTS
/////////////////////////////////////////////////////////////////////////////////////////////

// Reserved codes
// NOTE: Host must ensure that user-defined codes do not match any of the following driver-reserved
// codes!
#define MUJOE_SM_RET_CODE_NONE                (mujoe_sm_s_code_t)0x0000
#define MUJOE_SM_RET_CODE_ERROR               (mujoe_sm_s_code_t)0x0800
#define MUJOE_SM_RET_CODE_NOTI_HOST           (mujoe_sm_s_code_t)0x1000
#define MUJOE_SM_RET_CODE_TIMEOUT             (mujoe_sm_s_code_t)0x2000
#define MUJOE_SM_RET_CODE_EXT_TRIG_RXD        (mujoe_sm_s_code_t)0x4000
#define MUJOE_SM_RET_CODE_AWAIT_EXT_TRIG      (mujoe_sm_s_code_t)0x8000

//////////////////////////////////////////////////////////////////////////////////////////////
//  MACROS
/////////////////////////////////////////////////////////////////////////////////////////////

// TODO: Host must implement these macros
#define MUJOE_SM_MEMSET(PTR,X,LEN)			
#define MUJOE_SM_MEMCPY(DST,SRC,LEN)	    
#define MUJOE_SM_MALLOC(N)                  
#define MUJOE_SM_FREE(PTR)                  

#endif /* MUJOE_SM_HAL_H_ */
