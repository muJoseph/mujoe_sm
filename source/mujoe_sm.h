/*
 * mujoe_sm.h
 *
 *      Author: Joe Corteo
 *      Source: https://github.com/muJoseph/mujoe_sm
 */

#ifndef MUJOE_SM_H_
#define MUJOE_SM_H_

//////////////////////////////////////////////////////////////////////////////////////////////
//  INCLUDE
/////////////////////////////////////////////////////////////////////////////////////////////

#include "mujoe_sm_hal.h"

//////////////////////////////////////////////////////////////////////////////////////////////
//  TYPES
/////////////////////////////////////////////////////////////////////////////////////////////

typedef int mujoe_sm_s_time_ms_t;

// State machine driver error codes
typedef enum
{
	MUJOE_SM_ERR_NONE 				= 0,
	MUJOE_SM_ERR_INV_PARAM			= 1,
	MUJOE_SM_ERR_MALLOC_FAILED   	= 2,
	MUJOE_SM_ERR_NOT_IMPLEMENTED   	= 3,
	MUJOE_SM_ERR_NOT_SUPPORTED  	= 4,
	MUJOE_SM_ERR_INV_TRANS			= 5,
	MUJOE_SM_ERR_TIMED_OUT			= 6,
	MUJOE_SM_ERR_FAILURE 			= 0xFF,

}mujoe_sm_err_t;

// State machine transition table entry structure
typedef struct _mujoe_sm_trans
{
   mujoe_sm_s_index_t          curr;
   mujoe_sm_s_code_t           code;
   mujoe_sm_s_index_t          next;

}mujoe_sm_trans_t;

// State machine state handler return structure
typedef struct _mujoe_sm_s_ret
{
   mujoe_sm_s_code_t         code;
   mujoe_sm_s_time_ms_t      time_ms;

}mujoe_sm_s_ret;

// State machine state handler FP
typedef mujoe_sm_s_ret (*mujoe_sm_state_Fp_t)( void* pHostVar );

typedef union
{
	mujoe_sm_trans_t		invalidTrans;
	mujoe_sm_s_index_t      stateTimedOut;

}mujoe_sm_err_arg_params_u;

typedef struct _mujoe_sm_err_arg
{
	mujoe_sm_err_t				err;
	mujoe_sm_err_arg_params_u	params;

}mujoe_sm_err_arg_t;

// State machine error callback
typedef void (*mujoe_sm_error_Fp_t)( mujoe_sm_err_arg_t *pArg );

/**
 * @brief       Host scheduler transition callback function.
 *
 *
 * @param       delay_ms: Delay of transition in ms if non-zero. If -1,
 *                        pending transition is cancelled.
 *
 * @return  MUJOE_SM_ERR_NONE       - If transition scheduled successfully
 * @return  MUJOE_SM_ERR_FAILURE    - For all failure cases
 */
typedef mujoe_sm_err_t (*mujoe_sm_trans_Fp_t)( mujoe_sm_s_time_ms_t delay_ms );

// Host callback structure
typedef struct _mujoe_sm_Cbs
{
	mujoe_sm_error_Fp_t			error;
	mujoe_sm_trans_Fp_t			trans;

}mujoe_sm_Cbs_t;

// State machine context configuration structure
typedef struct _mujoe_sm_cfg
{
	mujoe_sm_state_Fp_t                *pStateTbl;
    int                                 numStates;
    mujoe_sm_trans_t                   *pTransTbl;
    int                                 numTrans;
    void								*pHostVar;

}mujoe_sm_cfg_t;

// State machine internal run-time variables
typedef struct _mujoe_sm_rtVar
{
	mujoe_sm_s_index_t		currState;
	mujoe_sm_s_index_t		lastState;
	mujoe_sm_s_code_t		currCode;
	mujoe_sm_s_code_t		lastCode;
	mujoe_sm_s_ret          ret;

}mujoe_sm_rtVar_t;

// State machine context structure
typedef struct _mujoe_sm
{
	mujoe_sm_cfg_t 			cfg;
	mujoe_sm_Cbs_t			*pCBs;
	mujoe_sm_rtVar_t		rt;


}mujoe_sm_t, *MUJOE_SM_CTX;

//////////////////////////////////////////////////////////////////////////////////////////////
//  API
/////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief       Initializes State machine context.
 *
 * @note        Must be called first!
 *
 * @param       ctx: Pointer to state machine context data structure
 * @param       pCfg: Pointer to state machine context configuration structure
 *
 */
extern void mujoe_sm_initCtx( MUJOE_SM_CTX ctx, mujoe_sm_cfg_t *pCfg );

/**
 * @brief       Registers host callback structure with state machine context.
 *
 *
 * @param       ctx: Pointer to state machine context data structure
 * @param       pCBs: Pointer to host callback function structure
 *
 * @return  MUJOE_SM_ERR_NONE
 * @return  MUJOE_SM_ERR_INV_PARAM
 */
extern mujoe_sm_err_t mujoe_sm_registerCallbacks( MUJOE_SM_CTX ctx, mujoe_sm_Cbs_t *pCBs );

/**
 * @brief      Triggers state machine transition via an external source.
 *
 *
 * @param       ctx: Pointer to state machine context data structure
 * @param       code: Code that is being awaited by state machine
 *
 * @return  MUJOE_SM_ERR_NONE
 * @return  MUJOE_SM_ERR_FAILURE
 */
extern mujoe_sm_err_t mujoe_sm_trigger( MUJOE_SM_CTX ctx, mujoe_sm_s_code_t code );

/**
 * @brief      Start state machine.
 *
 * @param       ctx: Pointer to state machine context data structure
 * @param       delay_ms: Delay before starting state machine (ms)
 *
 * @return  MUJOE_SM_ERR_NONE
 * @return  MUJOE_SM_ERR_FAILURE
 */
extern mujoe_sm_err_t mujoe_sm_start( MUJOE_SM_CTX ctx, mujoe_sm_s_time_ms_t delay_ms );

/**
 * @brief      Run the state machine.
 *
 * @note       Called by the application scheduler when events are raised by
 *             invoking mujoe_sm_trans_Fp_t callback.
 *
 * @param       ctx: Pointer to state machine context data structure
 *
 * @return  MUJOE_SM_ERR_NONE
 * @return  MUJOE_SM_ERR_FAILURE
 * @return  MUJOE_SM_ERR_NOT_IMPLEMENTED
 *
 */
extern mujoe_sm_err_t mujoe_sm_run( MUJOE_SM_CTX ctx );

#endif /* MUJOE_SM_H_ */
