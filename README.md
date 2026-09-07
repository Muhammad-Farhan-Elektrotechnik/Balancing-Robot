# Balancing-Robot

For my Balancing Robot Project im using PID (Propotional,Intergral,Derivative) with the help of Ziegler Nichols Method, more about it will later be specified.
Also the robot used all of the cores in the ESP32 (2 cores), where one of the core handle the calculation for PID and reading from the sensor, from there the results of the calculation delivered to the other task in another core, where it would execute the task to rotate the stepper motor. So it runs parallel and that means faster than just using one of the core for all of the tasks.

##**SCHEMATICS**
----------------
<img width="880" height="436" alt="Schematics" src="https://github.com/user-attachments/assets/a739171b-92dc-438b-a9f4-dd96e0af078c" />
----------------
**HARDWARE**
----------------
*ESP32 
*MPU6050 
*2 Stepper Motor
*2 Driver (DRV8825 in my case)
*2 electrolytic Capacitor (100 mikroFahrrad, 50v)
*Jumper Cable
*Power Supply (12v each)
-------------------------
**PINOUT** 
-----------------------
ESP32 | DRV8825 | MPU6050 | Power Supply
----- | ------- | ------- | ------------
3.3v  | RST,SLP |    -    |      -
GND   | GND     | GND     |      -

RST,SLP & VDD > 3.3v | GND > GND | STP & DIR > any GPIO Pin (still make sure its not i2c pinout) | SDA > 21 (i2c) | SCL > 22 (i2c) | Stepper Motor > A1,A2,B1,B2 (use Multimeter to find out which pair of cables are connected to the same coil) | Vmot & GND > Power Supply (connect the capacitor inbetween)
