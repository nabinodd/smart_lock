//######################################### MPU #########################################
#include "SoftwareSerial.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include "stdlib.h"

#define OUTPUT_READABLE_YAWPITCHROLL
// #define MPU_DBG
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

Quaternion q;        // [w, x, y, z]         quaternion container
VectorInt16 aa;      // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;  // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld; // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity; // [x, y, z]            gravity vector
float euler[3];      // [psi, theta, phi]    Euler angle container
float ypr[3];        // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

MPU6050 mpu;
int yaw = 0, pitch = 0, roll = 0;

//######################################### MPU #########################################

int lock_timeout = 3000;

//######################################## MODEM ########################################

#define TINY_GSM_MODEM_SIM808
#include "TinyGsmClient.h"
#include "PubSubClient.h"

#define SerialMon Serial
#define TINY_GSM_DEBUG SerialMon

//Stepper motor pins
#define A 2
#define B 3
#define C 4
#define D 5

#define NUMBER_OF_STEPS_PER_REV 512

SoftwareSerial SerialAT(10, 11); // RX, TX

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient mqtt(client);

const char apn[] = "ntnet";
const char gprsUser[] = "";
const char gprsPass[] = "";

const char *broker = "broker.hivemq.com";

const char *pub_init = "gsmnabin/init";
const char *sub_passcode = "gsmnabin/passcode";
const char *pub_alarm = "gsmnabin/alarm";

const int mqtt_retry_limit = 5;
const int retry_interval = 1000;

//######################################## MODEM ########################################

void setup()
{
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);
    
    setupMpu();
    SerialMon.begin(115200);
    SerialAT.begin(9600);
    modem.init();
    while (!checkNetwork())
        ;
    SerialMon.println("Network OK!!");
    if (checkGprsConn())
        SerialMon.println("GPRS OK!!");

    mqtt.setServer(broker, 1883);
    mqtt.setCallback(mqttCallback);
    while (!mqtt.connected())
    {
        mqttConnect(true);
        delay(1000);
    }
}

void loop()
{
    pollMpu();
    if (roll < -5 || roll > 5)
    {
        delay(1000);
        SerialMon.println("Intrusion detected");

        if (!mqtt.connected())
        {
            SerialMon.println("MQTT not connected");
            if (!mqttConnect(true))
                SerialMon.println("Connection broken");
        }

        else if (mqtt.connected())
        {
            mqtt.publish(pub_alarm, "Roll");
            SerialMon.println("Alarm published");
            delay(5000);
        }
    }
    mqtt.loop();
}