#include "webserver.h"

AsyncWebServer server(HTTP_PORT);
configweb config_network;

extern int Max_L1_UP;
extern int Max_L1_DOWN;
extern int Max_L2_UP;
extern int Max_L2_DOWN;

// char index_html_handle_root_1[]  = {R"rawliteral(
// 	<!DOCTYPE HTML><html><head>
// 		<title>ESP Input Form</title>
// 		<meta name="viewport" content="width=device-width, initial-scale=1">
// 		</head><body>
//             <h1>CONFIG SETUP</h1><br>
// 			<h3>VALUE FROM 1(MIN) TO 255(MAX)</h3><br>
// 			<form action="/motor_1_up">
// 				<h3>MOTOR 1 UP</h3><br>
// 				<input type="text" name="motor_1_up">
// 				<input type="submit" value="Save"><br>
// 			</form><br>
//             <form action="/motor_2_up">
// 				<h3>MOTOR 2 UP</h3><br>
// 				<input type="text" name="motor_2_up">
// 				<input type="submit" value="Save"><br>
// 			</form><br>
//             <form action="/motor_1_down">
// 				<h3>MOTOR 1 DOWN</h3><br>
// 				<input type="text" name="motor_1_down">
// 				<input type="submit" value="Save"><br>
// 			</form><br>
//             <form action="/motor_2_down">
// 				<h3>MOTOR 2 DOWN</h3>
// 				<input type="text" name="motor_2_down">
// 				<input type="submit" value="Save"><br>
// 			</form><br>
// 			<form action="/config_wifi">
// 				<h3>SETUP WIFI</h3><br>
// 				<h4>WIFI SSID</h4>
// 				<input type="text" name="wifi_ssid"><br>
// 				<h4>WIFI PASSWORD</h4>
// 				<input type="text" name="wifi_password"><br>
// 				<h4>DEVICE ID</h4>
// 				<input type="text" name="device_id"><br>
// 				<h4>MQTT SERVER</h4>
// 				<input type="text" name="mqtt_server"><br>
// 				<input type="submit" value="Save"><br>
// 			</form><br>)rawliteral"};

char index_html_handle_root_1[]  = 
	"<!DOCTYPE HTML><html><head>\n"
		"<title>ESP Input Form</title>\n"
		"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
		"</head><body>\n"
            "<h1>CONFIG SETUP</h1><br>\n"
			"<h3>VALUE FROM 1(MIN) TO 255(MAX)</h3><br>\n"
			"<form action=\"/motor_1_up\">\n"
				"<h3>MOTOR 1 UP</h3><br>\n"
				"<input type=\"text\" name=\"motor_1_up\">\n"
				"%d\n"
				"<input type=\"submit\" value=\"Save\"><br>\n"
			"</form><br>\n"
            "<form action=\"/motor_2_up\">\n"
				"<h3>MOTOR 2 UP</h3><br>\n"
				"<input type=\"text\" name=\"motor_2_up\">\n"
				"%d\n"
				"<input type=\"submit\" value=\"Save\"><br>\n"
			"</form><br>\n"
            "<form action=\"/motor_1_down\">\n"
				"<h3>MOTOR 1 DOWN</h3><br>\n"
				"<input type=\"text\" name=\"motor_1_down\">\n"
				"%d\n"
				"<input type=\"submit\" value=\"Save\"><br>\n"
			"</form><br>\n"
            "<form action=\"/motor_2_down\">\n"
				"<h3>MOTOR 2 DOWN</h3>\n"
				"<input type=\"text\" name=\"motor_2_down\">\n"
				"%d\n"
				"<input type=\"submit\" value=\"Save\"><br>\n"
			"</form><br>\n"
			"<form action=\"/config_wifi\">\n"
				"<h3>SETUP WIFI</h3><br>\n"
				"<h4>WIFI SSID</h4>\n"
				"<input type=\"text\" name=\"wifi_ssid\"><br>\n"
				"<h4>WIFI PASSWORD</h4>\n"
				"<input type=\"text\" name=\"wifi_password\"><br>\n"
				"<h4>DEVICE ID</h4>\n"
				"<input type=\"text\" name=\"device_id\"><br>\n"
				"<h4>MQTT SERVER</h4>\n"
				"<input type=\"text\" name=\"mqtt_server\"><br>\n"
				"<input type=\"submit\" value=\"Save\"><br>\n"
			"</form><br>\n"
			"<form action=\"/reset_wifi\">\n"
				"<h3>RESET WIFI</h3>\n"
				"<input type=\"submit\" value=\"RESET\"><br>\n"
			"</form><br>\n"
	    "</body>\n"
    "</html>";

