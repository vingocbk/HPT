#include "cua_tu_esp_6_sensor_mqtt.h"


void getStatus(){
    String datasend = "{\"deviceid\" : \"";
    datasend += String(deviceId);
    datasend += "\", \"devicetype\" : \"motor\", \"typecontrol\" : \"getstatus\",  \"number_disconnect\" : \"";
    datasend += String(countDisconnectToServer - 1);
    datasend += "\", \"all_time\" : \"";
    datasend += String(sum_time_disconnect_to_sever);
    datasend += "\", \"red\" : \"";
    datasend += String(countLedLightRed_After);
    datasend += "\", \"green\" : \"";
    datasend += String(countLedLightGreen_After);
    datasend += "\", \"blue\" : \"";
    datasend += String(countLedLightBlue_After);
    datasend +=  "\", \"status\" : \"";
    if(Forward == true){
        datasend += "close\"}";
        client.publish(topicsendStatus, datasend.c_str());
    }else{
        datasend += "open\"}";
        client.publish(topicsendStatus, datasend.c_str());
    }
    ECHOLN("-------getStatus-------");
}

void SendStatusReconnect(){
    const char* willTopic = "CabinetAvy/HPT/LWT";
    String ReconnectMessage = "{\"devicetype\" : \"";
    ReconnectMessage += m_Typedevice;
    ReconnectMessage += "\", \"deviceid\" : \"";
    ReconnectMessage += String(deviceId);
    ReconnectMessage += "\", \"status\" : \"ok\"}";
    client.publish(willTopic, ReconnectMessage.c_str());
    ECHOLN("-------Reconnect-------");
}




void loadDataBegin(){
    setmoderunbegin = char(EEPROM.read(EEPROM_SET_MODE_RUN_BEGIN));
    setmoderunbeginchange = setmoderunbegin;
    ECHOLN(setmoderunbegin);
    if(setmoderunbegin != 1 && setmoderunbegin != 2 && setmoderunbegin != 3 && setmoderunbegin != 4 && setmoderunbegin != 5){
        setmoderunbegin = 3;
        setmoderunbeginchange = setmoderunbegin;
        ECHOLN("read setmoderunbegin EEPROM fail, auto set 3!");
    }else{
        ECHO("Mode Run =  ");
        ECHOLN(setmoderunbeginchange);
    }


    if(EEPROM.read(EEPROM_DISTANT) != 255 && EEPROM.read(EEPROM_DISTANT) != 0){
        isSaveDistant = true;
        countPulDistant = EEPROM.read(EEPROM_DISTANT);
        ECHO("Distant = ");
        ECHOLN(countPulDistant);
        Close();
    }else{
        isSaveDistant = false;
        ECHOLN("isSaveDistant fasle!");
    }

    if(EEPROM.read(EEPROM_SET_TIME_RETURN) != 255 && EEPROM.read(EEPROM_SET_TIME_RETURN) != 0){
        time_return = EEPROM.read(EEPROM_SET_TIME_RETURN);
        ECHO("time_return = ");
        ECHO(time_return);
        ECHOLN("0 (ms)");
    }else{
        time_return = 45;
        ECHOLN("isSetTimeReurn fasle, auto set 450(ms)");
    }

    if(EEPROM.read(EEPROM_SET_PERCENT_OUT_LOW_SPEED) != 255 && EEPROM.read(EEPROM_SET_PERCENT_OUT_LOW_SPEED) != 0
        && EEPROM.read(EEPROM_SET_PERCENT_IN_LOW_SPEED) != 255 && EEPROM.read(EEPROM_SET_PERCENT_IN_LOW_SPEED) != 0){
        isSavePercentLowSpeed = true;
        percentLowSpeedIn = EEPROM.read(EEPROM_SET_PERCENT_IN_LOW_SPEED);
        ECHO("percentLowSpeedIn = ");
        ECHOLN(percentLowSpeedIn);

        percentLowSpeedOut = EEPROM.read(EEPROM_SET_PERCENT_OUT_LOW_SPEED);
        ECHO("percentLowSpeedOut = ");
        ECHOLN(percentLowSpeedOut);
    }else{
        isSavePercentLowSpeed = false;
        ECHOLN("isSavePercentLowSpeed fasle!");
    }

    delay_mqtt_loop = EEPROM.read(EEPROM_DELAY_MQTT_TO_LOW_SPEAKER);
    if(delay_mqtt_loop == 0 || delay_mqtt_loop == 255){
        delay_mqtt_loop = 10;
    }
    ECHO("delay_mqtt_loop = ");
    ECHO(delay_mqtt_loop);
    ECHOLN("(ms)");

    //touch sensor
    if(EEPROM.read(EEPROM_WIFI_IS_TOUCH_SENSOR) == 1){
        flag_is_touch_sensor = true;
        ECHOLN("flag_is_touch_sensor = true");
    }else{
        ECHOLN("flag_is_touch_sensor = false");
    }

    //count time check analog sensor
    count_check_analog_pin = EEPROM.read(EEPROM_COUNT_TIME_ANALOG);
    if(count_check_analog_pin == 0 || count_check_analog_pin == 255){
        count_check_analog_pin = 2;
    }
    ECHO("count_check_analog_pin = ");
    ECHO(count_check_analog_pin*100);
    ECHOLN("(ms)");
    
    //value error analog 
    valueErrorAnalogRead = EEPROM.read(EEPROM_VALUE_ERROR_ANALOG);
    if(valueErrorAnalogRead == 0){
        valueErrorAnalogRead = 100;
    }
    ECHO("value Error Analog Read: ");
    ECHOLN(valueErrorAnalogRead);


    //time auto close
    defineTimeAutoClose = EEPROM.read(EEPROM_TIME_AUTO_CLOSE);
    // if(defineTimeAutoClose == 0){
    //     defineTimeAutoClose = 10;       //10 min
    // }
    ECHO("Time Auto Close: ");
    ECHO(defineTimeAutoClose);
    ECHOLN("(min)");


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


    valueAnalogRead = analogRead(ANALOG_READ_BUTTON);
    prevalueAnalogRead = valueAnalogRead;


    for(int i = 1; i <= 5; i++){
        valueTouchSensorFix += touchRead(PIN_TOUCH_SENSOR);
        if(i == 5){
            valueTouchSensorFix = valueTouchSensorFix/5;
            ECHO("valueTouchSensorFix = ");
            ECHOLN(valueTouchSensorFix);
            break;
        }
    }

    
}


