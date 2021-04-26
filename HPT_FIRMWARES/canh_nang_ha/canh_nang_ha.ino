#include "canh_nang_ha.h"


extern AsyncWebServer server;
extern struct configweb config_network;

void getStatus(){
    char* topicsendStatus = "CabinetAvy/HPT/deviceStatus";
    String datasend = "{\"deviceid\" : \"";
    datasend += String(device_id);
    datasend += "\", \"devicetype\" : \"motor\", \"typecontrol\" : \"getstatus\", \"status\" : \"";
    if(Status_Motor == MOTOR_DOWN){
        datasend += "close\"}";
        client.publish(topicsendStatus, datasend.c_str());
    }else{
        datasend += "open\"}";
        client.publish(topicsendStatus, datasend.c_str());
    }
    ECHOLN("-------getStatus-------");
}


void checkButtonConfigClick(){
    //hold to config mode
    if(digitalRead(PIN_CONFIG) == HIGH){
        time_click_button_config = millis();
    }
    if(digitalRead(PIN_CONFIG) == LOW && (time_click_button_config + CONFIG_HOLD_TIME) <= millis()){
        time_click_button_config = millis();
        tickerSetApMode.start();
        digitalWrite(PIN_LOA, HIGH);
        delay(100);
        digitalWrite(PIN_LOA, LOW);
        // control_motor.normal_mode = false;
        // EEPROM.write(EEPROM_WIFI_IS_REGISTER, 0);
        // EEPROM.commit();
        // ECHOLN("Done Reset!");
        // is_ap_mode = true;
        is_wifi_mode = false;
        SetupConfigMode();
        StartWebServer();
    }
}

