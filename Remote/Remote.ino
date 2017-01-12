#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

MPU6050 mpu;

#define OUTPUT_READABLE_YAWPITCHROLL

#define LED_PIN 13

bool blinkState = false;
bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

Quaternion q;
VectorInt16 aa;
VectorInt16 aaReal;
VectorInt16 aaWorld;
VectorFloat gravity;
float ypr[3];

volatile bool mpuInterrupt = false;

void dmpDataReady()
{
  mpuInterrupt = true;
}

void setup()
{
  pinMode(8,OUTPUT);
  pinMode(12,OUTPUT);

  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  TWBR = 24;
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
  #endif

  Serial.begin(9600);

  while (!Serial);

  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();

  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788);

  if (devStatus == 0)
  {
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);
    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
  }

  else
  {
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  if (!dmpReady) return;
  while (!mpuInterrupt && fifoCount < packetSize)
  {

  }

  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
  fifoCount = mpu.getFIFOCount();
  if ((mpuIntStatus & 0x10) || fifoCount == 1024)
  {
    mpu.resetFIFO();
    Serial.println(F("FIFO overflow!"));
  }
  else if (mpuIntStatus & 0x02)
  {
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    fifoCount -= packetSize;

    #ifdef OUTPUT_READABLE_YAWPITCHROLL

    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    Serial.print("ypr\t");
    Serial.print(ypr[0] * 180/M_PI);
    Serial.print("\t");
    Serial.print(ypr[1] * 180/M_PI);
    Serial.print("\t");
    Serial.println(ypr[2] * 180/M_PI);

    if ( (ypr[2]*180/M_PI)<10.00 &&  (ypr[2]*180/M_PI)>0.00 && (ypr[1]*180/M_PI)>10.00)
    {
      drive_forward();
    }
    else if((ypr[2]*180/M_PI)<10.00 &&  (ypr[2]*180/M_PI)>0.00 && (ypr[1]*180/M_PI)<-10.00)
    {
      drive_backward();
    }
    else if((ypr[1]*180/M_PI)>-10.00 && (ypr[1]*180/M_PI)<10.00 && (ypr[2]*180/M_PI)>10.00)
    {
      turn_left();
    }
    else if((ypr[1]*180/M_PI)>-10.00 && (ypr[1]*180/M_PI)<10.00 && (ypr[2]*180/M_PI)<-10.00)
    {
      turn_right();
    }
    #endif

    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
  }
}

void drive_forward()
{
  digitalWrite(8,HIGH);
  digitalWrite(12,HIGH);
}

void drive_backward()
{
  digitalWrite(8,LOW);
  digitalWrite(12,LOW);
}

void turn_right()
{
  digitalWrite(12,LOW);
  digitalWrite(8,HIGH);
}

void turn_left()
{
  digitalWrite(12,HIGH);
  digitalWrite(8,LOW);
}