void resetDistant(){
    EEPROM.write(EEPROM_DISTANT, 0);
    EEPROM.commit();
    countPulFGDistant = 0;
    isSaveDistant = false;
    fristRun = true;
    countFrirstRun = 0;
    ECHOLN("resetDistant");
}

void Open(){
    ECHOLN("open");
    /////////////////////
    printlnmicoros = true;
    co = 0;


    statusStop = false;
    Forward = true;
    daytay = false;
    statusLed = true;
    timecaculateSpeed = 0;
    digitalWrite(DIR, QUAY_THUAN);
    SetPWMspeed.start();
    tickerCaculateSpeed.start();
	//bat den led
	if(countChangeLed == 0){
		tickerSetPwmLedLightOff.stop();
		tickerSetPwmLedLightOn.start();
	}
}

void Close(){
    ECHOLN("close");
    /////////////////////
    printlnmicoros = true;
    co = 0;


    statusStop = false;
    Forward = false;
    daytay = false;
    timecaculateSpeed = 0;
    digitalWrite(DIR, QUAY_NGHICH);
    SetPWMspeed.start();
    tickerCaculateSpeed.start();
    tickerSetPwmLedLightOn.stop();
}


void Stop(){
    tickerCaculateSpeed.stop();
    SetPWMspeed.stop();
    digitalWrite(PWM, LOW);
    delay(200);
    statusStop = false;
    daytay = false;
    timecaculateSpeed = 0;
    if(Forward == true){
        digitalWrite(DIR, QUAY_NGHICH);     //cho dong co quay nghich
        Forward = false;
    }else{
        digitalWrite(DIR, QUAY_THUAN);
        Forward = true;
    }
    SetPWMStopSpeed.start();
}


void SetupNetwork() {
    ECHOLN("Reading EEPROM ssid");
    esid = "";
    for (int i = EEPROM_WIFI_SSID_START; i < EEPROM_WIFI_SSID_END; ++i){
        esid += char(EEPROM.read(i));
    }
    ECHO("SSID: ");
    ECHOLN(esid);
    ECHOLN("Reading EEPROM pass");
    epass = "";
    for (int i = EEPROM_WIFI_PASS_START; i < EEPROM_WIFI_PASS_END; ++i){
        epass += char(EEPROM.read(i));
    }
    ECHO("PASS: ");
    ECHOLN(epass);

    ECHOLN("Reading EEPROM Device ID");
    deviceId = EEPROM.read(EEPROM_WIFI_DEVICE_ID);
    ECHO("ID: ");
    ECHOLN(deviceId);

    ECHOLN("Reading EEPROM server");
    sever = "";
    for (int i = EEPROM_WIFI_SERVER_START; i < EEPROM_WIFI_SERVER_END; ++i){
        sever += char(EEPROM.read(i));
    }
    ECHO("SERVER: ");
    ECHOLN(sever);
    
    testWifi(esid, epass);
}

