#include <Arduino.h>

#include <Adafruit_LSM6DSOX.h>

#include "pindef.h"
#include "config.h"

#define CALIBRATION_SAMPLES 1000 // Number of samples for calibration
#define CALIBRATION_DELAY_MS 20

#define GRAVITY 1 // IMU Acceleration readings are relative to 9.81 m/s^2

#define GRAVITY_MS2 9.81

// // --- Gyroscope Bias Variables (radians/s) ---
// float gyroBiasX = 0.0;
// float gyroBiasY = 0.0;
// float gyroBiasZ = 0.0;

// // --- Accelerometer Bias Variables (m/s^2) ---
// float accelBiasX = 0.0;
// float accelBiasY = 0.0;
// float accelBiasZ = 0.0;

Adafruit_LSM6DSOX imu;

void getIMUReading(float &ax, float &ay, float &az, float &gx, float &gy, float &gz)
{
    float rawGyroX = 0.0;
    float rawGyroY = 0.0;
    float rawGyroZ = 0.0;
    float rawAccelX = 0.0;
    float rawAccelY = 0.0;
    float rawAccelZ = 0.0;

    imu.readGyroscope(rawGyroX, rawGyroY, rawGyroZ);
    imu.readAcceleration(rawAccelX, rawAccelY, rawAccelZ);

#ifdef BARO_ENABLE
// TODO: Add Baro Code
#endif

    gx = rawGyroX;
    gy = rawGyroY;
    gz = rawGyroZ;

    // ax = (rawAccelX - accelBiasX) * GRAVITY_MS2;
    // ay = (rawAccelY - accelBiasY) * GRAVITY_MS2;
    // az = (rawAccelZ - accelBiasZ) * GRAVITY_MS2;

    ax = (rawAccelX);
    ay = (rawAccelY);
    az = (rawAccelZ);
}

void sensorDataStream()
{
    float rawGyroX = 0.0;
    float rawGyroY = 0.0;
    float rawGyroZ = 0.0;
    float rawAccelX = 0.0;
    float rawAccelY = 0.0;
    float rawAccelZ = 0.0;

    // imu.readGyroscope(rawGyroX, rawGyroY, rawGyroZ);
    // imu.readAcceleration(rawAccelX, rawAccelY, rawAccelZ);

    getIMUReading(rawAccelX, rawAccelY, rawAccelZ, rawGyroX, rawGyroY, rawGyroZ);

    Serial.print("\t\tAccel X: ");
    Serial.print(rawAccelX);
    Serial.print(" \tY: ");
    Serial.print(rawAccelY);
    Serial.print(" \tZ: ");
    Serial.print(rawAccelZ);
    Serial.println(" m/s^2 ");

    /* Display the results (rotation is measured in rad/s) */
    Serial.print("\t\tGyro X: ");
    Serial.print(rawGyroX);
    Serial.print(" \tY: ");
    Serial.print(rawGyroY);
    Serial.print(" \tZ: ");
    Serial.print(rawGyroZ);
    Serial.println(" radians/s ");
    Serial.println();

    delay(10);
}

void setupIMU()
{
    Wire.setPins(IMU_SDA, IMU_SCL);
    // Wire.setClock(400000);

    if (!imu.begin_I2C())
    {
        Serial.println("IMU Setup Failed!");
        while (1)
        {
            delay(10);
        }
    }

    // imu.setAccelDataRate(LSM6DS_RATE_6_66K_HZ);
    // imu.setAccelRange(LSM6DS_ACCEL_RANGE_16_G);
    // imu.setGyroDataRate(LSM6DS_RATE_6_66K_HZ);
    // imu.setGyroRange(LSM6DS_GYRO_RANGE_2000_DPS);

    // getBias();

#ifdef DEBUG_INFO

    // imu.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
    Serial.print("Accelerometer range set to: ");
    switch (imu.getAccelRange())
    {
    case LSM6DS_ACCEL_RANGE_2_G:
        Serial.println("+-2G");
        break;
    case LSM6DS_ACCEL_RANGE_4_G:
        Serial.println("+-4G");
        break;
    case LSM6DS_ACCEL_RANGE_8_G:
        Serial.println("+-8G");
        break;
    case LSM6DS_ACCEL_RANGE_16_G:
        Serial.println("+-16G");
        break;
    }

    // imu.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS );
    Serial.print("Gyro range set to: ");
    switch (imu.getGyroRange())
    {
    case LSM6DS_GYRO_RANGE_125_DPS:
        Serial.println("125 degrees/s");
        break;
    case LSM6DS_GYRO_RANGE_250_DPS:
        Serial.println("250 degrees/s");
        break;
    case LSM6DS_GYRO_RANGE_500_DPS:
        Serial.println("500 degrees/s");
        break;
    case LSM6DS_GYRO_RANGE_1000_DPS:
        Serial.println("1000 degrees/s");
        break;
    case LSM6DS_GYRO_RANGE_2000_DPS:
        Serial.println("2000 degrees/s");
        break;
    case ISM330DHCX_GYRO_RANGE_4000_DPS:
        break; // unsupported range for the DSOX
    }

    // imu.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
    Serial.print("Accelerometer data rate set to: ");
    switch (imu.getAccelDataRate())
    {
    case LSM6DS_RATE_SHUTDOWN:
        Serial.println("0 Hz");
        break;
    case LSM6DS_RATE_12_5_HZ:
        Serial.println("12.5 Hz");
        break;
    case LSM6DS_RATE_26_HZ:
        Serial.println("26 Hz");
        break;
    case LSM6DS_RATE_52_HZ:
        Serial.println("52 Hz");
        break;
    case LSM6DS_RATE_104_HZ:
        Serial.println("104 Hz");
        break;
    case LSM6DS_RATE_208_HZ:
        Serial.println("208 Hz");
        break;
    case LSM6DS_RATE_416_HZ:
        Serial.println("416 Hz");
        break;
    case LSM6DS_RATE_833_HZ:
        Serial.println("833 Hz");
        break;
    case LSM6DS_RATE_1_66K_HZ:
        Serial.println("1.66 KHz");
        break;
    case LSM6DS_RATE_3_33K_HZ:
        Serial.println("3.33 KHz");
        break;
    case LSM6DS_RATE_6_66K_HZ:
        Serial.println("6.66 KHz");
        break;
    }

    // imu.setGyroDataRate(LSM6DS_RATE_12_5_HZ);
    Serial.print("Gyro data rate set to: ");
    switch (imu.getGyroDataRate())
    {
    case LSM6DS_RATE_SHUTDOWN:
        Serial.println("0 Hz");
        break;
    case LSM6DS_RATE_12_5_HZ:
        Serial.println("12.5 Hz");
        break;
    case LSM6DS_RATE_26_HZ:
        Serial.println("26 Hz");
        break;
    case LSM6DS_RATE_52_HZ:
        Serial.println("52 Hz");
        break;
    case LSM6DS_RATE_104_HZ:
        Serial.println("104 Hz");
        break;
    case LSM6DS_RATE_208_HZ:
        Serial.println("208 Hz");
        break;
    case LSM6DS_RATE_416_HZ:
        Serial.println("416 Hz");
        break;
    case LSM6DS_RATE_833_HZ:
        Serial.println("833 Hz");
        break;
    case LSM6DS_RATE_1_66K_HZ:
        Serial.println("1.66 KHz");
        break;
    case LSM6DS_RATE_3_33K_HZ:
        Serial.println("3.33 KHz");
        break;
    case LSM6DS_RATE_6_66K_HZ:
        Serial.println("6.66 KHz");
        break;
    }
#endif
}