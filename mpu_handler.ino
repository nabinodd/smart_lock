bool setupMpu()
{
    Wire.begin();
    Wire.setClock(400000);
    mpu.initialize();
    if (!mpu.testConnection())
    {
        SerialMon.println(F("MPU6050 connection failed"));
        return false;
    }

    devStatus = mpu.dmpInitialize();

    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788);

    if (devStatus == 0)
    {
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        mpu.PrintActiveOffsets();
        mpu.setDMPEnabled(true);

        mpuIntStatus = mpu.getIntStatus();
        packetSize = mpu.dmpGetFIFOPacketSize();
        return true;
    }
    else
    {
        SerialMon.print(F("DMP Initialization failed (code "));
        SerialMon.print(devStatus);
        SerialMon.println(F(")"));
        return false;
    }
}

void pollMpu()
{
    mpu.resetFIFO();
    fifoCount = mpu.getFIFOCount();
    while (fifoCount < packetSize)
        fifoCount = mpu.getFIFOCount();
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    yaw = ypr[0] * 180 / M_PI;
    pitch = ypr[1] * 180 / M_PI;
    roll = ypr[2] * 180 / M_PI;

#ifdef MPU_DBG
    SerialMon.print("ypr\t");
    SerialMon.print(ypr[0] * 180 / M_PI);
    SerialMon.print("\t");
    SerialMon.print(ypr[1] * 180 / M_PI);
    SerialMon.print("\t");
    SerialMon.println(ypr[2] * 180 / M_PI);
#endif
}