bool testWifi(String esid, String epass) {
    ECHO("Connecting to: ");
    ECHOLN(esid);
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    server.close();
    // delay(1000);
    ECHO("delay: ");
    ECHOLN((deviceId-1)*3000 + 1000);
    for(int i = 0; i < 100; i++){
        if(digitalRead(PIN_CONFIG) == LOW){
            break;
        }
        delay(((deviceId-1)*3000 + 1000)/100);
    }
    WiFi.mode(WIFI_STA);        //bat che do station
    WiFi.begin(esid.c_str(), epass.c_str());
    int c = 0;
    ECHOLN("Waiting for Wifi to connect");
    while (c < 40) {
        if (WiFi.status() == WL_CONNECTED) {
            ECHOLN("\rWifi connected!");
            ECHO("Local IP: ");
            ECHOLN(WiFi.localIP());
            // digitalWrite(ledTestWifi, HIGH);
            ConnecttoMqttServer();
            return true;
        }
        if(digitalRead(PIN_CONFIG) == LOW){
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





// void StartNormalSever(){
//     server.on("/", HTTP_GET, handleRoot);
//     server.on("/getstatus", HTTP_GET, getStatus);
//     server.on("/setmoderun", HTTP_POST, setModeRunBegin);
//     server.on("/setlowspeed", HTTP_POST, setPercentLowSpeed);
//     server.on("/control_led", HTTP_POST, ControlLed);
//     server.on("/settimereturn", HTTP_POST, setTimeReturn);
//     server.on("/touchsensor", HTTP_POST, touchSensor);
//     server.on("/resetdistant", HTTP_GET, resetDistant);
//     server.on("/open", HTTP_GET, Open);
//     server.on("/close", HTTP_GET, Close);
//     server.on("/stop", HTTP_GET, Stop);
//     server.begin();
//     ECHOLN("HTTP server started");
// }

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
                    if(sensorValue[i] == deviceId){
                        isTrueControl = true;
                        break;
                    }
                }
                if(isTrueControl == true){
                    String dataType = rootData["typecontrol"];
                    //---------control color------------------
                    if(dataType == "controlled"){
                        if(statusLed == true){
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
                    if(sensorValue[i] == deviceId){
                        isTrueControl = true;
                        break;
                    }
                }
                if(isTrueControl == true){
                    String dataType = rootData["typecontrol"];
                    //---------getstatus------------------
                    if(dataType == "getstatus"){
                        getStatus();
                    }
                    //---------control------------------
                    else if(dataType == "control"){
                        String dataControl = rootData["data"];
                        if(dataControl == "open"){
                            if(statusStop = true && fristRun == false && countPulFGDistant >= (countPulDistant - 3)){
                                return;
                            }
                            Open();
                        }else if(dataControl == "close"){
                            if(statusStop = true && fristRun == false && countPulFGDistant <= 3){
                                return;
                            }
                            Close();
                        }
                        else if(dataControl == "stop"){
                            if(statusStop != true){
                                Stop();
                            }
                        }
                    }
                    //---------controlled------------------
                    else if(dataType == "controlled"){
                        if(statusLed == true){
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
                    //---------resetdistant------------------
                    else if(dataType == "resetdistant"){
                        resetDistant();
                    }
                    //---------settimereturn------------------
                    else if(dataType == "settimereturn"){
                        String dataTimereturn = rootData["data"];
                        time_return = dataTimereturn.toInt();
                        ECHO("Writed: ");
                        ECHO(time_return);
                        EEPROM.write(EEPROM_SET_TIME_RETURN, char(time_return));
                        EEPROM.commit();
                    }
                    //---------setmoderun------------------
                    else if(dataType == "setmoderun"){
                        String setmoderunstr = rootData["data"];
                        setmoderunbegin = setmoderunstr.toInt();
                        setmoderunbeginchange = setmoderunbegin;
                        ECHO("Writed: ");
                        ECHOLN(setmoderunbegin);
                        EEPROM.write(EEPROM_SET_MODE_RUN_BEGIN, char(setmoderunbegin));
                        EEPROM.commit();
                    }
                    //---------percent in out------------------
                    else if(dataType == "setlowspeed"){
                        int percentSlow[2];
                        for (int i = 0; i < 2; i++) { //Iterate through results
                            percentSlow[i] = rootData["data"][i];  //Implicit cast
                            ECHOLN(percentSlow[i]);
                        }
                        percentLowSpeedIn = percentSlow[0];
                        percentLowSpeedOut = percentSlow[1];
                        ECHO("Writed: ");
                        ECHO(percentLowSpeedOut);
                        ECHO(",");
                        ECHOLN(percentLowSpeedIn);
                        EEPROM.write(EEPROM_SET_PERCENT_OUT_LOW_SPEED, char(percentLowSpeedOut));
                        EEPROM.write(EEPROM_SET_PERCENT_IN_LOW_SPEED, char(percentLowSpeedIn));
                        EEPROM.commit();
                    }
                    else if(dataType == "changedelaymqtt"){
                        String delay_mqtt_loop_Str = rootData["data"];
                        delay_mqtt_loop = delay_mqtt_loop_Str.toInt();
                        ECHO("Writed: ");
                        ECHOLN(delay_mqtt_loop);
                        EEPROM.write(EEPROM_DELAY_MQTT_TO_LOW_SPEAKER, delay_mqtt_loop);
                        EEPROM.commit();
                    }
                    else if(dataType == "changedelayanalog"){
                        String delay_analog_Str = rootData["data"];
                        count_check_analog_pin = delay_analog_Str.toInt();      //1 = 100ms
                        ECHO("Writed: ");
                        ECHOLN(count_check_analog_pin);
                        EEPROM.write(EEPROM_COUNT_TIME_ANALOG, count_check_analog_pin);
                        EEPROM.commit();
                    }
                    else if(dataType == "changeerroranalog"){
                        String error_analog_Str = rootData["data"];
                        valueErrorAnalogRead = error_analog_Str.toInt();      
                        ECHO("Writed: ");
                        ECHOLN(valueErrorAnalogRead);
                        EEPROM.write(EEPROM_VALUE_ERROR_ANALOG, valueErrorAnalogRead);
                        EEPROM.commit();
                    }
                    else if(dataType == "timeautoclose"){
                        String Strtimeautoclose = rootData["data"];
                        defineTimeAutoClose = Strtimeautoclose.toInt();      
                        ECHO("Writed: ");
                        ECHOLN(defineTimeAutoClose);
                        EEPROM.write(EEPROM_TIME_AUTO_CLOSE, defineTimeAutoClose);
                        EEPROM.commit();
                    }
                }
            }
        }
    }
}


bool reconnect() {
    // Loop until we're reconnected
    ECHO("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Avy-";
    // clientId += GetFullSSID;
    clientId += String(random(0xffffff), HEX);
    const char *user = "tkrvqyzs";
    const char *pass = "lS_VJSmDuiXC";
    const char* willTopic = "CabinetAvy/HPT/LWT";
    uint8_t willQos = 0;
    boolean willRetain = false;
    String willMessage = "{\"devicetype\" : \"";
    willMessage += m_Typedevice;
    willMessage += "\", \"deviceid\" : \"";
    willMessage += String(deviceId);
    willMessage += "\", \"status\" : \"error\"}";
    // Attempt to connect
    // if (client.connect(clientId.c_str(), user, pass)){
    if (client.connect(clientId.c_str(), user, pass, willTopic, willQos, willRetain, willMessage.c_str())) {
    // if (client.connect(clientId.c_str())) {
        ECHO("connected with id: ");
        ECHOLN(clientId);
        // Once connected, publish an announcement...
        // client.publish("outTopic", "hello world");
        // ... and resubscribe
        // String topicGetstatus = m_Pretopic + m_Getstatus;
        String topicControl = m_Pretopic + m_Control;
        String topicControlhand = m_Pretopic + m_Controlhand;

        // char topicGetstatusArray[topicGetstatus.length() + 1];
        // char topicControlArray[topicControl.length() + 1];

        // topicGetstatus.toCharArray(topicGetstatusArray, topicGetstatus.length() + 1);
        // topicControl.toCharArray(topicControlArray, topicControl.length() + 1);
        
        // client.subscribe(topicGetstatusArray);
        client.subscribe(topicControl.c_str());
        client.subscribe(topicControlhand.c_str());
        ECHO("Done Subscribe Channel: ");
        // ECHO(topicGetstatus);
        // ECHO("  +  ");
        ECHO(topicControl);
        ECHO(", ");
        ECHOLN(topicControlhand);
        countDisconnectToServer++;
        if(flag_disconnect_to_sever == true){
            sum_time_disconnect_to_sever += millis() - count_time_disconnect_to_sever;
            flag_disconnect_to_sever = false;
        }
        digitalWrite(ledTestWifi, HIGH);
        getStatus();
        SendStatusReconnect();
    } 
    if(!client.connected()){
        ECHO("failed, rc=");
        ECHO(client.state());
        ECHOLN(" try again in 2 seconds");
    }
    return client.connected();
}


void caculateSpeed(){
    //ECHOLN("speed");
    if(timecaculateSpeed <= 30){
        timecaculateSpeed++;
    }
    pul = countPulFG;
    speed = (pul - prepul)/(0.1*6);
    prepul = pul;
    

    if(countFrirstRun == 2 && countPulFG > stop_dau && countPulFG < (countPulDistant - stop_cuoi)){        //countFrirstRun = 2 la luc bat dau qua trinh chay nhanh (modeFast = true)
        countFrirstRun = 10;        //lan dau tien chay nhanh
    }


    // ECHO("van toc: ");
    // ECHOLN(speed);
    if(abs(speed) <= MINSPEED && timecaculateSpeed >= 5){   //sau 5 lan chay thi moi tinh den van toc
        ECHOLN("Da dung lai");
        tickerCaculateSpeed.stop();
        SetPWMspeed.stop();
        digitalWrite(PWM, LOW);
        statusStop = false;

        

        if(isSaveDistant == true && fristRun == true){
            fristRun = false;
            countPulFGDistant = 0;
            prepul = 0;
        }
        


        if(fristRun == true && countFrirstRun < 5){
            countFrirstRun++;
        }
        
        if(fristRun == true && countFrirstRun == 1){
            countPulFGDistant = 0;
            prepul = 0;
        }
        else if(fristRun == true && countFrirstRun == 2){
            countPulDistant = abs(countPulFGDistant);
            EEPROM.write(EEPROM_DISTANT, countPulDistant);
            EEPROM.commit();
            isSaveDistant = true;
            if(countPulFGDistant < 0){
                countPulFGDistant = 0;
                prepul = 0;
            }
            fristRun = false;
        }


        if(fristRun == false && countPulFGDistant <= 3){
			//tat den
			tickerSetPwmLedLightOff.stop();
			tickerSetPwmLedLightOn.stop();
			countChangeLed = 0;
			ledcWrite(LED_CHANNEL_R, 0);
            ledcWrite(LED_CHANNEL_G, 0);
            ledcWrite(LED_CHANNEL_B, 0);

			countPulFGDistant = 0;
            flag_send_status_when_use_hand = true;
        }else if(fristRun == false && (countPulDistant -3) <= countPulFGDistant){
            //bat den
			tickerSetPwmLedLightOff.stop();
			tickerSetPwmLedLightOn.stop();
			countChangeLed = 0;
			ledcWrite(LED_CHANNEL_R, uint8_t(countLedLightRed_After*AlphaLed));
            ledcWrite(LED_CHANNEL_G, uint8_t(countLedLightGreen_After*AlphaLed));
            ledcWrite(LED_CHANNEL_B, uint8_t(countLedLightBlue_After*AlphaLed));
			
			countPulFGDistant = countPulDistant;
            flag_send_status_when_use_hand = true;
        }
        
        if(Forward == true){
            digitalWrite(DIR, QUAY_NGHICH);     //cho dong co quay nghich
            Forward = false;
        }else{
            digitalWrite(DIR, QUAY_THUAN);
            Forward = true;
        }

        timecaculateSpeed = 0;
        SetPWMStopSpeed.start();    
    }
}



void setpwmMotor(){
    countSetPwm++;
    if(co < 20){
        co ++;
    }
    if(co == 10){
        microsecondPwmmotor = micros();
    }
    if(co == 11 && printlnmicoros == true){
        ECHOLN(micros() - microsecondPwmmotor);
        printlnmicoros = false;
        co = 0;
    }
    
    switch(setmoderunbeginchange){
        case 1:{
            if(modeFast == false){ //ti le PWM la 4 HIGH 1 LOW
                switch (countSetPwm){
                    case 1: {
                        digitalWrite(PWM, HIGH);
                        break;
                    }
                    case 4: {
                        digitalWrite(PWM, LOW);
                        countSetPwm = 0;             
                        break;
                    }
                }
            }else{                //ti le PWM la 1 HIGH 3 LOWset
                digitalWrite(PWM, HIGH);
            }
            break;
        }
        case 2:{
            if(modeFast == false){ //ti le PWM la 3 HIGH 2 LOW
                switch (countSetPwm){
                    case 1: {
                        digitalWrite(PWM, HIGH);
                        break;
                    }
                    case 3: {
                        digitalWrite(PWM, LOW);
                        break;
                    }
                    case 4: {
                        countSetPwm = 0;
                        break;
                    }
                }
            }else{                //ti le PWM la 1 HIGH 3 LOW
                digitalWrite(PWM, HIGH);
            } 
            break;
        }
        case 3:{
            if(modeFast == false){ //ti le PWM la 2 HIGH 3 LOW
                switch (countSetPwm){
                    case 1: {
                        digitalWrite(PWM, HIGH);
                        break;
                    }
                    case 2: {
                        digitalWrite(PWM, LOW);
                        break;
                    }
                    case 4: {
                        countSetPwm = 0;
                        break;
                    }
                }
            }else{                //ti le PWM la 1 HIGH 3 LOW
                digitalWrite(PWM, HIGH);
            }
            break;
        }
        // case 4:{
        //     if(modeFast == false){ //ti le PWM la 1 HIGH 4 LOW
        //         switch (countSetPwm){
        //             case 1: {
        //                 digitalWrite(PWM, HIGH);
        //                 break;
        //             }
        //             case 2: {
        //                 digitalWrite(PWM, LOW);
        //                 break;
        //             }
        //             case 4: {
        //                 countSetPwm = 0;
        //                 break;
        //             }
        //         }
        //     }else{                //ti le PWM la 1 LOW 4 HIGH
        //         digitalWrite(PWM, HIGH);
        //     } 
        //     break;
        // }

    }

}

void setpwmStopMotor(){
    count_stop_motor++;
    //ECHOLN(count_stop_motor);
    // if(Forward == true){
    //     digitalWrite(DIR, HIGH);
    // }else{
    //     digitalWrite(DIR, LOW);
    // }
    if(count_stop_motor % 3 == 0){
        digitalWrite(PWM, HIGH);
    }else{
        digitalWrite(PWM, LOW);
        //countSetPwm = 0;
    }
    if(count_stop_motor >= time_return*1000){       //time_return *10 ms
        SetPWMStopSpeed.stop();
        // delay(100);
        digitalWrite(PWM, LOW);
        delay(100);
        luu_trang_thai_cua_sensor_ngay_khi_dung_lai = loai_bien_giong_nhau_cua_cam_bien;
        // ECHOLN(luu_trang_thai_cua_sensor_ngay_khi_dung_lai);
        loai_bien_giong_nhau_cua_cam_bien = 0;
        count_stop_motor = 0;


        statusStop = true;
        daytay = true;
        if(flag_send_status_when_use_hand == true && Forward == true){      //dang o trang thai dong cua
            String datasend = "{\"deviceid\" : \"";
            datasend += String(deviceId);
            datasend += "\", \"devicetype\" : \"motor\", \"typecontrol\" : \"control\",  \"number_disconnect\" : \"";
            datasend += String(countDisconnectToServer - 1);
            datasend += "\", \"all_time\" : \"";
            datasend += String(sum_time_disconnect_to_sever);
            datasend += "\", \"status\" : \"close\"}";
            client.publish(topicsendStatus, datasend.c_str());


            flag_send_status_when_use_hand = false;
        }
        else if(flag_send_status_when_use_hand == true && Forward == false){    //dang o trang thai mo cua
            String datasend = "{\"deviceid\" : \"";
            datasend += String(deviceId); 
            datasend += "\", \"devicetype\" : \"motor\", \"typecontrol\" : \"control\",  \"number_disconnect\" : \"";
            datasend += String(countDisconnectToServer - 1);
            datasend += "\", \"all_time\" : \"";
            datasend += String(sum_time_disconnect_to_sever);
            datasend += "\", \"status\" : \"open\"}";
            client.publish(topicsendStatus, datasend.c_str());

            flag_send_status_when_use_hand = false;
        }
        flag_reset_value_analog = true;
        
    }
    
}




void setLedApMode() {
    digitalWrite(ledTestWifi, !digitalRead(ledTestWifi));
}

void setPwmLedLighton(){
    countChangeLed++;
    float out_led_red, out_led_green, out_led_blue;
    out_led_red = (float)0 + (((float)countLedLightRed_After - (float)0)/255)*countChangeLed;
    out_led_red = abs(out_led_red);
    ledcWrite(LED_CHANNEL_R, uint8_t(out_led_red*AlphaLed));

    out_led_green = (float)0 + (((float)countLedLightGreen_After - (float)0)/255)*countChangeLed;
    out_led_green = abs(out_led_green);
    ledcWrite(LED_CHANNEL_G, uint8_t(out_led_green*AlphaLed));

    out_led_blue = (float)0 + (((float)countLedLightBlue_After - (float)0)/255)*countChangeLed;
    out_led_blue = abs(out_led_blue);
    ledcWrite(LED_CHANNEL_B, uint8_t(out_led_blue*AlphaLed));

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
	ledcWrite(LED_CHANNEL_R, uint8_t(out_led_red*AlphaLed));
    
    out_led_green = (float)countLedLightGreen_Before + (((float)0 - (float)countLedLightGreen_Before)/255)*countChangeLed;
    out_led_green = abs(out_led_green);
	ledcWrite(LED_CHANNEL_G, uint8_t(out_led_green*AlphaLed));
    
    out_led_blue = (float)countLedLightBlue_Before + (((float)0 - (float)countLedLightBlue_Before)/255)*countChangeLed;
    out_led_blue = abs(out_led_blue);
	ledcWrite(LED_CHANNEL_B, uint8_t(out_led_blue*AlphaLed));
    
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
    ledcWrite(LED_CHANNEL_R, uint8_t(out_led_red*AlphaLed));

    out_led_green = (float)countLedLightGreen_Before + (((float)countLedLightGreen_After - (float)countLedLightGreen_Before)/255)*countChangeLed;
    out_led_green = abs(out_led_green);
    ledcWrite(LED_CHANNEL_G, uint8_t(out_led_green*AlphaLed));

    out_led_blue = (float)countLedLightBlue_Before + (((float)countLedLightBlue_After - (float)countLedLightBlue_Before)/255)*countChangeLed;
    out_led_blue = abs(out_led_blue);
    ledcWrite(LED_CHANNEL_B, uint8_t(out_led_blue*AlphaLed));

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


void SetupConfigMode(){
    ECHOLN("[WifiService][setupAP] Open AP....");
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    server.close();
    delay(500);
    WiFi.mode(WIFI_AP_STA);
    String SSID_AP_MODE = SSID_PRE_AP_MODE + GetFullSSID();
    WiFi.softAP(SSID_AP_MODE.c_str(), PASSWORD_AP_MODE);
    IPAddress APIP(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(APIP, gateway, subnet);
    // ECHOLN(SSID_AP_MODE);

    ECHOLN("[WifiService][setupAP] Softap is running!");
    IPAddress myIP = WiFi.softAPIP();
    ECHO("[WifiService][setupAP] IP address: ");
    ECHOLN(myIP);
}


void StartConfigServer(){    
    ECHOLN("[HttpServerH][startConfigServer] Begin create new server...");
    server.on("/config", HTTP_POST, ConfigMode);


    /*return index page which is stored in serverIndex */
    server.on("/", HTTP_GET, []() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/html", loginIndex);
    });
    server.on("/serverIndex", HTTP_GET, []() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/html", serverIndex);
    });
    /*handling uploading firmware file */
    server.on("/update", HTTP_POST, []() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
        ESP.restart();
    }, []() {
        HTTPUpload& upload = server.upload();
        if (upload.status == UPLOAD_FILE_START) {
        Serial.printf("Update: %s\n", upload.filename.c_str());
            if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
                Update.printError(Serial);
            }
        } else if (upload.status == UPLOAD_FILE_WRITE) {
        /* flashing firmware to ESP*/
            if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
                Update.printError(Serial);
            }
        } else if (upload.status == UPLOAD_FILE_END) {
            if (Update.end(true)) { //true to set the size to the current progress
                Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
            } else {
                Update.printError(Serial);
            }
        }
    });


    server.begin();
    ECHOLN("[HttpServerH][startConfigServer] HTTP server started");
}

