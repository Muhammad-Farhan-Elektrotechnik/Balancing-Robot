# Balancing-Robot

For my Balancing Robot Project im using PID (Propotional,Intergral,Derivative) with the help of Ziegler Nichols Method, more about it will later be specified.
Also using the FreeRtos built in ESP32 so the robot can use the cores in the ESP32 (2 cores) and execute 2 task simultaneously, where one of the core handle the calculation for PID and reading from the sensor, from there the results of the calculation delivered to the other task in another core, where it would execute the task to rotate the stepper motor. So it runs parallel and that means faster than just using one of the core for all of the tasks.

SCHEMATICS
----------------
<img width="880" height="436" alt="Schematics" src="https://github.com/user-attachments/assets/a739171b-92dc-438b-a9f4-dd96e0af078c" />

HARDWARE
----------------
* ESP32 
* MPU6050 
* 2 Stepper Motor
* 2 Driver (DRV8825 in my case)
* 2 electrolytic Capacitor (100 mikroFahrrad, 50v)
* Jumper Cable
* Power Supply (12v each)
-------------------------
**PINOUT** 
-----------------------
>[!IMPORTANT]
>make sure to read the instruction with❗

ESP32 | DRV8825 | MPU6050 | Power Supply
----- | ------- | ------- | ------------
3.3v  | RST,SLP |    -    |      -
GND   | GND     | GND     |      -
any GPIO (still make sure its not i2c pinout)  | STP & DIR    |    -    |      - 
GPIO 21 (I2C) |     -   | SDA  |  -
GPIO 22 (I2C) |    -    | SCL  |   -
not connected  |  A1,A2,B1,B2 ❗(use Multimeter to find out which pair of cables are connected to the same coil) | -  | -
  not connected  | Vmot & GND | -  | + & - cables ❗(connect the capacitor inbetween)
  
  <img width="1600" height="736" alt="Selbsstabilisierender Robote" src="https://github.com/user-attachments/assets/2d85f41c-cf68-47b2-9bca-c5625e2d893f" />

> [!WARNING]
> In the photo the power supply is only 4 LiPO batteries which is 12v and run the 2 stepper motor parallel. It will be a problem when tuning the PID, since it required a few time to testing, because of the insufficient amount of current

> [!TIP]
> Using PCB would be reccomended.



MORE ABOUT PID
----------------------
There are 3 parameters that we can adjust, Kp,Ki, and Kd.
* Propotional mean responds to current error. Higher Kp = faster response but more overshoot and possible instability.
* Integral accumulates past errors. Eliminates steady-state error but can cause oscillation and windup.
* Derivative predicts future error. Dampens response and reduces overshoot but amplifies noise.
For further understanding a video about it from [Brian Douglass](https://youtu.be/wkfEZmsQqiA?si=tP_3sIJrEOYw4aCc) would be helpful.

HAND TUNING
------------------------------
Test out the Ziegler Nichols method [online](https://learnaboutpid.com/).

1. Make sure all of the parameter set to 0 (Kp,Ki, and Kd).
2. Then increase Kp until it reach steady oscillation.
<img width="697" height="259" alt="image" src="https://github.com/user-attachments/assets/c83633b0-5f1e-4796-8781-db50caf6b2c3" />

3. After that divide Kp by 2.
4. The next step is to increase Kd little by little until its not moving back and forth from the setpoint.
5. Increase Kp by a little.
6. By then the "robot" will move to one direction and here where Ki comes in. Increase and adjust Ki a little until it stabled.


