#include "cong_tac_den_triac_8.h"

void getStatus(){
    //-------------------------------------------
    if(sub_device_id1){
        String datasend = "{\"room_id\":\"";
        for(int i = 0; i<roomId.length(); i++){
            if(roomId[i] == NULL){
                break;
            }
            datasend += roomId[i];
        }
        datasend += "\",\"device_id\":\"";
        for(int i = 0; i<deviceId.length(); i++){
            if(deviceId[i] == NULL){
                break;
            }
            datasend += deviceId[i];
        }
        datasend += "\",\"number_disconnect\":\"";
        datasend += String(countDisconnectToServer - 1);
        datasend += "\", \"all_time\":\"";
        datasend += String(sum_time_disconnect_to_sever);
        datasend +=  "\",\"sub_device_id\":\"1\",\"data\":\"";
        if(digitalRead(LIGHT_1) == HIGH){
            datasend += "on\"}";
        }else{
            datasend += "off\"}";
        }
        client.publish(topicsendStatus, datasend.c_str());
        sub_device_id1 = false;
    }
    

    //-------------------------------------------
    if(sub_device_id2){
        String datasend = "{\"room_id\":\"";
        for(int i = 0; i<roomId.length(); i++){
            if(roomId[i] == NULL){
                break;
            }
            datasend += roomId[i];
        }
        datasend += "\",\"device_id\":\"";
        for(int i = 0; i<deviceId.length(); i++){
            if(deviceId[i] == NULL){
                break;
            }
            datasend += deviceId[i];
        }
        datasend += "\",\"number_disconnect\":\"";
        datasend += String(countDisconnectToServer - 1);
        datasend += "\", \"all_time\":\"";
        datasend += String(sum_time_disconnect_to_sever);
        datasend +=  "\",\"sub_device_id\":\"2\",\"data\":\"";
        if(digitalRead(LIGHT_2) == HIGH){
            datasend += "on\"}";
        }else{
            datasend += "off\"}";
        }
        client.publish(topicsendStatus, datasend.c_str());
        sub_device_id2 = false;
    }
    

    //-------------------------------------------
    if(sub_device_id3){
        String datasend = "{\"room_id\":\"";
        for(int i = 0; i<roomId.length(); i++){
            if(roomId[i] == NULL){
                break;
            }
            datasend += roomId[i];
        }
        datasend += "\",\"device_id\":\"";
        for(int i = 0; i<deviceId.length(); i++){
            if(deviceId[i] == NULL){
                break;
            }
            datasend += deviceId[i];
        }
        datasend += "\",\"number_disconnect\":\"";
        datasend += String(countDisconnectToServer - 1);
        datasend += "\", \"all_time\":\"";
        datasend += String(sum_time_disconnect_to_sever);
        datasend +=  "\",\"sub_device_id\":\"3\",\"data\":\"";
        if(digitalRead(LIGHT_3) == HIGH){
            datasend += "on\"}";
        }else{
            datasend += "off\"}";
        }
        client.publish(topicsendStatus, datasend.c_str());
        sub_device_id3 = false;
    }

    //-------------------------------------------
    if(sub_device_id4){
        String datasend = "{\"room_id\":\"";
        for(int i = 0; i<roomId.length(); i++){
            if(roomId[i] == NULL){
                break;
            }
            datasend += roomId[i];
        }
        datasend += "\",\"device_id\":\"";
        for(int i = 0; i<deviceId.length(); i++){
            if(deviceId[i] == NULL){
                break;
            }
            datasend += deviceId[i];
        }
        datasend += "\",\"number_disconnect\":\"";
        datasend += String(countDisconnectToServer - 1);
        datasend += "\", \"all_time\":\"";
        datasend += String(sum_time_disconnect_to_sever);
        datasend +=  "\",\"sub_device_id\":\"4\",\"data\":\"";
        if(digitalRead(LIGHT_4) == HIGH){
            datasend += "on\"}";
        }else{
            datasend += "off\"}";
        }
        client.publish(topicsendStatus, datasend.c_str());
        sub_device_id4 = false;
    }
    //-------------------------------------------
    if(sub_device_id5){
        String datasend = "{\"room_id\":\"";
        for(int i = 0; i<roomId.length(); i++){
            if(roomId[i] == NULL){
                break;
            }
            datasend += roomId[i];
        }
        datasend += "\",\"device_id\":\"";
        for(int i = 0; i<deviceId.length(); i++){
            if(deviceId[i] == NULL){
                break;
            }
            datasend += deviceId[i];
        }
        datasend += "\",\"number_disconnect\":\"";
        datasend += String(countDisconnectToServer - 1);
        datasend += "\", \"all_time\":\"";
        datasend += String(sum_time_disconnect_to_sever);
        datasend +=  "\",\"sub_device_id\":\"5\",\"data\":\"";
        if(digitalRead(LIGHT_5) == HIGH){
            datasend += "on\"}";
        }else{
            datasend += "off\"}";
        }
        client.publish(topicsendStatus, datasend.c_str());
        sub_device_id5 = false;
    }
    //-------------------------------------------
    if(sub_device_id6){
        String datasend = "{\"room_id\":\"";
        for(int i = 0; i<roomId.length(); i++){
            if(roomId[i] == NULL){
                break;
            }
            datasend += roomId[i];
        }
        datasend += "\",\"device_id\":\"";
        for(int i = 0; i<deviceId.length(); i++){
            if(deviceId[i] == NULL){
                break;
            }
            datasend += deviceId[i];
        }
        datasend += "\",\"number_disconnect\":\"";
        datasend += String(countDisconnectToServer - 1);
        datasend += "\", \"all_time\":\"";
        datasend += String(sum_time_disconnect_to_sever);
        datasend +=  "\",\"sub_device_id\":\"6\",\"data\":\"";
        if(digitalRead(LIGHT_6) == HIGH){
            datasend += "on\"}";
        }else{
            datasend += "off\"}";
        }
        client.publish(topicsendStatus, datasend.c_str());
        sub_device_id6 = false;
    }
    //-------------------------------------------
    if(sub_device_id7){
        String datasend = "{\"room_id\":\"";
        for(int i = 0; i<roomId.length(); i++){
            if(roomId[i] == NULL){
                break;
            }
            datasend += roomId[i];
        }
        datasend += "\",\"device_id\":\"";
        for(int i = 0; i<deviceId.length(); i++){
            if(deviceId[i] == NULL){
                break;
            }
            datasend += deviceId[i];
        }
        datasend += "\",\"number_disconnect\":\"";
        datasend += String(countDisconnectToServer - 1);
        datasend += "\", \"all_time\":\"";
        datasend += String(sum_time_disconnect_to_sever);
        datasend +=  "\",\"sub_device_id\":\"7\",\"data\":\"";
        if(digitalRead(LIGHT_7) == HIGH){
            datasend += "on\"}";
        }else{
            datasend += "off\"}";
        }
        client.publish(topicsendStatus, datasend.c_str());
        sub_device_id7 = false;
    }
    //-------------------------------------------
    if(sub_device_id8){
        String datasend = "{\"room_id\":\"";
        for(int i = 0; i<roomId.length(); i++){
            if(roomId[i] == NULL){
                break;
            }
            datasend += roomId[i];
        }
        datasend += "\",\"device_id\":\"";
        for(int i = 0; i<deviceId.length(); i++){
            if(deviceId[i] == NULL){
                break;
            }
            datasend += deviceId[i];
        }
        datasend += "\",\"number_disconnect\":\"";
        datasend += String(countDisconnectToServer - 1);
        datasend += "\", \"all_time\":\"";
        datasend += String(sum_time_disconnect_to_sever);
        datasend +=  "\",\"sub_device_id\":\"8\",\"data\":\"";
        if(digitalRead(LIGHT_8) == HIGH){
            datasend += "on\"}";
        }else{
            datasend += "off\"}";
        }
        client.publish(topicsendStatus, datasend.c_str());
        sub_device_id8 = false;
    }
    

    ECHOLN("-------getStatus-------");
}