String GetFullSSID() {
    uint8_t mac[WL_MAC_ADDR_LENGTH];
    String macID;
    WiFi.mode(WIFI_AP);
    WiFi.softAPmacAddress(mac);
    macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) + String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
    macID.toUpperCase();
    ECHO("[Helper][getIdentify] Identify: ");
    ECHO(SSID_PRE_AP_MODE);
    ECHOLN(macID);
    return macID;
}


void ConfigMode(){
    StaticJsonBuffer<RESPONSE_LENGTH> jsonBuffer;
    ECHOLN(server.arg("plain"));
    JsonObject& rootData = jsonBuffer.parseObject(server.arg("plain"));
    ECHOLN("--------------");
    tickerSetApMode.stop();
    digitalWrite(ledTestWifi, LOW);
    if (rootData.success()) {
        server.sendHeader("Access-Control-Allow-Headers", "*");
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "application/json; charset=utf-8", "{\"status\":\"success\"}");
        //server.stop();
        String nssid = rootData["ssid"];
        String npass = rootData["password"];
        String nid = rootData["deviceid"];
        String nserver = rootData["server"];


        esid = nssid;
        epass = npass;
        deviceId = nid.toInt();
        sever = nserver;

        ECHOLN("clearing eeprom");
        for (int i = 0; i <= EEPROM_WIFI_SERVER_END; i++){ 
            EEPROM.write(i, 0); 
        }
        ECHOLN("writing eeprom ssid:");
        ECHO("Wrote: ");
        for (int i = 0; i < nssid.length(); ++i){
            EEPROM.write(i+EEPROM_WIFI_SSID_START, nssid[i]);             
            ECHO(nssid[i]);
        }
        ECHOLN("");
        ECHOLN("writing eeprom pass:"); 
        ECHO("Wrote: ");
        for (int i = 0; i < npass.length(); ++i){
            EEPROM.write(i+EEPROM_WIFI_PASS_START, npass[i]);
            ECHO(npass[i]);
        }
        ECHOLN("");
        ECHOLN("writing eeprom device id:"); 
        ECHO("Wrote: ");
        EEPROM.write(EEPROM_WIFI_DEVICE_ID, deviceId);
        ECHOLN(deviceId);

        ECHOLN("writing eeprom server:");
        ECHO("Wrote: ");
        for (int i = 0; i < nserver.length(); ++i){
            EEPROM.write(i+EEPROM_WIFI_SERVER_START, nserver[i]);
            ECHO(nserver[i]);
        }
        ECHOLN("");

        EEPROM.commit();
        ECHOLN("Done writing!");

        if (testWifi(nssid, npass)) {

            // ConnecttoMqttServer();
            Flag_Normal_Mode = true;
            return;
        }
        tickerSetApMode.start();
        ECHOLN("Wrong wifi!!!");
        SetupConfigMode();
        StartConfigServer();
        return;
    }
    ECHOLN("Wrong data!!!");
}




