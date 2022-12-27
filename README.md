# mujoe_sm
State machine intended for use with embedded systems.

## Example
The *example* folder contains a Code Composer Studio (CCS) project targetting the Texas Instruments (TI) CC2640R2 microcontroller in
the TI LAUNCHXL-CC2640R2 hardware design. The application project uses the TI BLE-Stack to setup a Bluetooth Low Energy (BLE) simple peripheral. The BLE-Stack uses TI's TI-RTOS for its scheduler, which the *mujoe_sm* driver uses to schedule deterministic state transitions. 

The mujoe_sm driver is used to implement a simple UART state machine for receiving and transmitting ASCII characters via a USB COM port interface.

![image](https://user-images.githubusercontent.com/5027131/209600837-e3ec53a3-7631-4964-ba5a-27647b32dad9.png)

A terminal program (such as *PuTTY*) can be used for interfacing with the launchpad's on-board FTDI chip.

**Application project:** *example/simple_peripheral_cc2640r2lp_mujoe_sm.zip*

**BLE stack project:** *example/simple_peripheral_cc2640r2lp_stack_library.zip*
