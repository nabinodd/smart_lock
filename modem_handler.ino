void printGsmInfo()
{
    String modemInfo = modem.getModemInfo();
    SerialMon.print("Modem Info: ");
    SerialMon.println(modemInfo);
}

bool checkNetwork()
{
    SerialMon.print("Waiting for network...");
    if (!modem.waitForNetwork())
    {
        SerialMon.println(" fail");
        delay(10000);
        return false;
    }

    if (modem.isNetworkConnected())
    {
        SerialMon.println(" success");
        return true;
    }
    return false;
}

bool checkGprsConn()
{
    SerialMon.print(F("Connecting to "));
    SerialMon.print(apn);
    bool gprs_status = modem.gprsConnect(apn, gprsUser, gprsPass);
    if (!gprs_status)
    {
        SerialMon.println(" fail");
        delay(2000);
        return false;
    }

    if (modem.isGprsConnected())
    {
        SerialMon.println(" success");
        return true;
    }
    return false;
}