void IRAM_ATTR dirhallSensor1(){      //nhan du lieu tu cam bien ben ngoai
    if(loai_bien_giong_nhau_cua_cam_bien != 1){
        loai_bien_giong_nhau_cua_cam_bien = 1;
        ECHOLN("1");
        if(Forward == true){
            countPulFGDistant++;
        }else{
            countPulFGDistant--;
        }

        if(daytay == true && statusStop == true){
            // loai_bien_giong_nhau_cua_cam_bien = 0;
            if(countHall2 == true){
                //cho dong co chay thuan
                countHall2 = false;
                // ECHOLN("open");
                Open();
            }
            else if(countHall6 == true){
                //cho dong co chay nghich
                countHall6 = false;
                // ECHOLN("close");
                Close();
            }
            else{
                countHall1 = true;
            }
        }
    }
    
}

void IRAM_ATTR dirhallSensor2(){
    if(loai_bien_giong_nhau_cua_cam_bien != 2){
        loai_bien_giong_nhau_cua_cam_bien = 2;
        ECHOLN("2");
        if(Forward == true){
            countPulFGDistant++;
        }else{
            countPulFGDistant--;
        }

        // ECHOLN(co untPulFGDistant);
        if(daytay == true && statusStop == true){
            // loai_bien_giong_nhau_cua_cam_bien = 0;
            if(countHall3 == true){
                //cho dong co chay thuan
                countHall3 = false;
                // ECHOLN("open");
                Open();
            }
            else if(countHall1 == true){
                //cho dong co chay nghich
                countHall1 = false;
                // ECHOLN("close");
                Close();
            }
            else{
                countHall2 = true;
            }
        }
    }
}
void IRAM_ATTR dirhallSensor3(){
    if(loai_bien_giong_nhau_cua_cam_bien != 3){
        loai_bien_giong_nhau_cua_cam_bien = 3;
        ECHOLN("3");
        if(Forward == true){
            countPulFGDistant++;
        }else{
            countPulFGDistant--;
        }

        if(daytay == true && statusStop == true){
            // loai_bien_giong_nhau_cua_cam_bien = 0;
            if(countHall4 == true){
                //cho dong co chay thuan
                countHall4 = false;
                // ECHOLN("open");
                Open();
            }
            else if(countHall2 == true){
                //cho dong co chay nghich
                countHall2 = false;
                // ECHOLN("close");
                Close();
            }
            else{
                countHall3 = true;
            }
        }
    }
}