// char index_html_handle_root_2[] = {R"rawliteral(
// 			<form action="/reset_wifi">
// 				<h3>RESET WIFI</h3>
// 				<input type="submit" value="RESET"><br>
// 			</form><br>
// 	    </body>
//     </html>)rawliteral"};



// char index_html[] = {
// "<!DOCTYPE html>\n"
// "<html>\n"
// "<body>\n"
// "<body style=\"background-color:black; color:cyan;; font-family:verdana\">\n"
// "\n"
// "\n"
// "<p style=\"font-size:500%; text-align:center\">\n"
// "<I><b>%.1f<sup><small><small>F</p>\n"
// "\n"
// "\n"
// "</body>\n"
// "</html>"};



void handleRoot(AsyncWebServerRequest *request) {
	char index_html[2048];
	//index_html += index_html_handle_root_1;
	// strcat(index_html,index_html_handle_root_1);
	// strcat(index_html,index_html_handle_root_2);
	// char html[256];
	// float dhttemp = 25.0;
	
	snprintf_P(index_html, sizeof(index_html), index_html_handle_root_1, Max_L1_UP, Max_L2_UP, Max_L1_DOWN, Max_L2_DOWN);
	request->send_P(200, "text/html", index_html);
	// request->send_P(200, "text/html", html, processor);
	// request->send_P(200, "text/html", index_html_handle_root_2);
	
}



//motor 1 up
const char index_html_handle_motor_1_up[] PROGMEM = R"rawliteral(
	<!DOCTYPE HTML><html><head>
		<title>ESP Input Form</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		</head><body>
		<center>
			<h1>DONE SETUP MOTOR 1 UP</h1><br>
			<br><a href=\><button>Back to Main</button></a><br>
		</center>
	</body></html>)rawliteral";
void handleMotor1Up(AsyncWebServerRequest *request) {
	request->send_P(200, "text/html", index_html_handle_motor_1_up);
    if (request->hasParam("motor_1_up")){
		String motor_1_up = request->getParam("motor_1_up")->value();
		ECHOLN("motor_1_up: " + motor_1_up);
		if(motor_1_up != ""){
            int motor_1_up_int = motor_1_up.toInt();
            EEPROM.write(EEPROM_MOTOR_1_UP, char(motor_1_up_int));
            EEPROM.commit();
            ECHOLN("Done writing motor 1 up!");
            config_network.is_receive_data = true;
		}
	}
}

//motor 2 up
const char index_html_handle_motor_2_up[] PROGMEM = R"rawliteral(
	<!DOCTYPE HTML><html><head>
		<title>ESP Input Form</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		</head><body>
		<center>
			<h1>DONE SETUP MOTOR 2 UP</h1><br>
			<br><a href=\><button>Back to Main</button></a><br>
		</center>
	</body></html>)rawliteral";
void handleMotor2Up(AsyncWebServerRequest *request) {
	request->send_P(200, "text/html", index_html_handle_motor_2_up);
    if (request->hasParam("motor_2_up")){
		String motor_2_up = request->getParam("motor_2_up")->value();
		ECHOLN("motor_2_up: " + motor_2_up);
		if(motor_2_up != ""){
            int motor_2_up_int = motor_2_up.toInt();
            EEPROM.write(EEPROM_MOTOR_2_UP, char(motor_2_up_int));
            EEPROM.commit();
            ECHOLN("Done writing motor 2 up!");
            config_network.is_receive_data = true;
		}
	}
}

//motor 1 down
const char index_html_handle_motor_1_down[] PROGMEM = R"rawliteral(
	<!DOCTYPE HTML><html><head>
		<title>ESP Input Form</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		</head><body>
		<center>
			<h1>DONE SETUP MOTOR 1 DOWN</h1><br>
			<br><a href=\><button>Back to Main</button></a><br>
		</center>
	</body></html>)rawliteral";
void handleMotor1Down(AsyncWebServerRequest *request) {
	request->send_P(200, "text/html", index_html_handle_motor_1_down);
    if (request->hasParam("motor_1_down")){
		String motor_1_down = request->getParam("motor_1_down")->value();
		ECHOLN("motor_1_down: " + motor_1_down);
		if(motor_1_down != ""){
            int motor_1_down_int = motor_1_down.toInt();
            EEPROM.write(EEPROM_MOTOR_1_DOWN, char(motor_1_down_int));
            EEPROM.commit();
            ECHOLN("Done writing motor 1 down!");
            config_network.is_receive_data = true;
		}
	}
}