void setLedApMode() {
    digitalWrite(LED_TEST_AP, !digitalRead(LED_TEST_AP));
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



void ConfigMode(){
    StaticJsonBuffer<RESPONSE_LENGTH> jsonBuffer;
    ECHOLN(server.arg("plain"));
    JsonObject& rootData = jsonBuffer.parseObject(server.arg("plain"));
    ECHOLN("--------------");
    tickerSetApMode.stop();
    digitalWrite(LED_TEST_AP, HIGH);
    if (rootData.success()) {
        server.sendHeader("Access-Control-Allow-Headers", "*");
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "application/json; charset=utf-8", "{\"status\":\"success\"}");
        //server.stop();
        String nssid = rootData["ssid"];
        String npass = rootData["password"];
        String nid = rootData["device_id"];
        String nroom = rootData["room_id"];
        String nserver = rootData["server"];


        esid = nssid;
        epass = npass;
        deviceId = nid;
        roomId = nroom;
        sever = nserver;

        ECHOLN("clearing eeprom");
        for (int i = 0; i <= EEPROM_WIFI_SERVER_END; i++){
            EEPROM.write(i, NULL);
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
        for (int i = 0; i < nid.length(); ++i){
            EEPROM.write(i+EEPROM_WIFI_DEVICE_ID_START, nid[i]);
            ECHO(nid[i]);
        }
        ECHOLN("");

        ECHOLN("writing eeprom room id:"); 
        ECHO("Wrote: ");
        for (int i = 0; i < nroom.length(); ++i){
            EEPROM.write(i+EEPROM_WIFI_ROOM_ID_START, nroom[i]);
            ECHO(nroom[i]);
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
        return;
    }
    ECHOLN("Wrong data!!!");
}



bool testWifi(String esid, String epass) {
    ECHO("Connecting to: ");
    ECHOLN(esid);
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    server.close();
    // ECHO("delay: ");
    // ECHOLN((deviceId + 1)*3000 + 1000);
    // for(int i = 0; i < 100; i++){
    //     if(digitalRead(PIN_CONFIG) == LOW){
    //         break;
    //     }
    //     delay(((deviceId + 1)*3000 + 1000)/100);
    // }
    delay(1000);

    WiFi.mode(WIFI_STA);        //bat che do station
    WiFi.begin(esid.c_str(), epass.c_str());
    int c = 0;
    ECHOLN("Waiting for Wifi to connect");
    while (c < 40) {
        if (WiFi.status() == WL_CONNECTED) {
            ECHOLN("\rWifi connected!");
            ECHO("Local IP: ");
            ECHOLN(WiFi.localIP());
            // digitalWrite(LED_TEST_AP, LOW);
            ConnecttoMqttServer();
            return true;
        }
        if(digitalRead(PIN_CONFIG) == LOW){
            break;
        }
        //////////////////////////////////////read btn input
        if(flag_input_btn1 != digitalRead(BTN_INPUT_1)){
            delay(400);
            flag_input_btn1 = digitalRead(BTN_INPUT_1);
            count_bt1++;
            sub_device_id1 = true;
            flag_send_status = true;
            break;
        }
        if(flag_input_btn2 != digitalRead(BTN_INPUT_2)){
            delay(400);
            flag_input_btn2 = digitalRead(BTN_INPUT_2);
            count_bt2++;
            sub_device_id2 = true;
            flag_send_status = true;
            break;
        }
        if(flag_input_btn3 != digitalRead(BTN_INPUT_3)){
            delay(400);
            flag_input_btn3 = digitalRead(BTN_INPUT_3);
            count_bt3++;
            sub_device_id3 = true;
            flag_send_status = true;
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
    ECHOLN("[HttpServerH][startConfigServer] Begin create new server...");
    server.on("/config", HTTP_POST, ConfigMode);
    server.begin();
    ECHOLN("[HttpServerH][startConfigServer] HTTP server started");
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

    ECHOLN("Reading EEPROM Device Id");
    deviceId = "";
    for (int i = EEPROM_WIFI_DEVICE_ID_START; i < EEPROM_WIFI_DEVICE_ID_END; ++i){
        deviceId += char(EEPROM.read(i));
    }
    ECHO("DEVICE ID: ");
    ECHOLN(deviceId);

    ECHOLN("Reading EEPROM Room Id");
    roomId = "";
    for (int i = EEPROM_WIFI_ROOM_ID_START; i < EEPROM_WIFI_ROOM_ID_END; ++i){
        roomId += char(EEPROM.read(i));
    }
    ECHO("ROOM ID: ");
    ECHOLN(roomId);

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
        if(rootData["room_id"] == roomId){
            // int arraySize = rootData["deviceid"].size();   //get size of JSON Array
            // int sensorValue[arraySize];
            // bool isTrueControl = false;
            // for (int i = 0; i < arraySize; i++) { //Iterate through results
            //     sensorValue[i] = rootData["deviceid"][i];  //Implicit cast
            //     // ECHOLN(sensorValue[i]);
            //     if(sensorValue[i] == deviceId){
            //         isTrueControl = true;
            //         break;
            //     }
            // }
            String dataType = rootData["type_control"];
            //---------getstatus------------------
            if(dataType == "get_status"){
                sub_device_id1 = true;
                sub_device_id2 = true;
                sub_device_id3 = true;
                sub_device_id4 = true;
                sub_device_id5 = true;
                sub_device_id6 = true;
                sub_device_id7 = true;
                sub_device_id8 = true;
                getStatus();
                return;
            }
            if(rootData["device_id"] == deviceId){
                String Strsub_device_id = rootData["sub_device_id"];
                int sub_device_id = Strsub_device_id.toInt();
                //channel 1
                if(sub_device_id == 1){
                    String data = rootData["data"];
                    if(data == "on"){
                        if(digitalRead(LIGHT_1) == LOW){
                            count_bt1++;
                            sub_device_id1 = true;
                        }
                    }
                    else if(data == "off"){
                        if(digitalRead(LIGHT_1) == HIGH){
                            count_bt1++;
                            sub_device_id1 = true;
                        }
                    }
                }
                //channel 3
                else if(sub_device_id == 2){
                    String data = rootData["data"];
                    if(data == "on"){
                        if(digitalRead(LIGHT_2) == LOW){
                            count_bt2++;
                            sub_device_id2 = true;
                        }
                    }
                    else if(data == "off"){
                        if(digitalRead(LIGHT_2) == HIGH){
                            count_bt2++;
                            sub_device_id2 = true;
                        }
                    }
                }
                //channel 3
                else if(sub_device_id == 3){
                    String data = rootData["data"];
                    if(data == "on"){
                        if(digitalRead(LIGHT_3) == LOW){
                            count_bt3++;
                            sub_device_id3 = true;
                        }
                    }
                    else if(data == "off"){
                        if(digitalRead(LIGHT_3) == HIGH){
                            count_bt3++;
                            sub_device_id3 = true;
                        }
                    }
                }
                //channel 4
                else if(sub_device_id == 4){
                    String data = rootData["data"];
                    if(data == "on"){
                        if(digitalRead(LIGHT_4) == LOW){
                            count_bt4++;
                            sub_device_id4 = true;
                        }
                    }
                    else if(data == "off"){
                        if(digitalRead(LIGHT_4) == HIGH){
                            count_bt4++;
                            sub_device_id4 = true;
                        }
                    }
                }
                //channel 5
                else if(sub_device_id == 5){
                    String data = rootData["data"];
                    if(data == "on"){
                        if(digitalRead(LIGHT_5) == LOW){
                            count_bt5++;
                            sub_device_id5 = true;
                        }
                    }
                    else if(data == "off"){
                        if(digitalRead(LIGHT_5) == HIGH){
                            count_bt5++;
                            sub_device_id5 = true;
                        }
                    }
                }
                //channel 6
                else if(sub_device_id == 6){
                    String data = rootData["data"];
                    if(data == "on"){
                        if(digitalRead(LIGHT_6) == LOW){
                            count_bt6++;
                            sub_device_id6 = true;
                        }
                    }
                    else if(data == "off"){
                        if(digitalRead(LIGHT_6) == HIGH){
                            count_bt6++;
                            sub_device_id6 = true;
                        }
                    }
                }
                //channel 7
                else if(sub_device_id == 7){
                    String data = rootData["data"];
                    if(data == "on"){
                        if(digitalRead(LIGHT_7) == LOW){
                            count_bt7++;
                            sub_device_id7 = true;
                        }
                    }
                    else if(data == "off"){
                        if(digitalRead(LIGHT_7) == HIGH){
                            count_bt7++;
                            sub_device_id7 = true;
                        }
                    }
                }
                //channel 8
                else if(sub_device_id == 8){
                    String data = rootData["data"];
                    if(data == "on"){
                        if(digitalRead(LIGHT_8) == LOW){
                            count_bt8++;
                            sub_device_id8 = true;
                        }
                    }
                    else if(data == "off"){
                        if(digitalRead(LIGHT_8) == HIGH){
                            count_bt8++;
                            sub_device_id8 = true;
                        }
                    }
                }
                flag_send_status = true;
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
    const char* willTopic = "CabinetAvy/HPT/LWT";
    uint8_t willQos = 0;
    boolean willRetain = false;
    String willMessage = "{\"devicetype\" : \"";
    willMessage += m_Typedevice;
    willMessage += "\", \"deviceid\" : \"";
    willMessage += String(deviceId);
    willMessage += "\", \"status\" : \"error\"}";
    // Attempt to connect
    if (client.connect(clientId.c_str(), willTopic, willQos, willRetain, willMessage.c_str())) {
        ECHO("connected with id: ");
        ECHOLN(clientId);
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
        digitalWrite(LED_TEST_AP, LOW);
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



void tickerupdate(){
    tickerSetApMode.update();
}

void setup(void)
{
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
    Serial.begin(115200);
    EEPROM.begin(512);
    pinMode(LED_TEST_AP, OUTPUT);
    pinMode(PIN_CONFIG, INPUT_PULLUP);
    pinMode(LIGHT_1, OUTPUT);
    pinMode(LIGHT_2, OUTPUT);
    pinMode(LIGHT_3, OUTPUT);
    pinMode(LIGHT_4, OUTPUT);
    pinMode(LIGHT_5, OUTPUT);
    pinMode(LIGHT_6, OUTPUT);
    pinMode(LIGHT_7, OUTPUT);
    pinMode(LIGHT_8, OUTPUT);
    pinMode(BTN_INPUT_1, INPUT_PULLUP);
    pinMode(BTN_INPUT_2, INPUT_PULLUP);
    pinMode(BTN_INPUT_3, INPUT_PULLUP);
    pinMode(BTN_INPUT_4, INPUT_PULLUP);
    pinMode(BTN_INPUT_5, INPUT_PULLUP);
    pinMode(BTN_INPUT_6, INPUT_PULLUP);
    pinMode(BTN_INPUT_7, INPUT_PULLUP);
    pinMode(BTN_INPUT_8, INPUT);
    delay(100);
//    pinMode(PIN_GAS, INPUT_PULLUP);
    digitalWrite(LED_TEST_AP, HIGH);
    digitalWrite(LIGHT_1, HIGH);
    digitalWrite(LIGHT_2, HIGH);
    digitalWrite(LIGHT_3, HIGH);
    digitalWrite(LIGHT_4, HIGH);
    digitalWrite(LIGHT_5, HIGH);
    digitalWrite(LIGHT_6, HIGH);
    digitalWrite(LIGHT_7, HIGH);
    digitalWrite(LIGHT_8, HIGH);

    flag_input_btn1 = digitalRead(BTN_INPUT_1);
    flag_input_btn2 = digitalRead(BTN_INPUT_2);
    flag_input_btn3 = digitalRead(BTN_INPUT_3);
    flag_input_btn4 = digitalRead(BTN_INPUT_4);
    flag_input_btn5 = digitalRead(BTN_INPUT_5);
    flag_input_btn6 = digitalRead(BTN_INPUT_6);
    flag_input_btn7 = digitalRead(BTN_INPUT_7);
    flag_input_btn8 = digitalRead(BTN_INPUT_8);

    SetupNetwork();     //khi hoat dong binh thuong
}



void loop(void)
{
    if (Flag_Normal_Mode == true && WiFi.status() != WL_CONNECTED){
        //   digitalWrite(LED_TEST_AP, HIGH);
          testWifi(esid, epass);
    } 


    if(WiFi.status() == WL_CONNECTED){
        if (!client.connected()) {
            digitalWrite(LED_TEST_AP, HIGH);
            if(flag_disconnect_to_sever == false){
                count_time_disconnect_to_sever = millis();
                lastReconnectAttempt = millis();
                flag_disconnect_to_sever = true;
            }
            unsigned long nowReconnectAttempt = millis();

            if (abs(nowReconnectAttempt - lastReconnectAttempt) > 5000) {
                lastReconnectAttempt = nowReconnectAttempt;
                reconnect();
            }
        }else{
            client.loop();
        }
    }
    //////////////////////////////////////read btn input
    if(flag_input_btn1 != digitalRead(BTN_INPUT_1)){
        delay(400);
        flag_input_btn1 = digitalRead(BTN_INPUT_1);
        count_bt1++;
        sub_device_id1 = true;
        flag_send_status = true;
    }
    if(flag_input_btn2 != digitalRead(BTN_INPUT_2)){
        delay(400);
        flag_input_btn2 = digitalRead(BTN_INPUT_2);
        count_bt2++;
        sub_device_id2 = true;
        flag_send_status = true;
    }
    if(flag_input_btn3 != digitalRead(BTN_INPUT_3)){
        delay(400);
        flag_input_btn3 = digitalRead(BTN_INPUT_3);
        count_bt3++;
        sub_device_id3 = true;
        flag_send_status = true;
    }
    if(flag_input_btn4 != digitalRead(BTN_INPUT_4)){
        delay(400);
        flag_input_btn4 = digitalRead(BTN_INPUT_4);
        count_bt4++;
        sub_device_id4 = true;
        flag_send_status = true;
    }
    if(flag_input_btn5 != digitalRead(BTN_INPUT_5)){
        delay(400);
        flag_input_btn5 = digitalRead(BTN_INPUT_5);
        count_bt5++;
        sub_device_id5 = true;
        flag_send_status = true;
    }
    if(flag_input_btn6 != digitalRead(BTN_INPUT_6)){
        delay(400);
        flag_input_btn6 = digitalRead(BTN_INPUT_6);
        count_bt6++;
        sub_device_id6 = true;
        flag_send_status = true;
    }
    if(flag_input_btn7 != digitalRead(BTN_INPUT_7)){
        delay(400);
        flag_input_btn7 = digitalRead(BTN_INPUT_7);
        count_bt7++;
        sub_device_id7 = true;
        flag_send_status = true;
    }
    if(flag_input_btn8 != digitalRead(BTN_INPUT_8)){
        delay(400);
        flag_input_btn8 = digitalRead(BTN_INPUT_8);
        count_bt8++;
        sub_device_id8 = true;
        flag_send_status = true;
    }

    ///////////////////////////////////// control light
    if(count_bt1 % 2 == 1){
        digitalWrite(LIGHT_1, HIGH);
    }else{
        digitalWrite(LIGHT_1, LOW);
    }
    if(count_bt2 % 2 == 1){
        digitalWrite(LIGHT_2, HIGH);
    }else{
        digitalWrite(LIGHT_2, LOW);
    }
    if(count_bt3 % 2 == 1){
        digitalWrite(LIGHT_3, HIGH);
    }else{
        digitalWrite(LIGHT_3, LOW);
    }
    if(count_bt4 % 2 == 1){
        digitalWrite(LIGHT_4, HIGH);
    }else{
        digitalWrite(LIGHT_4, LOW);
    }
    if(count_bt5 % 2 == 1){
        digitalWrite(LIGHT_5, HIGH);
    }else{
        digitalWrite(LIGHT_5, LOW);
    }
    if(count_bt6 % 2 == 1){
        digitalWrite(LIGHT_6, HIGH);
    }else{
        digitalWrite(LIGHT_6, LOW);
    }
    if(count_bt7 % 2 == 1){
        digitalWrite(LIGHT_7, HIGH);
    }else{
        digitalWrite(LIGHT_7, LOW);
    }
    if(count_bt8 % 2 == 1){
        digitalWrite(LIGHT_8, HIGH);
    }else{
        digitalWrite(LIGHT_8, LOW);
    }
    if(flag_send_status){
        flag_send_status = false;
        getStatus();
    }
    ////////////////////////////////////////////////////
    checkButtonConfigClick();
    tickerupdate();
    server.handleClient();
    
}