void IRAM_ATTR dirhallSensor4(){
    if(loai_bien_giong_nhau_cua_cam_bien != 4){
        loai_bien_giong_nhau_cua_cam_bien = 4;
        ECHOLN("4");
        if(Forward == true){
            countPulFGDistant++;
        }else{
            countPulFGDistant--;
        }

        if(daytay == true && statusStop == true){
            // loai_bien_giong_nhau_cua_cam_bien = 0;
            if(countHall5 == true){
                //cho dong co chay thuan
                countHall5 = false;
                // ECHOLN("open");
                Open();
            }
            else if(countHall3 == true){
                //cho dong co chay nghich
                countHall3 = false;
                // ECHOLN("close");
                Close();
            }
            else{
                countHall4 = true;
            }
        }
    }
}

void IRAM_ATTR dirhallSensor5(){
    if(loai_bien_giong_nhau_cua_cam_bien != 5){
        loai_bien_giong_nhau_cua_cam_bien = 5;
        ECHOLN("5");
        if(Forward == true){
            countPulFGDistant++;
        }else{
            countPulFGDistant--;
        }

        if(daytay == true && statusStop == true){
            // loai_bien_giong_nhau_cua_cam_bien = 0;
            if(countHall6 == true){
                //cho dong co chay thuan
                countHall6 = false;
                // ECHOLN("open");
                Open();
            }
            else if(countHall4 == true){
                //cho dong co chay nghich
                countHall4 = false;
                // ECHOLN("close");
                Close();
            }
            else{
                countHall5 = true;
            }
        }
    }
}

