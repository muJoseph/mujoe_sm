# mujoe_sm
State machine intended for use with embedded systems.

**Features:**

- Scheduler agnostic
- Static state transition table for deterministic operation
- State transitions via external, asynchronous triggers (e.g. ISRs, etc.)
- External trigger timeout detection

## Example

The *example* folder contains a Code Composer Studio (CCS) project targetting the Texas Instruments (TI) CC2640R2 microcontroller in
the TI LAUNCHXL-CC2640R2 hardware design. The application project uses the TI BLE-Stack to setup a Bluetooth Low Energy (BLE) simple peripheral. The BLE-Stack uses TI's TI-RTOS for its scheduler, which the *mujoe_sm* driver uses to schedule deterministic state transitions. 

The mujoe_sm driver is used to implement a simple UART state machine for receiving and transmitting ASCII characters via a USB COM port interface.

![image](https://user-images.githubusercontent.com/5027131/209600837-e3ec53a3-7631-4964-ba5a-27647b32dad9.png)

A terminal program (such as *PuTTY*) can be used for interfacing with the launchpad's on-board FTDI chip:

![image](https://user-images.githubusercontent.com/5027131/209758999-35cad50d-11f5-4548-b139-e55209849f50.png)

The UART state machine showcases the *mujoe_sm* API feature set, including: delayed state transitions, external trigger detection and external trigger timeout.

## Example files

**Application project:** *example/simple_peripheral_cc2640r2lp_mujoe_sm.zip*

**BLE stack project:** *example/simple_peripheral_cc2640r2lp_stack_library.zip*

**NOTE:** Must have TI's *Code Composer Studio* (version 10.2 or later) and the *Simplelink CC2640R2 SDK v5.30.00.03* installed on your computer.
