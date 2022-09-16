#include "temperature_sensor.h"

extern int Value_Temperature;
void getStatus(){
    String datasend = "{\"deviceid\" : \"";
    datasend += String(deviceId);
    datasend += "\", \"devicetype\" : \"";
    if(digitalRead(PIN_MODE_IN_OUT) == IN){
        datasend += "temperaturesIn";
    }else{
        datasend += "temperaturesOut";
    }
    datasend += "\", \"typecontrol\" : \"getstatus\",  \"number_disconnect\" : \"";
    datasend += String(countDisconnectToServer - 1);
    datasend += "\", \"all_time\" : \"";
    datasend += String(sum_time_disconnect_to_sever);
    datasend +=  "\", \"data\" : \"";
    datasend += String(templateAfter);
    datasend += "\"}";
    client.publish(topicsendStatus, datasend.c_str());
    ECHOLN("-------getStatus-------");
}

void setLedApMode() {
    digitalWrite(LED_TEST_AP, !digitalRead(LED_TEST_AP));
}

void setBuzzer(){
    couter_buzzer ++;
    switch (couter_buzzer)
    {
    case 1:
        digitalWrite(PIN_BUZZER, HIGH);
        break;
    case 2:
        digitalWrite(PIN_BUZZER, LOW);
        break;
    case 3:
        digitalWrite(PIN_BUZZER, HIGH);
        break;
    case 4:
        digitalWrite(PIN_BUZZER, LOW);
        break;
    case 30:
        couter_buzzer = 0;
        tickerBuzzer.stop();
        break;
    default:
        break;
    }
}

void handleRoot(){
    char index_html[2048];
    snprintf_P(index_html, sizeof(index_html), index_html_handle_root_1, Value_Temperature);
	server.send(200, "text/html", index_html);

}

void ConfigModeWifi(){
    tickerSetApMode.stop();
    uint8_t count = 0;
    String nssid;
    String npass;
    String nserver;
    for (uint8_t i = 0; i < server.args(); i++) {
        if(server.argName(i) == "wifi_ssid" && server.arg(i) != ""){
            count ++;
            nssid = server.arg(i);
        }
        if(server.argName(i) == "wifi_password" && server.arg(i) != ""){
            count ++;
            npass = server.arg(i);
        }
        if(server.argName(i) == "mqtt_server" && server.arg(i) != ""){
            count ++;
            nserver = server.arg(i);
        }
    }
    if(count != 3){
        server.send(200, "text/html", index_html_handle_config_wifi_error);
        return;
    }
    server.send(200, "text/html", index_html_handle_config_wifi);
    esid = nssid;
    epass = npass;
    sever = nserver;

    ECHOLN("clearing eeprom");
    for (int i = 0; i <= EEPROM_WIFI_SERVER_END; i++){ 
        EEPROM.write(i, 0); 
    }
    ECHOLN("writing register");
    EEPROM.write(EEPROM_WIFI_IS_REGISTER, 1);

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

}

void ConfigModeTemperature(){
    server.send(200, "text/html", index_html_handle_setup_temperature);
    for (uint8_t i = 0; i < server.args(); i++) {
        if(server.argName(i) == "setup_temperature" && server.arg(i) != ""){
            String temperature = server.arg(i);
            Value_Temperature = temperature.toInt();
            ECHO("Value_Temperature: ");
            ECHOLN(Value_Temperature);
            EEPROM.write(EEPROM_VALUE_TEMPERATURE, char(Value_Temperature));
            EEPROM.commit();
            break;
        }
    }
}

void configResetRegister(){
    server.sendHeader("Access-Control-Allow-Headers", "*");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "{\"status\":\"success\"}");
    ECHOLN("resetting register..."); 
    wifiIsRegister = false;
    EEPROM.write(EEPROM_WIFI_IS_REGISTER, 0);
    EEPROM.commit();
    ECHOLN("Done writing!");
}

void notFound(){
    server.sendHeader("Access-Control-Allow-Headers", "*");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "Not found");
}

