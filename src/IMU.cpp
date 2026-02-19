#include <Arduino.h>

#include <Adafruit_LSM6DSOX.h>

#include "pindef.h"
#include "config.h"

#define CALIBRATION_SAMPLES 1000 // Number of samples for calibration
#define CALIBRATION_DELAY_MS 20

#define GRAVITY 1 // IMU Acceleration readings are relative to 9.81 m/s^2

#define GRAVITY_MS2 9.81

// --- Gyroscope Bias Variables (radians/s) ---
float gyroBiasX = 0.0;
float gyroBiasY = 0.0;
float gyroBiasZ = 0.0;

// --- Accelerometer Bias Variables (m/s^2) ---
float accelBiasX = 0.0;
float accelBiasY = 0.0;
float accelBiasZ = 0.0;

Adafruit_LSM6DSOX imu;

void getBias()
{
    float sumGyroX = 0.0;
    float sumGyroY = 0.0;
    float sumGyroZ = 0.0;
    float sumAccelX = 0.0;
    float sumAccelY = 0.0;
    float sumAccelZ = 0.0;

    float rawGyroX = 0.0;
    float rawGyroY = 0.0;
    float rawGyroZ = 0.0;
    float rawAccelX = 0.0;
    float rawAccelY = 0.0;
    float rawAccelZ = 0.0;

    for (int i = 0; i < CALIBRATION_SAMPLES; i++)
    {
        // imu.getEvent(&accel, &gyro, &temp); // Get raw readings
        imu.readGyroscope(rawGyroX, rawGyroY, rawGyroZ);
        imu.readAcceleration(rawAccelX, rawAccelY, rawAccelZ);

        sumGyroX += rawGyroX;
        sumGyroY += rawGyroY;
        sumGyroZ += rawGyroZ;

        sumAccelX += rawAccelX;
        sumAccelY += rawAccelY;
        sumAccelZ += rawAccelZ;

        delay(CALIBRATION_DELAY_MS);
        if (i % (CALIBRATION_SAMPLES / 10) == 0)
        { // Print progress 10 times
            Serial.print(".");
        }
    }

    // --- Calculate Gyro Bias ---
    // Gyroscope bias is simply the average reading when stationary.
    gyroBiasX = sumGyroX / CALIBRATION_SAMPLES;
    gyroBiasY = sumGyroY / CALIBRATION_SAMPLES;
    gyroBiasZ = sumGyroZ / CALIBRATION_SAMPLES;

    // --- Calculate Accelerometer Bias ---
    // The accelerometer should ideally read 0 m/s^2 on X and Y axes when level.
    // The Z-axis should read +GRAVITY or -GRAVITY m/s^2.
    // So, bias is the average reading minus the expected value.

    float avgAccelX = sumAccelX / CALIBRATION_SAMPLES;
    float avgAccelY = sumAccelY / CALIBRATION_SAMPLES;
    float avgAccelZ = sumAccelZ / CALIBRATION_SAMPLES;

    accelBiasX = avgAccelX - 0.0; // Expected X acceleration is 0 m/s^2
    accelBiasY = avgAccelY - 0.0; // Expected Y acceleration is 0 m/s^2

    // Expected Z acceleration is +GRAVITY m/s^2 if sensor Z-axis is pointing UP.
    // If your sensor Z-axis points DOWN during calibration, use -GRAVITY.
    accelBiasZ = avgAccelZ - GRAVITY; // Adjust based on your IMU's default orientation relative to gravity

    // Example for if Z-axis is pointing down during calibration:
    // accelBiasZ = avgAccelZ - (-GRAVITY);

#ifdef DEBUG_INFO
    Serial.println("\n\nBias Values:");
    Serial.printf("Avg AX: %f , Avg AY: %f , Avg AZ: %f\n", avgAccelX, avgAccelY, avgAccelZ);
    Serial.printf("GX: %f , GY: %f , GZ: %f\n", gyroBiasX, gyroBiasY, gyroBiasZ);
    Serial.printf("AX: %f , AY: %f , AZ: %f\n", accelBiasX, accelBiasY, accelBiasZ);
#endif
}

void getSensorReading(float &ax, float &ay, float &az, float &gx, float &gy, float &gz)
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

    gx = rawGyroX - gyroBiasX;
    gy = rawGyroY - gyroBiasY;
    gz = rawGyroZ - gyroBiasZ;

    // ax = (rawAccelX - accelBiasX) * GRAVITY_MS2;
    // ay = (rawAccelY - accelBiasY) * GRAVITY_MS2;
    // az = (rawAccelZ - accelBiasZ) * GRAVITY_MS2;

    ax = (rawAccelX - accelBiasX);
    ay = (rawAccelY - accelBiasY);
    az = (rawAccelZ - accelBiasZ);

#ifdef BARO_ENABLE
// TODO: Add Baro Code
#endif
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

    getSensorReading(rawAccelX, rawAccelY, rawAccelZ, rawGyroX, rawGyroY, rawGyroZ);

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

void setupSensors()
{
    Wire.setPins(IMU_SDA, IMU_SCL);
    Wire.setClock(400000);

    if (!imu.begin_I2C())
    {
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