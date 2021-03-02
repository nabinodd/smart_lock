void mqttCallback(char *topic, byte *payload, unsigned int len)
{
    SerialMon.print("Message arrived [");
    SerialMon.print(topic);
    SerialMon.print("]: ");
    SerialMon.write(payload, len);
    SerialMon.println();

    if (String(topic) == sub_passcode)
    {
        char data[len + 1];
        int end_val = 0;

        for (int i = 0; i < len; i++)
        {
            data[i] = payload[i];
            end_val = i;
        }
        data[end_val + 1] = '\0';
        String dataz = data;
        SerialMon.println(dataz);
        if (dataz == "1234")
        {
            stepR(100, 900);
            SerialMon.println("Unlocked....");
        }
        else if (dataz == "12345")
        {
            stepF(100, 900);
            Serial.println("Locked");
        }
        else if (dataz == "123456")
        {
            stepR(100, 900);
            SerialMon.println("Unlocked....");
            delay(lock_timeout);
            stepF(100, 900);
            Serial.println("Locked");
        }
    }
}

bool mqttConnect(bool blocking)
{
    int c = 0;
    SerialMon.print("Connecting to ");
    SerialMon.println(broker);
    if (blocking == true)
    {
        while (1)
        {
            SerialMon.print("Attempting connection : ");
            SerialMon.println(c);

            boolean status = mqtt.connect("gsmnabin");

            if (!status)
                continue;

            else if (status)
            {
                mqtt.subscribe(sub_passcode);
                SerialMon.println("Suscribed to Passcode");
                SerialMon.println("[OK] MQTT connection success");
                mqtt.publish(pub_init, "System connected");
                return mqtt.connected();
            }
        }
    }
    else
    {
        SerialMon.print("Attempting connection : ");
        SerialMon.println(c);

        boolean status = mqtt.connect("gsmnabin");

        if (!status)
            SerialMon.println("Unable to make MQTT conn..");

        else if (status)
        {
            mqtt.subscribe(sub_passcode);
            SerialMon.println("Suscribed to Passcode");
            SerialMon.println("[OK] MQTT connection success");
            mqtt.publish(pub_init, "System connected");
            return mqtt.connected();
        }
    }
    return false;
}