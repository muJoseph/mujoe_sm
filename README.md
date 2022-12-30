# mujoe_sm
State machine intended for use with embedded systems.

**Features:**

- Scheduler agnostic
- Static state transition table for deterministic operation
- State transitions via external, asynchronous triggers (e.g. ISRs, etc.)
- External trigger timeout detection

## Quick start:

The following code snippets are taken from the included example project (see the subsequent section).

Create the state machine context, the state handler table, and the state transition table.

```c

// state machine context
static mujoe_sm_t uart_sm;

// state handler table
const mujoe_sm_state_Fp_t uart_sm_state_tbl[UART_SM_NUM_STATES] =
{
     uart_idle,                     // UART_SM_STATE_IDLE
     uart_awaitCmd,                 // UART_SM_STATE_AWT_CMD
     uart_cmdReceived,              // UART_SM_STATE_CMD_RXD
     uart_cmdTimeout,               // UART_SM_STATE_CMD_TIMEOUT
};

// state machine transition table
static const mujoe_sm_trans_t uart_sm_trans_tbl[] =
{
   // Current State ----------------------- Return Code --------------------------------------------------------- Next State

   // UART_SM_STATE_IDLE rules
   {  .curr = UART_SM_STATE_IDLE,          .code = UART_SM_RET_CODE_LISTEN_FOR_CMD,                              .next = UART_SM_STATE_AWT_CMD },

   // UART_SM_STATE_AWT_CMD rules
   {  .curr = UART_SM_STATE_AWT_CMD,       .code = MUJOE_SM_RET_CODE_EXT_TRIG_RXD | UART_SM_RET_CODE_CMD_RXD,    .next = UART_SM_STATE_CMD_RXD },
   {  .curr = UART_SM_STATE_AWT_CMD,       .code = MUJOE_SM_RET_CODE_TIMEOUT | UART_SM_RET_CODE_CMD_RXD,         .next = UART_SM_STATE_CMD_TIMEOUT },

   // UART_SM_STATE_CMD_RXD rules
   {  .curr = UART_SM_STATE_CMD_RXD,       .code = UART_SM_RET_CODE_SUCCESS,                                     .next = UART_SM_STATE_AWT_CMD },

   // UART_SM_STATE_CMD_TIMEOUT rules
   {  .curr = UART_SM_STATE_CMD_TIMEOUT,   .code = UART_SM_RET_CODE_SUCCESS,                                     .next = UART_SM_STATE_AWT_CMD },

};

```

The state handler table contains the callback functions for each state and the state transition table contains the transition rules for each state
based on the code returned by the current state's state handler callback function.

Initialize the state machine context structure.

```c

// state machine context configuration structure
mujoe_sm_cfg_t cfg =
{
    .pStateTbl     = (mujoe_sm_state_Fp_t *)uart_sm_state_tbl,
    .numStates     = UART_SM_NUM_STATES,
    .pTransTbl     = (mujoe_sm_trans_t *)uart_sm_trans_tbl,
    .numTrans      = sizeof(uart_sm_trans_tbl)/sizeof(mujoe_sm_trans_t),
    .pHostVar      = &hostArg,
};

// initialize state machine context with configuration
mujoe_sm_initCtx( &uart_sm, &cfg );

```

Register host callback functions with the state machine context.

```c

// host callbacks
static const mujoe_sm_Cbs_t mujoe_sm_Cbs =
{
     .error = mujoe_sm_error_CB,        // error handler
     .trans = mujoe_sm_trans_CB,        // state transition handler
}; 

// register callbacks with state machine context
mujoe_sm_registerCallbacks( &uart_sm, &mujoe_sm_Cbs );

```

Start the state machine.

```c
int delay_ms = 100;
mujoe_sm_start( &uart_sm, delay_ms );

```

Invoke *mujoe_sm_run* when the app-specific event/semaphore is raised by the *mujoe_sm_trans_Fp_t* callback within your scheduler.

```c

// uart state machine RTOS event
if (events & SBP_UART_SM_EVT)
{
    mujoe_sm_run( &uart_sm );
}

```

Call *mujoe_sm_trigger* to notify state machine of external, asynchronous events.

```c

void uart_driver_read_CB( UART_Handle handle, void *buf, size_t count)
{
   // Fire trigger to notify state machine that UART peripheral has received new bytes
   mujoe_sm_trigger( &uart_sm, UART_SM_RET_CODE_CMD_RXD );
}

```

## Example:

The *example* folder contains a Code Composer Studio (CCS) project targetting the Texas Instruments (TI) CC2640R2 microcontroller in
the TI LAUNCHXL-CC2640R2 hardware design. The application project uses the TI BLE-Stack to setup a Bluetooth Low Energy (BLE) simple peripheral. The BLE-Stack uses TI's TI-RTOS for its scheduler, which the *mujoe_sm* driver uses to schedule deterministic state transitions. 

The mujoe_sm driver is used to implement a simple UART state machine for receiving and transmitting ASCII characters via a USB COM port interface.

![image](https://user-images.githubusercontent.com/5027131/209600837-e3ec53a3-7631-4964-ba5a-27647b32dad9.png)

A terminal program (such as *PuTTY*) can be used for interfacing with the launchpad's on-board FTDI chip:

![image](https://user-images.githubusercontent.com/5027131/209758999-35cad50d-11f5-4548-b139-e55209849f50.png)

The UART state machine showcases the *mujoe_sm* API feature set, including: delayed state transitions, external trigger detection and external trigger timeout.

## Example files:

**Application project:** *example/simple_peripheral_cc2640r2lp_mujoe_sm.zip*

**BLE stack project:** *example/simple_peripheral_cc2640r2lp_stack_library.zip*

**NOTE:** Must have TI's *Code Composer Studio* (version 10.2 or later) and the *Simplelink CC2640R2 SDK v5.30.00.03* installed on your computer.