//motor 2 down
const char index_html_handle_motor_2_down[] PROGMEM = R"rawliteral(
	<!DOCTYPE HTML><html><head>
		<title>ESP Input Form</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		</head><body>
		<center>
			<h1>DONE SETUP MOTOR 2 DOWN</h1><br>
			<br><a href=\><button>Back to Main</button></a><br>
		</center>
	</body></html>)rawliteral";
void handleMotor2Down(AsyncWebServerRequest *request) {
	request->send_P(200, "text/html", index_html_handle_motor_2_down);
    if (request->hasParam("motor_2_down")){
		String motor_2_down = request->getParam("motor_2_down")->value();
		ECHOLN("motor_2_down: " + motor_2_down);
		if(motor_2_down != ""){
            int motor_2_down_int = motor_2_down.toInt();
            EEPROM.write(EEPROM_MOTOR_2_DOWN, char(motor_2_down_int));
            EEPROM.commit();
            ECHOLN("Done writing motor 2 down!");
            config_network.is_receive_data = true;
		}
	}
}


//setup wifi
const char index_html_handle_config_wifi[] PROGMEM = R"rawliteral(
	<!DOCTYPE HTML><html><head>
		<title>ESP Input Form</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		</head><body>
		<center>
			<h1>DONE SETUP WIFI</h1><br>
			<br><a href=\><button>Back to Main</button></a><br>
		</center>
	</body></html>)rawliteral";
void handleConfigWifi(AsyncWebServerRequest *request) {
	request->send_P(200, "text/html", index_html_handle_config_wifi);
    if (request->hasParam("wifi_ssid")){
		String wifi_ssid = request->getParam("wifi_ssid")->value();
		String wifi_password = request->getParam("wifi_password")->value();
		String device_id = request->getParam("device_id")->value();
		String mqtt_server = request->getParam("mqtt_server")->value();
		ECHOLN("wifi_ssid: " + wifi_ssid);
		ECHOLN("wifi_password: " + wifi_password);
		ECHOLN("device_id: " + device_id);
		ECHOLN("mqtt_server: " + mqtt_server);
		int device_id_int = device_id.toInt();
		if(wifi_ssid != "" && wifi_password != "" && device_id != "" && mqtt_server != ""){
			EEPROM.write(EEPROM_WIFI_IS_REGISTER, 1);

			ECHOLN("clearing eeprom");
			for (int i = 0; i <= EEPROM_WIFI_SERVER_END; i++){ 
				EEPROM.write(i, 0); 
			}
			ECHOLN("writing eeprom ssid:");
			ECHO("Wrote: ");
			for (int i = 0; i < wifi_ssid.length(); ++i){
				EEPROM.write(i+EEPROM_WIFI_SSID_START, wifi_ssid[i]);             
				ECHO(wifi_ssid[i]);
			}
			ECHOLN("");
			ECHOLN("writing eeprom pass:"); 
			ECHO("Wrote: ");
			for (int i = 0; i < wifi_password.length(); ++i){
				EEPROM.write(i+EEPROM_WIFI_PASS_START, wifi_password[i]);
				ECHO(wifi_password[i]);
			}
			ECHOLN("");
			ECHOLN("writing eeprom device id:"); 
			ECHO("Wrote: ");
			EEPROM.write(EEPROM_WIFI_DEVICE_ID, device_id_int);
			ECHOLN(device_id_int);

			ECHOLN("writing eeprom server:");
			ECHO("Wrote: ");
			for (int i = 0; i < mqtt_server.length(); ++i){
				EEPROM.write(i+EEPROM_WIFI_SERVER_START, mqtt_server[i]);
				ECHO(mqtt_server[i]);
			}
			ECHOLN("");

			EEPROM.commit();
			ECHOLN("Done writing!");
			config_network.is_receive_wifi = true;

		}
	}
}

//reset wifi
const char index_html_handle_reset_wifi[] PROGMEM = R"rawliteral(
	<!DOCTYPE HTML><html><head>
		<title>ESP Input Form</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		</head><body>
		<center>
			<h1>DONE RESET WIFI</h1><br>
			<br><a href=\><button>Back to Main</button></a><br>
		</center>
	</body></html>)rawliteral";
void handleResetWifi(AsyncWebServerRequest *request) {
	request->send_P(200, "text/html", index_html_handle_reset_wifi);
	EEPROM.write(EEPROM_WIFI_IS_REGISTER, 0);
	EEPROM.commit();
	ECHOLN("Done Reset!");
	config_network.is_reset_wifi = true;

}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
