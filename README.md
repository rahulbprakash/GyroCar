#GyroCar
##Overview
A GyroCar was designed using a Gyroscope and RF Kit. The remote contains the gyroscope with an arduino microcontroller and a transmitter ,the car had the reciver, motor driver and arduino microcontroller. Using I2C protocol the gyroscope data was processed and used.

##Components Used
1. Arduino Uno
2. DC Motors
3. Gyroscope (MPU6050)
4. RF Kit (with Encoder Decoder module HT12E HT12D)
5. 9V Batteries
6. Chassis and wheels

##Usage
Copy the files in library folder to arduino's library folder and then compile the code. The code is straight forward, use Remote.ino on the remote arduino and Car.ino on car arduino. 

##Demo
![Model](/Images/GyroCar_Demo.gif?raw=true "Optional Title")

##References
1. [MPU6050 Info](https://www.i2cdevlib.com/devices/mpu6050#source)