String MacID(){
    uint8_t mac[WL_MAC_ADDR_LENGTH];
    String macID;
    WiFi.mode(WIFI_AP);
    WiFi.softAPmacAddress(mac);
    macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) + String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
    macID.toUpperCase();
    return macID;
}
void SetupConfigMode(){
    ECHOLN("[WifiService][setupAP] Open AP....");
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    // server.close();
    delay(500);
    WiFi.mode(WIFI_AP);
    String SSID_AP_MODE = SSID_PRE_AP_MODE + MacID();
    WiFi.softAP(SSID_AP_MODE.c_str(), PASSWORD_AP_MODE);
    IPAddress APIP(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(APIP, gateway, subnet);
    ECHOLN(SSID_AP_MODE);

    ECHOLN("[WifiService][setupAP] Softap is running!");
    IPAddress myIP = WiFi.softAPIP();
    ECHO("[WifiService][setupAP] IP address: ");
    ECHOLN(myIP);
}

void setLedApMode() {
    digitalWrite(ledTestWifi, !digitalRead(ledTestWifi));
}

void setPwmLedLighton(){
    countChangeLed++;
    float out_led_red, out_led_green, out_led_blue;
    out_led_red = (float)0 + (((float)countLedLightRed_After - (float)0)/255)*countChangeLed;
    out_led_red = abs(out_led_red);
    ledcWrite(LED_CHANNEL_R, uint8_t(out_led_red));

    out_led_green = (float)0 + (((float)countLedLightGreen_After - (float)0)/255)*countChangeLed;
    out_led_green = abs(out_led_green);
    ledcWrite(LED_CHANNEL_G, uint8_t(out_led_green));

    out_led_blue = (float)0 + (((float)countLedLightBlue_After - (float)0)/255)*countChangeLed;
    out_led_blue = abs(out_led_blue);
    ledcWrite(LED_CHANNEL_B, uint8_t(out_led_blue));

    if(countChangeLed == 255){
        ECHOLN("On Led");
		countChangeLed = 0;
	}

}

void setPwmLedLightoff(){
    countChangeLed++;
	float out_led_red, out_led_green, out_led_blue;
    out_led_red = (float)countLedLightRed_Before + (((float)0 - (float)countLedLightRed_Before)/255)*countChangeLed;
    out_led_red = abs(out_led_red);
	ledcWrite(LED_CHANNEL_R, uint8_t(out_led_red));
    
    out_led_green = (float)countLedLightGreen_Before + (((float)0 - (float)countLedLightGreen_Before)/255)*countChangeLed;
    out_led_green = abs(out_led_green);
	ledcWrite(LED_CHANNEL_G, uint8_t(out_led_green));
    
    out_led_blue = (float)countLedLightBlue_Before + (((float)0 - (float)countLedLightBlue_Before)/255)*countChangeLed;
    out_led_blue = abs(out_led_blue);
	ledcWrite(LED_CHANNEL_B, uint8_t(out_led_blue));
    
    if(countChangeLed == 255){
        ECHOLN("Off Led");
		countChangeLed = 0;
	}
}

void setPwmLedLightChange(){
    countChangeLed++;
    float out_led_red, out_led_green, out_led_blue;
    out_led_red = (float)countLedLightRed_Before + (((float)countLedLightRed_After - (float)countLedLightRed_Before)/255)*countChangeLed;
    out_led_red = abs(out_led_red);
    ledcWrite(LED_CHANNEL_R, uint8_t(out_led_red));

    out_led_green = (float)countLedLightGreen_Before + (((float)countLedLightGreen_After - (float)countLedLightGreen_Before)/255)*countChangeLed;
    out_led_green = abs(out_led_green);
    ledcWrite(LED_CHANNEL_G, uint8_t(out_led_green));

    out_led_blue = (float)countLedLightBlue_Before + (((float)countLedLightBlue_After - (float)countLedLightBlue_Before)/255)*countChangeLed;
    out_led_blue = abs(out_led_blue);
    ledcWrite(LED_CHANNEL_B, uint8_t(out_led_blue));

    // ECHO(uint8_t(out_led_red));
    // ECHO("-----");
    // ECHO(uint8_t(out_led_green));
    // ECHO("-----");
    // ECHOLN(uint8_t(out_led_blue));
    if(countChangeLed == 255){
        ECHOLN("Change Led");
		countChangeLed = 0;
        countLedLightRed_Before = countLedLightRed_After;
        countLedLightGreen_Before = countLedLightGreen_After;
        countLedLightBlue_Before = countLedLightBlue_After;
	}

}


void StartWebServer(){
	server.on("/", HTTP_GET, handleRoot);
	server.on("/motor_1_up", HTTP_GET, handleMotor1Up);
	server.on("/motor_2_up", HTTP_GET, handleMotor2Up);
	server.on("/motor_1_down", HTTP_GET, handleMotor1Down);
	server.on("/motor_2_down", HTTP_GET, handleMotor2Down);
	server.on("/config_wifi", HTTP_GET, handleConfigWifi);
	server.on("/reset_wifi", HTTP_GET, handleResetWifi);
	server.onNotFound(notFound);
	server.begin();
	Serial.println( "HTTP server started" );
}

void checkButtonUpDownClick(){
    if(!is_wifi_mode){
        if(digitalRead(PIN_MODE)){
            if(digitalRead(BUTTON_UP) == LOW && digitalRead(BUTTON_DOWN) == HIGH){
                ledcWrite(L1UpChannel, Max_L1_UP);
                ledcWrite(L1DownChannel, 0);
                ledcWrite(L2UpChannel, Max_L2_UP);
                ledcWrite(L2DownChannel, 0);
                delay(10);
            }
            else if(digitalRead(BUTTON_UP) == HIGH && digitalRead(BUTTON_DOWN) == LOW){
                ledcWrite(L1UpChannel, 0);
                ledcWrite(L1DownChannel, Max_L1_DOWN);
                ledcWrite(L2UpChannel, 0);
                ledcWrite(L2DownChannel, Max_L2_DOWN);  
                delay(10);
            }
            else{
                ledcWrite(L1UpChannel, 0);
                ledcWrite(L1DownChannel, 0);
                ledcWrite(L2UpChannel, 0);
                ledcWrite(L2DownChannel, 0);
                delay(10);
            }
        }
        else{
            if(digitalRead(BUTTON_UP) == LOW && digitalRead(BUTTON_DOWN) == HIGH){
                while (digitalRead(BUTTON_UP) == LOW && digitalRead(BUTTON_DOWN) == HIGH)
                {
                    delay(50);
                }
                if(Status_Motor == MOTOR_UP){
                    Status_Motor = MOTOR_STOP;
                    ledcWrite(L1UpChannel, 0);
                    ledcWrite(L1DownChannel, 0);
                    ledcWrite(L2UpChannel, 0);
                    ledcWrite(L2DownChannel, 0);
                }
                else{	//motor is current stop
                    Status_Motor = MOTOR_UP;
                    ledcWrite(L1UpChannel, Max_L1_UP);
                    ledcWrite(L1DownChannel, 0);
                    ledcWrite(L2UpChannel, Max_L2_UP);
                    ledcWrite(L2DownChannel, 0);
                }
                
            }
            else if(digitalRead(BUTTON_UP) == HIGH && digitalRead(BUTTON_DOWN) == LOW){
                while (digitalRead(BUTTON_UP) == HIGH && digitalRead(BUTTON_DOWN) == LOW)
                {
                    delay(50);
                }
                if(Status_Motor == MOTOR_DOWN){	//motor is current stop
                    Status_Motor = MOTOR_STOP;
                    ledcWrite(L1UpChannel, 0);
                    ledcWrite(L1DownChannel, 0);
                    ledcWrite(L2UpChannel, 0);
                    ledcWrite(L2DownChannel, 0);
                    
                }
                else{
                    Status_Motor = MOTOR_DOWN;
                    ledcWrite(L1UpChannel, 0);
                    ledcWrite(L1DownChannel, Max_L1_DOWN);
                    ledcWrite(L2UpChannel, 0);
                    ledcWrite(L2DownChannel, Max_L2_DOWN);  
                }
            }
            delay(10);
        }
    }
    else{
        if(digitalRead(BUTTON_UP) == LOW && digitalRead(BUTTON_DOWN) == HIGH){
                while (digitalRead(BUTTON_UP) == LOW && digitalRead(BUTTON_DOWN) == HIGH)
                {
                    delay(50);
                }
                if(Status_Motor == MOTOR_UP){
                    Status_Motor = MOTOR_STOP;
                    ledcWrite(L1UpChannel, 0);
                    ledcWrite(L1DownChannel, 0);
                    ledcWrite(L2UpChannel, 0);
                    ledcWrite(L2DownChannel, 0);
                }
                else{	//motor is current stop
                    Status_Motor = MOTOR_UP;
                    ledcWrite(L1UpChannel, Max_L1_UP);
                    ledcWrite(L1DownChannel, 0);
                    ledcWrite(L2UpChannel, Max_L2_UP);
                    ledcWrite(L2DownChannel, 0);
                }
                
            }
            else if(digitalRead(BUTTON_UP) == HIGH && digitalRead(BUTTON_DOWN) == LOW){
                while (digitalRead(BUTTON_UP) == HIGH && digitalRead(BUTTON_DOWN) == LOW)
                {
                    delay(50);
                }
                if(Status_Motor == MOTOR_DOWN){	//motor is current stop
                    Status_Motor = MOTOR_STOP;
                    ledcWrite(L1UpChannel, 0);
                    ledcWrite(L1DownChannel, 0);
                    ledcWrite(L2UpChannel, 0);
                    ledcWrite(L2DownChannel, 0);
                    
                }
                else{
                    Status_Motor = MOTOR_DOWN;
                    ledcWrite(L1UpChannel, 0);
                    ledcWrite(L1DownChannel, Max_L1_DOWN);
                    ledcWrite(L2UpChannel, 0);
                    ledcWrite(L2DownChannel, Max_L2_DOWN);  
                }
            }
            delay(10);
    }
}

void ReadEeprom(){
	Max_L1_UP = char(EEPROM.read(EEPROM_MOTOR_1_UP));
    if(Max_L1_UP == 0){
        Max_L1_UP = 255;
    }
	ECHO("MAX MOTOR 1 UP: ");
	ECHOLN(Max_L1_UP);

	Max_L2_UP = char(EEPROM.read(EEPROM_MOTOR_2_UP));
    if(Max_L2_UP == 0){
        Max_L2_UP = 255;
    }
	ECHO("MAX MOTOR 2 UP: ");
	ECHOLN(Max_L2_UP);

	Max_L1_DOWN = char(EEPROM.read(EEPROM_MOTOR_1_DOWN));
    if(Max_L1_DOWN == 0){
        Max_L1_DOWN = 255;
    }
	ECHO("MAX MOTOR 1 DOWN: ");
	ECHOLN(Max_L1_DOWN);
	
	Max_L2_DOWN = char(EEPROM.read(EEPROM_MOTOR_2_DOWN));
    if(Max_L2_DOWN == 0){
        Max_L2_DOWN = 255;
    }
	ECHO("MAX MOTOR 2 DOWN: ");
	ECHOLN(Max_L2_DOWN);
}


void ReadEepromWifi(){
	ECHOLN("Reading EEPROM ssid");
    ssid = "";
    for (int i = EEPROM_WIFI_SSID_START; i < EEPROM_WIFI_SSID_END; ++i){
        ssid += char(EEPROM.read(i));
    }
    ECHO("SSID: ");
    ECHOLN(ssid);
    ECHOLN("Reading EEPROM pass");
    pass = "";
    for (int i = EEPROM_WIFI_PASS_START; i < EEPROM_WIFI_PASS_END; ++i){
        pass += char(EEPROM.read(i));
    }
    ECHO("PASS: ");
    ECHOLN(pass);

    ECHOLN("Reading EEPROM Device ID");
    device_id = EEPROM.read(EEPROM_WIFI_DEVICE_ID);
    ECHO("ID: ");
    ECHOLN(device_id);

    ECHOLN("Reading EEPROM server");
    sever = "";
    for (int i = EEPROM_WIFI_SERVER_START; i < EEPROM_WIFI_SERVER_END; ++i){
        sever += char(EEPROM.read(i));
    }
    ECHO("SERVER: ");
    ECHOLN(sever);
}


bool testWifi(String esid, String epass) {
    ECHO("Connecting to: ");
    ECHOLN(esid);
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    server.end();
    WiFi.mode(WIFI_STA);        //bat che do station
    WiFi.begin(esid.c_str(), epass.c_str());
    int c = 0;
    ECHOLN("Waiting for Wifi to connect");
    is_wifi_mode = true;
    while (c < 40) {
        if (WiFi.status() == WL_CONNECTED) {
            ECHOLN("\rWifi connected!");
            ECHO("Local IP: ");
            ECHOLN(WiFi.localIP());
            // digitalWrite(ledTestWifi, HIGH);
            // ConnecttoMqttServer();
            return true;
        }
        if(digitalRead(PIN_CONFIG) == LOW || digitalRead(BUTTON_UP) == LOW || digitalRead(BUTTON_DOWN) == LOW){
            break;
        }
        delay(500);
        ECHO(".");
        c++;
    }
    ECHOLN("");
    ECHOLN("Connect timed out");
    return false;
}

bool reconnect() {
    // Loop until we're reconnected
    ECHO("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Avy-";
    // clientId += GetFullSSID;
    clientId += String(random(0xffffff), HEX);
    const char* willTopic = "CabinetAvy/HPT/LWT";
    uint8_t willQos = 0;
    boolean willRetain = false;
    String willMessage = "{\"devicetype\" : \"";
    willMessage += m_Typedevice;
    willMessage += "\", \"deviceid\" : \"";
    willMessage += String(device_id);
    willMessage += "\", \"status\" : \"error\"}";
    // Attempt to connect
    // if (client.connect(clientId.c_str(), user, pass)){
    // if (client.connect(clientId.c_str(), user, pass, willTopic, willQos, willRetain, willMessage.c_str())) {
    if (client.connect(clientId.c_str())) {
        ECHO("connected with id: ");
        ECHOLN(clientId);
        String topicControl = m_Pretopic + m_Control;
        String topicControlhand = m_Pretopic + m_Controlhand;

        
        // client.subscribe(topicGetstatusArray);
        client.subscribe(topicControl.c_str());
        client.subscribe(topicControlhand.c_str());
        ECHO("Done Subscribe Channel: ");
        // ECHO(topicGetstatus);
        // ECHO("  +  ");
        ECHO(topicControl);
        ECHO(", ");
        ECHOLN(topicControlhand);
        digitalWrite(ledTestWifi, HIGH);
        getStatus();
    } 
    if(!client.connected()){
        ECHO("failed, rc=");
        ECHO(client.state());
        ECHOLN(" try again in 2 seconds");
    }
    return client.connected();
}



void ConnecttoMqttServer(){
    // client.setServer(mqtt_server, MQTT_PORT);
    client.setServer(sever.c_str(), MQTT_PORT);
    client.setCallback(callbackMqttBroker);
    reconnect();
}

void callbackMqttBroker(char* topic, byte* payload, unsigned int length){
    
    String Topic = String(topic);
    ECHO("TOPIC: ");
    ECHOLN(Topic);

    String data;
    for (int i = 0; i < length; i++) {
        data += char(payload[i]);
    }
    ECHO("DATA: ");
    ECHOLN(data);
    StaticJsonBuffer<RESPONSE_LENGTH> jsonBuffer;
    JsonObject& rootData = jsonBuffer.parseObject(data);

    if(Topic.indexOf(m_Controlhand) > 0){
        if (rootData.success()){
            if(rootData["typedevice"] == m_Typedevice){
                int arraySize = rootData["deviceid"].size();   //get size of JSON Array
                int sensorValue[arraySize];
                bool isTrueControl = false;
                // ECHO("size: ");
                // ECHOLN(arraySize);
                for (int i = 0; i < arraySize; i++) { //Iterate through results
                    sensorValue[i] = rootData["deviceid"][i];  //Implicit cast
                    // ECHOLN(sensorValue[i]);
                    if(sensorValue[i] == device_id){
                        isTrueControl = true;
                        break;
                    }
                }
                if(isTrueControl){
                    String dataType = rootData["typecontrol"];
                    //---------control color------------------
                    if(dataType == "controlled"){
                        if(statusLed){
                            timeToSaveData = millis();
                            flagIsHaveChangeColorHand = true;
                            int controlled[3];
                            for (int i = 0; i < 3; i++) { //Iterate through results
                                controlled[i] = rootData["data"][i];  //Implicit cast
                            }
                            countLedLightRed_After = controlled[0];
                            countLedLightGreen_After = controlled[1];
                            countLedLightBlue_After = controlled[2];
                            ledcWrite(LED_CHANNEL_R, uint8_t(countLedLightRed_After));
                            ledcWrite(LED_CHANNEL_G, uint8_t(countLedLightGreen_After));
                            ledcWrite(LED_CHANNEL_B, uint8_t(countLedLightBlue_After));
                            countLedLightRed_Before = countLedLightRed_After;
                            countLedLightGreen_Before = countLedLightGreen_After;
                            countLedLightBlue_Before = countLedLightBlue_After;
                        }
                    }
                }
            }
        }
    }

    else if(Topic.indexOf(m_Control) > 0){
        if (rootData.success()){
            //--------------getstatus-------------
            if(rootData["typedevice"] == m_Typedevice){
                int arraySize = rootData["deviceid"].size();   //get size of JSON Array
                int sensorValue[arraySize];
                bool isTrueControl = false;
                // ECHO("size: ");
                // ECHOLN(arraySize);
                for (int i = 0; i < arraySize; i++) { //Iterate through results
                    sensorValue[i] = rootData["deviceid"][i];  //Implicit cast
                    // ECHOLN(sensorValue[i]);
                    if(sensorValue[i] == device_id){
                        isTrueControl = true;
                        break;
                    }
                }
                if(isTrueControl){
                    String dataType = rootData["typecontrol"];
                    //---------getstatus------------------
                    if(dataType == "getstatus"){
                        getStatus();
                    }
                    //---------control------------------
                    else if(dataType == "control"){
                        String dataControl = rootData["data"];
                        if(dataControl == "open"){
                            // Open();
                            digitalWrite(PIN_LOA, HIGH);
                            delay(100);
                            digitalWrite(PIN_LOA, LOW);
                            statusLed = true;
                            Status_Motor = MOTOR_UP;
                            ledcWrite(L1UpChannel, Max_L1_UP);
                            ledcWrite(L1DownChannel, 0);
                            ledcWrite(L2UpChannel, Max_L2_UP);
                            ledcWrite(L2DownChannel, 0);

                            tickerSetPwmLedLightOff.stop();
                            tickerSetPwmLedLightChange.stop();
                            tickerSetPwmLedLightOn.start();
                        }else if(dataControl == "close"){
                            // Close();
                            digitalWrite(PIN_LOA, HIGH);
                            delay(100);
                            digitalWrite(PIN_LOA, LOW);
                            statusLed = false;
                            Status_Motor = MOTOR_DOWN;
                            ledcWrite(L1UpChannel, 0);
                            ledcWrite(L1DownChannel, Max_L1_DOWN);
                            ledcWrite(L2UpChannel, 0);
                            ledcWrite(L2DownChannel, Max_L2_DOWN); 

                            tickerSetPwmLedLightOn.stop();
                            tickerSetPwmLedLightChange.stop(); 
                            tickerSetPwmLedLightOff.start();
                        }
                        else if(dataControl == "stop"){
                            // Stop();
                            Status_Motor = MOTOR_STOP;
                            ledcWrite(L1UpChannel, 0);
                            ledcWrite(L1DownChannel, 0);
                            ledcWrite(L2UpChannel, 0);
                            ledcWrite(L2DownChannel, 0);  
                        }
                    }
                    //---------controlled------------------
                    else if(dataType == "controlled"){
                        if(statusLed){
                            int controlled[3];
                            for (int i = 0; i < 3; i++) { //Iterate through results
                                controlled[i] = rootData["data"][i];  //Implicit cast
                            }
                            countLedLightRed_After = controlled[0];
                            countLedLightGreen_After = controlled[1];
                            countLedLightBlue_After = controlled[2];
                            ECHO("Writed: ");
                            ECHO(countLedLightRed_After);
                            ECHO(",");
                            ECHO(countLedLightGreen_After);
                            ECHO(",");
                            ECHOLN(countLedLightBlue_After);
                            tickerSetPwmLedLightOn.stop();
                            tickerSetPwmLedLightOff.stop();
                            tickerSetPwmLedLightChange.start();
                            EEPROM.write(EEPROM_WIFI_LED_RED, char(countLedLightRed_After));
                            EEPROM.write(EEPROM_WIFI_LED_GREEN, char(countLedLightGreen_After));
                            EEPROM.write(EEPROM_WIFI_LED_BLUE, char(countLedLightBlue_After));
                            EEPROM.commit();
                        }
                    }

                }
            }
        }
    }

}

void setup(void)
{
  	WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
    Serial.begin(115200);
    EEPROM.begin(EEPROM_WIFI_MAX_CLEAR);
	pinMode(PIN_LOA, OUTPUT);
	pinMode(ledTestWifi, OUTPUT);
	pinMode(PIN_MODE, INPUT);
	pinMode(PIN_CONFIG, INPUT);
	pinMode(BUTTON_UP, INPUT);
	pinMode(BUTTON_DOWN, INPUT);
	delay(10);
	digitalWrite(ledTestWifi, LOW);

	// configure LED PWM functionalitites
  	ledcSetup(L1UpChannel, freq, resolution);
	ledcSetup(L1DownChannel, freq, resolution);
	ledcSetup(L2UpChannel, freq, resolution);
	ledcSetup(L2DownChannel, freq, resolution);
	ledcSetup(LED_CHANNEL_R, freq, resolution);
	ledcSetup(LED_CHANNEL_G, freq, resolution);
	ledcSetup(LED_CHANNEL_B, freq, resolution);
  
	// attach the channel to the GPIO to be controlled
	ledcAttachPin(L1, L1UpChannel);
	ledcAttachPin(L2, L1DownChannel);
	ledcAttachPin(L3, L2UpChannel);
	ledcAttachPin(L4, L2DownChannel);
	ledcAttachPin(PIN_LED_LIGHT_R, LED_CHANNEL_R);
	ledcAttachPin(PIN_LED_LIGHT_G, LED_CHANNEL_G);
	ledcAttachPin(PIN_LED_LIGHT_B, LED_CHANNEL_B);

	ledcWrite(L1UpChannel, 0);
	ledcWrite(L1DownChannel, 0);
	ledcWrite(L2UpChannel, 0);
	ledcWrite(L2DownChannel, 0);

	Status_Motor = MOTOR_STOP;

	ReadEeprom();

    //led RGB
    countLedLightRed_After = char(EEPROM.read(EEPROM_WIFI_LED_RED));
	countLedLightGreen_After = char(EEPROM.read(EEPROM_WIFI_LED_GREEN));
    countLedLightBlue_After = char(EEPROM.read(EEPROM_WIFI_LED_BLUE));
    countLedLightRed_Before = countLedLightRed_After;
    countLedLightGreen_Before = countLedLightGreen_After;
    countLedLightBlue_Before = countLedLightBlue_After;
	ledcWrite(LED_CHANNEL_R, countLedLightRed_After);
    ledcWrite(LED_CHANNEL_G, countLedLightGreen_After);
    ledcWrite(LED_CHANNEL_B, countLedLightBlue_After);

	if(EEPROM.read(EEPROM_WIFI_IS_REGISTER) != 0){
		ECHOLN("Turn on Setup Wifi");
        is_wifi_mode = true;
		ReadEepromWifi();
		if(testWifi(ssid, pass)){
			ConnecttoMqttServer();
		}
	}
    else{
        ECHOLN("Run Without Wifi");
    }
	

}

void loop(void)
{
	checkButtonUpDownClick();
	checkButtonConfigClick();
    tickerSetApMode.update();
	tickerSetPwmLedLightOn.update();
	tickerSetPwmLedLightOff.update();
    tickerSetPwmLedLightChange.update();

	if(config_network.is_receive_data){
		config_network.is_receive_data = false;
		ReadEeprom();
	}

    if(config_network.is_receive_wifi){
        config_network.is_receive_wifi = false;
        tickerSetApMode.stop();
        digitalWrite(ledTestWifi, LOW);
        ECHOLN("Turn on Setup Wifi");
		ReadEepromWifi();
		if(testWifi(ssid, pass)){
			ConnecttoMqttServer();
		}
    }

    if (is_wifi_mode && WiFi.status() != WL_CONNECTED){
        digitalWrite(ledTestWifi, LOW);
        if(testWifi(ssid, pass)){
			ConnecttoMqttServer();
		}
    } 

    else if(is_wifi_mode && WiFi.status() == WL_CONNECTED){
        if (!client.connected()) {
            digitalWrite(ledTestWifi, LOW);
            unsigned long nowReconnectAttempt = millis();
            static unsigned long lastReconnectAttempt = 0;
            if (abs(nowReconnectAttempt - lastReconnectAttempt) > 5000) {
                lastReconnectAttempt = nowReconnectAttempt;
                reconnect();
            }
        }else{
            client.loop();
        }
        
    }


    //save data led color when cotrol by hand
    if(millis() == timeToSaveData + 5000 && flagIsHaveChangeColorHand){
        flagIsHaveChangeColorHand = false;
        ECHOLN("Save data");
        ECHO("RED: ");
        ECHOLN(countLedLightRed_After);
        ECHO("GREEN: ");
        ECHOLN(countLedLightGreen_After);
        ECHO("BLUE: ");
        ECHOLN(countLedLightBlue_After);

        EEPROM.write(EEPROM_WIFI_LED_RED, countLedLightRed_After);
        EEPROM.write(EEPROM_WIFI_LED_GREEN, countLedLightGreen_After);
        EEPROM.write(EEPROM_WIFI_LED_BLUE, countLedLightBlue_After);
        
        EEPROM.commit();
        ECHOLN("Done writing!");

    }
    

}
