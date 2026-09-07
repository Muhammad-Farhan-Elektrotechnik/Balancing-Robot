# Balancing-Robot

For my Balancing Robot Project im using PID (Propotional,Intergral,Derivative) with the help of Ziegler Nichols Method, more about it will later be specified.
Also the robot used all of the cores in the ESP32 (2 cores), where one of the core handle the calculation for PID and reading from the sensor, from there the results of the calculation delivered to the other task in another core, where it would execute the task to rotate the stepper motor. So it runs parallel and that means faster than just using one of the core for all of the tasks.