bool testWifi(String esid, String epass) {
    ECHO("Connecting to: ");
    ECHOLN(esid);
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    server.close();
    // ECHO("delay: ");
    // ECHOLN((deviceId)*3000 + 1000);
    // for(int i = 0; i < 100; i++){
    //     if(digitalRead(PIN_CONFIG) == LOW){
    //         break;
    //     }
    //     delay(((deviceId)*3000 + 1000)/100);
    // }

    WiFi.mode(WIFI_STA);        //bat che do station
    WiFi.begin(esid.c_str(), epass.c_str());
    int c = 0;
    ECHOLN("Waiting for Wifi to connect");
    while (c < 40) {
        if (WiFi.status() == WL_CONNECTED) {
            ECHOLN("\rWifi connected!");
            ECHO("Local IP: ");
            ECHOLN(WiFi.localIP());
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


void checkButtonConfigClick(){
    if (digitalRead(PIN_CONFIG) == LOW && (ConfigTimeout + CONFIG_HOLD_TIME) <= millis()) { // Khi an nut
        ConfigTimeout = millis();
        //tickerSetMotor.attach(0.2, setLedApMode);  //every 0.2s
        Flag_Normal_Mode = false;
        tickerSetApMode.start();
        SetupConfigMode();
        StartConfigServer();
    } else if(digitalRead(PIN_CONFIG) == HIGH) {
        ConfigTimeout = millis();
    }
}

void SetupConfigMode(){
    ECHOLN("[WifiService][setupAP] Open AP....");
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    server.close();
    delay(500);
    WiFi.mode(WIFI_AP_STA);
    IPAddress APIP(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(APIP, gateway, subnet);
    String SSID_AP_MODE = SSID_PRE_AP_MODE + GetFullSSID();
    WiFi.softAP(SSID_AP_MODE.c_str(), PASSWORD_AP_MODE);
    ECHOLN(SSID_AP_MODE);

    ECHOLN("[WifiService][setupAP] Softap is running!");
    IPAddress myIP = WiFi.softAPIP();
    ECHO("[WifiService][setupAP] IP address: ");
    ECHOLN(myIP);
}


void StartConfigServer(){    
    // ECHOLN("[HttpServerH][startConfigServer] Begin create new server...");
    // server.on("/config", HTTP_POST, ConfigModeWifi);
    // server.on("/temperature", HTTP_POST, ConfigModeTemperature);
    // server.on("/resetRegister", HTTP_POST, configResetRegister);
    // server.begin();
    // ECHOLN("[HttpServerH][startConfigServer] HTTP server started");
    server.on("/", HTTP_GET, handleRoot);
	server.on("/config_wifi", HTTP_GET, ConfigModeWifi);
	server.on("/config_temperature", HTTP_GET, ConfigModeTemperature);
	server.on("/reset_register", HTTP_GET, configResetRegister);
	server.onNotFound(notFound);
	server.begin();
	Serial.println( "HTTP server started" );
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

    if (rootData.success()){
        //--------------getstatus-------------
        if(rootData["typedevice"] == m_Typedevice){
            int arraySize = rootData["deviceid"].size();   //get size of JSON Array
            int sensorValue[arraySize];
            bool isTrueControl = false;
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
    // Attempt to connect
    if (client.connect(clientId.c_str(), m_userNameServer, m_passSever)) {
        ECHO("connected with id: ");
        ECHOLN(clientId);
        digitalWrite(LED_TEST_AP, LOW);
        // Once connected, publish an announcement...
        // client.publish("outTopic", "hello world");
        // ... and resubscribe
        // String topicGetstatus = m_Pretopic + m_Getstatus;
        String topicControl = m_Pretopic + m_Control;

        client.subscribe(topicControl.c_str());
        ECHO("Done Subscribe Channel: ");
        // ECHO(topicGetstatus);
        // ECHO("  +  ");
        ECHOLN(topicControl);
        countDisconnectToServer++;
        if(flag_disconnect_to_sever == true){
            sum_time_disconnect_to_sever += millis() - count_time_disconnect_to_sever;
            flag_disconnect_to_sever = false;
        }
        getStatus();
    } 
    if(!client.connected()){
        ECHO("failed, rc=");
        ECHO(client.state());
        ECHOLN(" try again in 2 seconds");
    }
    return client.connected();
}


void tickerupdate(){
    tickerSetApMode.update();
    tickerBuzzer.update();
}

void setup(void)
{
    Serial.begin(115200);
    EEPROM.begin(512);
    pinMode(LED_TEST_AP, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_CONFIG, INPUT_PULLUP);
    pinMode(PIN_MODE_IN_OUT, INPUT_PULLUP);
    delay(10);
    digitalWrite(LED_TEST_AP, LOW);
    ECHOLN();
    if(char(EEPROM.read(EEPROM_WIFI_IS_REGISTER)) == 1){
        wifiIsRegister = true;
        digitalWrite(LED_TEST_AP, HIGH);
        SetupNetwork();     //khi hoat dong binh thuong
    }
    Value_Temperature = EEPROM.read(EEPROM_VALUE_TEMPERATURE);
    if(Value_Temperature < 10 || Value_Temperature > 70){
        Value_Temperature = 50;    //defaul is 50'C
    }
    ECHO("Value_Temperature: ");
    ECHOLN(Value_Temperature);

}

void loop(void)
{
    if(wifiIsRegister){
        if (Flag_Normal_Mode && WiFi.status() != WL_CONNECTED){
            digitalWrite(LED_TEST_AP, HIGH);
            testWifi(esid, epass);
        } 

        if(Flag_Normal_Mode && WiFi.status() == WL_CONNECTED && (millis() - Time) > TIME_RESEND){
            Time = millis();
            sensors.requestTemperatures();                // Send the command to get temperatures  
            ECHO("Temperature is: ");
            templateAfter = (int)sensors.getTempCByIndex(0);
            ECHOLN(templateAfter);   // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
        
            if(templateAfter != templateBefor){
                getStatus();
            }
            if(templateAfter >= Value_Temperature && couter_buzzer == 0){
                tickerBuzzer.start();
            }
            
            templateBefor = templateAfter;
        }
        

        if(Flag_Normal_Mode && WiFi.status() == WL_CONNECTED){
            if (!client.connected()) {
                if(flag_disconnect_to_sever == false){
                    count_time_disconnect_to_sever = millis();
                    lastReconnectAttempt = millis();
                    flag_disconnect_to_sever = true;
                }
                unsigned long nowReconnectAttempt = millis();

                if (nowReconnectAttempt - lastReconnectAttempt > 3000) {
                    lastReconnectAttempt = nowReconnectAttempt;
                    reconnect();
                }
            }else{
                unsigned long nowClientMqttLoop = millis();
                if (nowClientMqttLoop - lastClientMqttLoop > delay_mqtt_loop) {
                    lastClientMqttLoop = nowClientMqttLoop;
                    client.loop();
                }
            }
        }
    }
    else{
        if(millis() - Time >= TIME_RESEND){
            Time = millis();
            sensors.requestTemperatures();                // Send the command to get temperatures  
            templateAfter = (int)sensors.getTempCByIndex(0);
            ECHO("temperature is: ");
            ECHOLN(templateAfter);
            if(templateAfter >= Value_Temperature && couter_buzzer == 0){
                tickerBuzzer.start();
            }
        }
    }

    checkButtonConfigClick();
    tickerupdate();
    server.handleClient();
    
}