void IRAM_ATTR dirhallSensor6(){
    if(loai_bien_giong_nhau_cua_cam_bien != 6){
        loai_bien_giong_nhau_cua_cam_bien = 6;
        ECHOLN("6");
        if(Forward == true){
            countPulFGDistant++;
        }else{
            countPulFGDistant--;
        }

        if(daytay == true && statusStop == true){
            // loai_bien_giong_nhau_cua_cam_bien = 0;
            if(countHall1 == true){
                //cho dong co chay thuan
                countHall1 = false;
                // ECHOLN("open");
                Open();
            }
            else if(countHall5 == true){
                //cho dong co chay nghich
                countHall5 = false;
                // ECHOLN("close");
                Close();
            }
            else{
                countHall6 = true;
            }
        }
    }
}


void IRAM_ATTR inputSpeed(){
    if(Forward == true){
        countPulFG++;
    }else{
        countPulFG--;
    }
    // ECHOLN(countPulFG);
}


void checkTouchSensor(){
    if(abs(millis() - time_check_touch_sensor) > TIME_CHECK_TOUCH_SENSOR){
        time_check_touch_sensor = millis();
        for(int i = 0; i <= 2; i++){
            valueTouchSensor = touchRead(PIN_TOUCH_SENSOR);
            if(abs(valueTouchSensorFix - valueTouchSensor) <= VALUE_ERROR_TOUCH_SENSOR){
                return;
            }
        }

        ECHOLN("Touch Sensor!");
        digitalWrite(PWM, LOW);
        daytay = false;
        delay(500);
        if(statusStop == false){
            Stop();
        }
        else if(Forward == false && statusStop == true){
            Close();
        }else{
            Open();
        }
    }
}


void checkAnalogReadButton(){
    //reset_value_analog
    //flag_reset_value_analog dung de reset lai gia tri bien tro: pre = current
    //flag = true thi se reset, con bang false la da reset roi, vi the load gia tri phai la bang false
    if(flag_reset_value_analog == true){
        valueAnalogRead = analogRead(ANALOG_READ_BUTTON);
        prevalueAnalogRead = valueAnalogRead;
        flag_reset_value_analog = false;
    }

    //analogRead
    if(flag_reset_value_analog == false && statusStop == true && Forward == true && abs(millis() - time_check_analog_pin) > TIME_CHECK_ANALOG){
        time_check_analog_pin = millis();
        valueAnalogRead = analogRead(ANALOG_READ_BUTTON);
        if(abs(valueAnalogRead - prevalueAnalogRead) > valueErrorAnalogRead){
            for(int i = 0; i < count_check_analog_pin; i++){
                valueAnalogRead = analogRead(ANALOG_READ_BUTTON);
                if(abs(valueAnalogRead - prevalueAnalogRead) <= valueErrorAnalogRead){
                    return;
                }
                delay(100);
            }
            ECHOLN("Analog Sensor!");
            Open();
        }
        
    }
}

void setSpeedControl(){
    // ECHOLN(countPulFGDistant > (percentLowSpeedOut/100)*countPulDistant);
    if(fristRun == false && Forward == true && countPulFGDistant < ((100 - (float)percentLowSpeedOut)/100)*countPulDistant){
        modeFast = true;
    }else if(fristRun == false && Forward == false && countPulFGDistant > ((float)percentLowSpeedIn/100)*countPulDistant){
        modeFast = true;       
    }else{
        modeFast = false;
    }
}


