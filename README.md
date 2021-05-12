## SoPIoT's Small Thing Library
<br/>

### What it does
- provides a library for implementing Big-Things (cloud, dev-board, etc..)
    - any devices which have a linux OS can execute a Thing
- provides a library for implementing Small-Things (arduino, etc..)
    - arduino requires MQTT-SN Gateway to communicate with Middleware

<br/><br/>

### How to Install

To make an Arduino Thing, copy all folders in things/small/arduino/libraries to Arduino library folder (C:\Users\<USERNAME>\Documents\Arduino\libraries)

Write a Small-Thing code following some examples in 'samples' directory

<br/>

### Configure Xbee

#### Xbee Coordinator (Gateway)
    
1. Update latest firmware  

    select latest firmware named  `Coordinator API` (S2) or `ZIGBEE TH Reg` (S2C) and press `update`.

2. Set the settings as below.

    ```
    // Identity and SC must be set equally between Coordinator and Routor

    ID : 0
    SC : FFFF
    JV : 0
    DH : 0
    DL : FFFF
    AP : 2
    IR : 1388

    // if Xbee module is `XB24C`(S2), you have to set this up, too.
    CE : 1
    ```
3. Set configure by press `write`.

#### Xbee Coordinator (Routor)

1. Update latest firmware  

    select latest firmware named `Routor API` (S2) or `ZIGBEE TH Reg` (S2C) and press `update`.

2. Set the settings as below.

    ```
    // Identity and SC must be set equally between Coordinator and Routor

    ID : 0
    SC : FFFF
    JV : 1
    DH : 0
    DL : 0
    AP : 2
    IR : 1388

    // if Xbee module is `XB24C`(S2), you have to set this up, too.
    CE : 0
    ```

3. Set configure by press `write`.
