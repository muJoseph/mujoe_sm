/*
 * mujoe_sm.c
 *
 *      Author: Joe Corteo
 *      Source: https://github.com/muJoseph/mujoe_sm
 *
 */

//////////////////////////////////////////////////////////////////////////////////////////////
//  INCLUDE
/////////////////////////////////////////////////////////////////////////////////////////////

#include "mujoe_sm.h"

//////////////////////////////////////////////////////////////////////////////////////////////
//  LOCAL FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////

static mujoe_sm_err_t stim_trans( MUJOE_SM_CTX ctx, mujoe_sm_s_time_ms_t ms );
static void reset_var( MUJOE_SM_CTX ctx );

//////////////////////////////////////////////////////////////////////////////////////////////
//  CONSTANTS
/////////////////////////////////////////////////////////////////////////////////////////////

#define MUJOE_SM_CODE_MASK              (mujoe_sm_s_code_t)0x00FF

//////////////////////////////////////////////////////////////////////////////////////////////
//  FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////


void mujoe_sm_initCtx( MUJOE_SM_CTX ctx, mujoe_sm_cfg_t *pCfg )
{
	MUJOE_SM_MEMSET(ctx,0,sizeof(mujoe_sm_t));
	ctx->cfg = *pCfg;

} // mujoe_sm_initCtx

mujoe_sm_err_t mujoe_sm_registerCallbacks( MUJOE_SM_CTX ctx, mujoe_sm_Cbs_t *pCBs )
{
	if( pCBs )
	{
		ctx->pCBs = pCBs;
		return MUJOE_SM_ERR_NONE;
	}
	else
		return MUJOE_SM_ERR_INV_PARAM;

} // mujoe_sm_registerCallbacks

mujoe_sm_err_t mujoe_sm_trigger( MUJOE_SM_CTX ctx, mujoe_sm_s_code_t code )
{
    // Check if state machine is awaiting external trigger
	if( ctx->rt.ret.code & MUJOE_SM_RET_CODE_AWAIT_EXT_TRIG )
	{
	    // Ensure that code is being awaited
	    if( (ctx->rt.ret.code & MUJOE_SM_CODE_MASK) == code )
	    {
	        // Clear awaiting flag
	        ctx->rt.ret.code &= ~MUJOE_SM_RET_CODE_AWAIT_EXT_TRIG;
	        // Set truggered flag
	        ctx->rt.ret.code |= MUJOE_SM_RET_CODE_EXT_TRIG_RXD;

	        // Cancel current transition timer
	        ctx->pCBs->trans( -1 );
	        // Induce transition immediately
	        ctx->pCBs->trans( 0 );

	        return MUJOE_SM_ERR_NONE;
	    }
	}

    return MUJOE_SM_ERR_FAILURE;

}// mujoe_sm_trigger

mujoe_sm_err_t mujoe_sm_start( MUJOE_SM_CTX ctx, mujoe_sm_s_time_ms_t delay_ms )
{
    mujoe_sm_err_t err = MUJOE_SM_ERR_NONE;
    err |= stim_trans( ctx, -1 );
    reset_var( ctx );
    err |= stim_trans( ctx, delay_ms );
    return err;

}// mujoe_sm_start


mujoe_sm_err_t mujoe_sm_run( MUJOE_SM_CTX ctx )
{
	mujoe_sm_err_t err = MUJOE_SM_ERR_NONE;

	if( (ctx->rt.ret.code & MUJOE_SM_CODE_MASK) != MUJOE_SM_RET_CODE_NONE )
	{
	    // If still awaiting trigger, timeout has occurred, set flag
	    if( ctx->rt.ret.code & MUJOE_SM_RET_CODE_AWAIT_EXT_TRIG )
	    {
	        ctx->rt.ret.code &= ~MUJOE_SM_RET_CODE_AWAIT_EXT_TRIG;
	        ctx->rt.ret.code |= MUJOE_SM_RET_CODE_TIMEOUT;
	    }


        mujoe_sm_trans_t trans;
        trans.code = MUJOE_SM_RET_CODE_NONE;

        int i;
        for( i = 0; i < ctx->cfg.numTrans; i++ )
        {
            // Find the transition rule from the transition table using the return code from
            // the state handler
            trans = ctx->cfg.pTransTbl[i];

            if( (trans.code == ctx->rt.ret.code ) &&
                (trans.curr == ctx->rt.currState) )
            {
                // Update internal state machine variables
                ctx->rt.lastState = ctx->rt.currState;
                ctx->rt.currState = trans.next;

                ctx->rt.lastCode = ctx->rt.currCode;
                ctx->rt.currCode = ctx->rt.ret.code;
                break;
            }
        }

        // Transition does not exist, throw error
        if( (trans.code & MUJOE_SM_CODE_MASK) == MUJOE_SM_RET_CODE_NONE )
        {
            if( ctx->pCBs && ctx->pCBs->error )
            {
                mujoe_sm_err_arg_t err_arg;
                err_arg.err = MUJOE_SM_ERR_INV_TRANS;
                MUJOE_SM_MEMCPY(&err_arg.params.invalidTrans, &trans, sizeof(trans) );

                ctx->pCBs->error( &err_arg );
                return MUJOE_SM_ERR_FAILURE;
            }
            else
                return MUJOE_SM_ERR_NOT_IMPLEMENTED;
        }
	}

    if( ctx->rt.currState < ctx->cfg.numStates )
    {
        // Invoke state handler
    	if( ctx->cfg.pStateTbl[ctx->rt.currState] )
    	    ctx->rt.ret = ctx->cfg.pStateTbl[ctx->rt.currState]( ctx->cfg.pHostVar );
    	else
    		return MUJOE_SM_ERR_NOT_IMPLEMENTED;

        // Invoke state transition
    	err = stim_trans(ctx, ctx->rt.ret.time_ms );
    }

	return err;

}// mujoe_sm_run

//////////////////////////////////////////////////////////////////////////////////////////////
//  STATIC FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////

static void reset_var( MUJOE_SM_CTX ctx )
{
    MUJOE_SM_MEMSET( &ctx->rt, 0, sizeof(mujoe_sm_rtVar_t) );

} // reset_var

static mujoe_sm_err_t stim_trans( MUJOE_SM_CTX ctx, mujoe_sm_s_time_ms_t ms )
{
    if( ctx->pCBs && ctx->pCBs->trans )
        return ctx->pCBs->trans( ms );
    else
        return MUJOE_SM_ERR_NOT_IMPLEMENTED;

} // stim_trans


