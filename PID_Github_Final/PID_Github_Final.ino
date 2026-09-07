//Libary
#include <Wire.h>
#include <AccelStepper.h>

//MPU Address
const int MPU_ADDR = 0x68;

//offset variable
float gyroXoffset = 0, gyroYoffset = 0, gyroZoffset = 0;

//Stepper 1 & 2
//set it according to the GPIO
AccelStepper Stepper1(AccelStepper::DRIVER,"STP GPIO","DIR GPIO");
AccelStepper Stepper2(AccelStepper::DRIVER,"STP GPIO","DIR GPIO");

volatile float Steps = 0.0;
float Input = 0.0;
double LastInput = 0.0;

//target pitch/roll
float Target = 0.0;

//PID parameter, tune it here
double Kp = 0.0;
double Ki = 0.0;
double Kd = 0.0;

double Output = 0.0;
double Proportional = 0.0;
double Integral = 0.0;
double Derivative = 0.0;

TaskHandle_t movTaskHandle;
TaskHandle_t pidTaskHandle;

void MOV_Task(void *parameter){
      
      Stepper1.setMaxSpeed(4000);
      Stepper2.setMaxSpeed(4000);

      Stepper1.setAcceleration(12000);
      Stepper2.setAcceleration(12000);

    while(1){
      // + or - set it accoriding to the stepper motor rotation.
      Stepper1.setSpeed(Steps);
      Stepper2.setSpeed(-Steps);

      Stepper1.runSpeed();
      Stepper2.runSpeed();
    }
    
}

void PID_Task(void *parameter){

//Gyro and Accelerometer Variable
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
float pitch = 0.0, roll = 0.0;

unsigned long time = millis();

     
      while(1){

      unsigned long curtime = millis();
      double dt = (curtime-time)/1000.0;

       Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  
  //reading raw data from IMU
  if(Wire.requestFrom(MPU_ADDR, 14, true)==14){

  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  Wire.read(); Wire.read(); 
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
  } else {
      Serial.println("Error! Can't get the raw data!");
  }
 
 //processing raw data
  float gyroXrate = (GyX - gyroXoffset) / 131.0;
  float gyroYrate = (GyY - gyroYoffset) / 131.0;
  float accelRoll = atan2(AcY, AcZ) * 180 / PI;
  float accelPitch  = atan2(-AcX, AcZ) * 180 / PI;

      //complementary filter (sensor fusion)
  pitch = 0.98 * (pitch + gyroYrate * dt) + 0.02 * accelPitch;
  roll  = 0.98 * (roll + gyroXrate * dt) + 0.02 * accelRoll;
  Serial.println(roll); //pitch or roll set it according to the mounting
      
            Input = roll;//pitch or roll set it according to the mounting
      double Error = Target - Input;
  
      Proportional = Kp*Error;
      if(Ki!=0){
      Integral += Error*dt;
      
      //avoid windup
      Integral = constrain(Integral, -4000/Ki, 4000/ Ki);
      } else {
        Integral= 0;
      }
      Derivative = (Input-LastInput)/dt;

      Output = Proportional + Integral*Ki - Derivative*Kd;
      
      Output = constrain(Output, -4000,4000);
      Serial.println(Output);
      //global variable to send to MOVE Task
      Steps = Output;
      
time = curtime;
LastInput = Input;

//yield
vTaskDelay(1/portTICK_PERIOD_MS);
      }
}

void setup() {
Serial.begin(9600);
//"waking up" MPU6050
Wire.begin();
Wire.beginTransmission(MPU_ADDR);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);

  //calibrating IMU
   const float samples = 2000.0;
  long gx = 0, gy = 0, gz = 0;
  for (int i = 0; i < samples; i++) {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 6, true);

    int16_t GyX = Wire.read() << 8 | Wire.read();
    int16_t GyY = Wire.read() << 8 | Wire.read();
    int16_t GyZ = Wire.read() << 8 | Wire.read();

    gx += GyX;
    gy += GyY;
    gz += GyZ;
  }
//average offset in each axis
  gyroXoffset = gx / samples;
  gyroYoffset = gy / samples;
  gyroZoffset = gz / samples;

  //RTOS Dual Core

  //first core rotating the stepper motor
  xTaskCreatePinnedToCore(MOV_Task, //name of the task
                    "MOVE Task",
                    4096, // number of bytes provided
                    NULL,
                    1, //priority
                    &movTaskHandle,
                    1); //core
//second core calclating PID  
xTaskCreatePinnedToCore (PID_Task,//name of the task
                  "PID Task",
                  4096, // number of bytes provided
                  NULL,
                  0, //priority
                  &pidTaskHandle,
                  0);//core
}

void loop() {
  
  //does nothing
}
