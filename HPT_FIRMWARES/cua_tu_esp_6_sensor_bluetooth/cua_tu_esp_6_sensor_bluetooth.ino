#include "cua_tu_esp_6_sensor_bluetooth.h"


void loadDataBegin(){
    

    //register
    if(char(EEPROM.read(EEPROM_IS_REGISTER) == 1)){
        isRegister = true;
        ECHOLN("isRegister = true");
    }
    else{
        isRegister = false;
        ECHOLN("isRegister = false");
    }

    //deviceid
    esid = "";
    for (int i = EEPROM_WIFI_DEVICE_ID_START; i < EEPROM_WIFI_DEVICE_ID_END; ++i){
        esid += char(EEPROM.read(i));
    }
    ECHO("SSID: ");
    ECHOLN(esid);

    //mode run
    setmoderunbegin = char(EEPROM.read(EEPROM_SET_MODE_RUN_BEGIN));
    setmoderunbeginchange = setmoderunbegin;
    // ECHOLN(setmoderunbegin);
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
        time_return = 50;
        ECHOLN("isSetTimeReurn fasle, auto set 500(ms)");
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

    
}

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {

    if(event == ESP_SPP_DATA_IND_EVT){
        if (param->data_ind.len < 512) {
            String data;
            for(int i = 0; i < param->data_ind.len; i++){
                data += (char)param->data_ind.data[i];
            }
            ECHO("data: ");
            ECHOLN(data);
            if(!Flag_Normal_Mode){
                StaticJsonBuffer<RESPONSE_LENGTH> jsonBuffer;
                JsonObject& rootData = jsonBuffer.parseObject(data);
                if (rootData.success()){
                    String deviceid = rootData["deviceid"];
                    esid = deviceid;
                    // deviceId = deviceIdStr.toInt();
                    // ECHOLN("clearing eeprom");
                    for (int i = EEPROM_WIFI_DEVICE_ID_START; i <= EEPROM_WIFI_DEVICE_ID_END; i++){ 
                        EEPROM.write(i, 0); 
                    }

                    for (int i = 0; i < esid.length(); ++i){
                        EEPROM.write(i+EEPROM_WIFI_DEVICE_ID_START, esid[i]);             
                        // ECHO(esid[i]);
                    }
                    EEPROM.write(EEPROM_IS_REGISTER, 1);
                    EEPROM.commit();

                    ECHO("Writed esid: ");
                    ECHOLN(esid);
                    ECHOLN("Registered!");
                    isRegister = true;
                    

                    // SetupNomalMode();
                }
            }else{
                StaticJsonBuffer<RESPONSE_LENGTH> jsonBuffer;
                JsonObject& rootData = jsonBuffer.parseObject(data);
                if (rootData.success()){
                    String dataType = rootData["typecontrol"];
                    //---------getstatus------------------
                    if(dataType == "getstatus"){
                        // 1 - Distant
                        // 2 - mode run
                        // 3 - time return
                        // 4 - percent low in
                        // 5 - percent low out
                        // 6 - delay button
                        // 7 - error button
                        String dataSend = "{\"1\":\"";
                        dataSend += (String)countPulDistant;
                        dataSend += "\",\"2\":\"";
                        dataSend += (String)setmoderunbeginchange;
                        dataSend += "\",\"3\":\"";
                        dataSend += (String)time_return;
                        dataSend += "\",\"4\":\"";
                        dataSend += (String)percentLowSpeedIn;
                        dataSend += "\",\"5\":\"";
                        dataSend += (String)percentLowSpeedOut;
                        dataSend += "\",\"6\":\"";
                        dataSend += (String)count_check_analog_pin;
                        dataSend += "\",\"7\":\"";
                        dataSend += (String)valueErrorAnalogRead;
                        dataSend += "\",\"8\":\"";
                        dataSend += (String)defineTimeAutoClose;
                        dataSend += "\"}";
                        SerialBT.write((uint8_t *)dataSend.c_str(), dataSend.length());
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
                    //---------control color------------------
                    else if(dataType == "controlled"){
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
                    //---------resetdistant------------------
                    else if(dataType == "resetdistant"){
                        resetDistant();
                    }
                    //---------cleardata------------------
                    else if(dataType == "cleardata"){
                        ClearData();
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


void resetDistant(){
    EEPROM.write(EEPROM_DISTANT, 0);
    EEPROM.commit();
    countPulFGDistant = 0;
    isSaveDistant = false;
    fristRun = true;
    countFrirstRun = 0;
    ECHOLN("resetDistant");
}

void ClearData(){
    for(int i = 0; i < EEPROM_WIFI_MAX_CLEAR; i++){
        EEPROM.write(i, 0);
    }
    EEPROM.commit();
    countPulFGDistant = 0;
    isSaveDistant = false;
    fristRun = true;
    countFrirstRun = 0;
    ECHOLN("ClearData");
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


    ECHO("van toc: ");
    ECHOLN(speed);
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
    if(count_stop_motor % 2 == 0){
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
            flag_send_status_when_use_hand = false;
        }
        else if(flag_send_status_when_use_hand == true && Forward == false){    //dang o trang thai mo cua
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


void SetupNomalMode(){
    // ECHOLN("An error occurred initializing Bluetooth");
    SerialBT.flush();
    SerialBT.end(); 
    delay(10);
    tickerSetApMode.stop();
    Flag_Normal_Mode = true;
    String NameConfig = "Avy-" + esid;
    // ECHOLN("An error occurred initializing Bluetooth");
    if (!SerialBT.begin(NameConfig)) {
        ECHOLN("An error occurred initializing Bluetooth");
        digitalWrite(ledTestWifi, LOW);
    } else {
        ECHO("Bluetooth initialized: ");
        ECHOLN(NameConfig);
        digitalWrite(ledTestWifi, HIGH);
    }
    SerialBT.register_callback(callback);
}

void SetupConfigMode(){
    SerialBT.flush();
    SerialBT.end(); 
    delay(10);
    Flag_Normal_Mode = false;
    isRegister = false;
    if (!SerialBT.begin(GetFullSSID())) {
        ECHOLN("An error occurred initializing Bluetooth");
        digitalWrite(ledTestWifi, LOW);
    } else {
        ECHO("Bluetooth initialized: ");
        ECHOLN(GetFullSSID().c_str());
        tickerSetApMode.start();
    }
    SerialBT.register_callback(callback);
}



String GetFullSSID() {
    uint8_t mac[WL_MAC_ADDR_LENGTH];
    String macID;
    macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) + String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
    macID.toUpperCase();
    // return SSID_PRE_AP_MODE + macID;
    return SSID_PRE_AP_MODE;
}


void ConfigMode(){
    // StaticJsonBuffer<RESPONSE_LENGTH> jsonBuffer;
    // ECHOLN(server.arg("plain"));
    // JsonObject& rootData = jsonBuffer.parseObject(server.arg("plain"));
    // ECHOLN("--------------");
    // tickerSetApMode.stop();
    // digitalWrite(ledTestWifi, LOW);
    // if (rootData.success()) {
    //     server.sendHeader("Access-Control-Allow-Headers", "*");
    //     server.sendHeader("Access-Control-Allow-Origin", "*");
    //     server.send(200, "application/json; charset=utf-8", "{\"status\":\"success\"}");
    //     //server.stop();
    //     String nssid = rootData["ssid"];
    //     String npass = rootData["password"];
    //     String nid = rootData["deviceid"];
    //     String nserver = rootData["server"];


    //     return;
    // }
    // ECHOLN("Wrong data!!!");
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

    if(!isRegister){
        SetupConfigMode();
    }else{
        SetupNomalMode();
    }

    


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

    if(!Flag_Normal_Mode && isRegister){
        SetupNomalMode();
    }

    if(Flag_Normal_Mode){
        if(Forward == false && statusLed == true && statusStop == false  && countPulFGDistant <= 10){
            statusLed = false;
            tickerSetPwmLedLightOn.stop();
            tickerSetPwmLedLightOff.start();
        }
        checkAnalogReadButton();
        setSpeedControl();

        if(defineTimeAutoClose != 0){
            checkAutoClose();
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
    //hold to config mode
    if(digitalRead(PIN_CONFIG) == HIGH){
        time_click_button = millis();
    }
    if(digitalRead(PIN_CONFIG) == LOW && (time_click_button + CONFIG_HOLD_TIME) <= millis()){
        time_click_button = millis();
        Flag_Normal_Mode = false;
        ClearData();
        SetupConfigMode();
    }
    tickerupdate();
    


}