void checkAutoClose(){
    //reset lai bien flagAutoClose
    if(Forward == true && statusStop == true && flagAutoClose == true){
        flagAutoClose = false;
    }
    //setup bien flagAutoClose len true, day la thoi diem tu mo va bat dau tinh thoi gian
    if(Forward == false && statusStop == true  && countPulFGDistant >= (countPulDistant - 3) && flagAutoClose == false){
        timeAutoClose = millis();
        flagAutoClose = true;
        ECHO("Start time auto close: ");
        ECHO(defineTimeAutoClose);
        ECHOLN("(min)");
    }
    if(flagAutoClose == true && millis() >= timeAutoClose + defineTimeAutoClose*1000*60){       //don vi tinh theo phut
        flagAutoClose = false;
        Close();
    }
}


void tickerupdate(){
    tickerCaculateSpeed.update();
    SetPWMspeed.update();
    SetPWMStopSpeed.update();
    tickerSetApMode.update();
	tickerSetPwmLedLightOn.update();
	tickerSetPwmLedLightOff.update();
    tickerSetPwmLedLightChange.update();
}



void setup() {
  // put your setup code here, to run once:
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
    Serial.begin(115200);
    EEPROM.begin(EEPROM_WIFI_MAX_CLEAR);
    
    
    ledcSetup(LED_CHANNEL_R, 1000, 8); // 1 kHz PWM, 8-bit resolution
    ledcSetup(LED_CHANNEL_G, 1000, 8); // 1 kHz PWM, 8-bit resolution
    ledcSetup(LED_CHANNEL_B, 1000, 8); // 1 kHz PWM, 8-bit resolution

    ledcAttachPin(PIN_LED_LIGHT_R, LED_CHANNEL_R); // analog pin to channel led_R
    ledcAttachPin(PIN_LED_LIGHT_G, LED_CHANNEL_G); // analog pin to channel led_G
    ledcAttachPin(PIN_LED_LIGHT_B, LED_CHANNEL_B); // analog pin to channel led_B
    
    delay(10);
    ECHOLN("");
    ECHOLN("START!!!");
    pinMode(DIR, OUTPUT);
    pinMode(PWM, OUTPUT);
    pinMode(PIN_CONFIG, INPUT_PULLUP);
    pinMode(inputFG, INPUT_PULLUP);
    pinMode(hallSensor1, INPUT_PULLUP);
    pinMode(hallSensor2, INPUT_PULLUP);
    pinMode(hallSensor3, INPUT_PULLUP);
    pinMode(hallSensor4, INPUT_PULLUP);
    pinMode(hallSensor5, INPUT_PULLUP);
    pinMode(hallSensor6, INPUT_PULLUP);
    pinMode(ledTestWifi, OUTPUT);
    digitalWrite(ledTestWifi, LOW);
    
    
    loadDataBegin();
    SetupNetwork();


    attachInterrupt(digitalPinToInterrupt(hallSensor1), dirhallSensor1, RISING);
    attachInterrupt(digitalPinToInterrupt(hallSensor2), dirhallSensor2, RISING);
    attachInterrupt(digitalPinToInterrupt(hallSensor3), dirhallSensor3, RISING);
    attachInterrupt(digitalPinToInterrupt(hallSensor4), dirhallSensor4, RISING);
    attachInterrupt(digitalPinToInterrupt(hallSensor5), dirhallSensor5, RISING);
    attachInterrupt(digitalPinToInterrupt(hallSensor6), dirhallSensor6, RISING);
    attachInterrupt(digitalPinToInterrupt(inputFG), inputSpeed, FALLING);

}

void loop() {
    // put your main code here, to run repeatedly:

    if(WiFi.status() == WL_CONNECTED){
        //tat den led khi quay ve
        if(Forward == false && statusLed == true && statusStop == false  && countPulFGDistant <= 10){
            statusLed = false;
            tickerSetPwmLedLightOn.stop();
            tickerSetPwmLedLightOff.start();
        }
        
        if(defineTimeAutoClose != 0){
            checkAutoClose();
        }
        checkAnalogReadButton();
        setSpeedControl();
        if(flag_is_touch_sensor == true){
            checkTouchSensor();
        }
        if (!client.connected()) {
            digitalWrite(ledTestWifi, LOW);
            if(flag_disconnect_to_sever == false){
                count_time_disconnect_to_sever = millis();
                lastReconnectAttempt = millis();
                flag_disconnect_to_sever = true;
            }
            unsigned long nowReconnectAttempt = millis();

            if (abs(nowReconnectAttempt - lastReconnectAttempt) > 10000 && statusStop == true) {
                lastReconnectAttempt = nowReconnectAttempt;
                reconnect();
            }
        }else{
            // client.loop();
            unsigned long nowClientMqttLoop = millis();
            if (abs(nowClientMqttLoop - lastClientMqttLoop) > delay_mqtt_loop) {
                lastClientMqttLoop = nowClientMqttLoop;
                client.loop();
            }
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

    tickerupdate();
    if(Flag_Normal_Mode == false){
        server.handleClient();
    }


    if (Flag_Normal_Mode == true && WiFi.status() != WL_CONNECTED){
        digitalWrite(ledTestWifi, LOW);
        if(flag_disconnect_to_sever == false){
            count_time_disconnect_to_sever = millis();
            flag_disconnect_to_sever = true;
        }
        SetPWMspeed.stop();
        tickerCaculateSpeed.stop();
        digitalWrite(PWM, LOW);
        delay(100);
        statusStop = true;
        daytay = true;
        testWifi(esid, epass);
    }    


    //hold to config mode
    if(digitalRead(PIN_CONFIG) == HIGH){
        time_click_button = millis();
    }
    if(digitalRead(PIN_CONFIG) == LOW && (time_click_button + CONFIG_HOLD_TIME) <= millis()){
        time_click_button = millis();
        // Stop();
        digitalWrite(ledTestWifi, LOW);
        Flag_Normal_Mode = false;
        tickerSetApMode.start();
        SetupConfigMode();
        StartConfigServer();
    }


}
