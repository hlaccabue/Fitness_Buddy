/**
* Bluetooth LE Serial Simple Example
* 
* Outputs the text "Hello!" to the Bluetooth LE Serial port every second.
*
* Avinab Malla
* 24 July 2022
**/

#include <BleSerial.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

BleSerial ble;
Adafruit_MPU6050 mpu;
float v_gyr_x = 0.0;
float v_gyr_y = 0.0;
float v_gyr_z = 0.0;
float p_gyr_x = 0.0;
float p_gyr_y = 0.0;
float p_gyr_z = 0.0;

void setup()
{
	//Start the BLE Serial
	//Enter the Bluetooth name here
	ble.begin("FitnessBuddy");
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
    
  }
  Serial.println("MPU6050 Found!");

  //setupt motion detection
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);  // Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);

  Serial.println("");
  delay(100);

  pinMode(0, OUTPUT); 
  pinMode(4, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(0, LOW);
  digitalWrite(4, LOW);
  digitalWrite(15, LOW);
  digitalWrite(2, LOW);
  
}

void loop()
{
	//The usage is similar to Serial
	 /* Get new sensor events with the readings */
	 if(ble.connected() == true){
    digitalWrite(2, HIGH);
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    float x_init = g.gyro.x;
    float y_init = g.gyro.y;
    float z_init = g.gyro.z; 
    char charVal[10];
    while(ble.connected() == true){
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);
      /* Print out the values */
      ble.print("AccelX:");         
      dtostrf(a.acceleration.x, 5, 2, charVal);
      ble.print(charVal);
      ble.print(",");
      ble.print("AccelY:");
      dtostrf(a.acceleration.y, 5, 2, charVal);
      ble.print(charVal);
      ble.print(",");
      ble.print("AccelZ:");
      dtostrf(a.acceleration.z, 5, 2, charVal);
      ble.print(charVal);
      ble.print(", ");
      ble.print("GyroX:");
      dtostrf(g.gyro.x, 5, 2, charVal);
      ble.print(charVal);
      ble.print(",");
      ble.print("GyroY:");
      dtostrf(g.gyro.y, 5, 2, charVal);
      ble.print(charVal);
      ble.print(",");
      ble.print("GyroZ:");
      dtostrf(g.gyro.z, 5, 2, charVal);
      ble.print(charVal);
      ble.println("");
      //v_gyr_x = v_gyr_x + (g.gyro.x-x_init)*.001;
      //v_gyr_y = v_gyr_y + (g.gyro.y-y_init)*.001;
      //v_gyr_y = v_gyr_z + (g.gyro.z-z_init)*.001;
      //p_gyr_x = p_gyr_x + (v_gyr_x)*.001;
      //p_gyr_y = p_gyr_y + (v_gyr_y)*.001;
      //p_gyr_y = p_gyr_z + (v_gyr_z)*.001;
      if(abs(g.gyro.x) > 2|| abs(g.gyro.y) > 2 || abs(g.gyro.z) > 2){
        digitalWrite(0, HIGH);
        digitalWrite(4, LOW);
        digitalWrite(15, LOW);
        
      }
      else if(abs(g.gyro.x) > 1|| abs(g.gyro.y) > 1 || abs(g.gyro.z) > 1){
        digitalWrite(0, LOW);
        digitalWrite(4, HIGH);
        digitalWrite(15, LOW);
      }
      else{
        digitalWrite(0, LOW);
        digitalWrite(4, LOW);
        digitalWrite(15, HIGH);
      }
      delay(250);
    }
    digitalWrite(2, LOW);
    digitalWrite(0, LOW);
    digitalWrite(4, LOW);
    digitalWrite(15, LOW);
    v_gyr_x = 0.0;
    v_gyr_y = 0.0;
    v_gyr_z = 0.0;
    p_gyr_x = 0.0;
    p_gyr_y = 0.0;
    p_gyr_z = 0.0;
  }
}
