#include "webserver.h"

AsyncWebServer server(HTTP_PORT);
configweb config_network;

extern int Value_Temperature;

char index_html_handle_root_1[]  = 
	"<!DOCTYPE HTML><html><head>\n"
		"<title>ESP Input Form</title>\n"
		"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
		"</head><body>\n"
            "<h1>CONFIG SETUP</h1><br>\n"
			"<h3>VALUE FROM 1(MIN) TO 255(MAX)</h3><br>\n"
			"<form action=\"/setup_temperature\">\n"
				"<h3>MOTOR 1 UP</h3><br>\n"
				"<input type=\"text\" name=\"motor_1_up\">\n"
				"%d\n"
				"<input type=\"submit\" value=\"Save\"><br>\n"
			"</form><br>\n"
			"<form action=\"/config_wifi\">\n"
				"<h3>SETUP WIFI</h3><br>\n"
				"<h4>WIFI SSID</h4>\n"
				"<input type=\"text\" name=\"wifi_ssid\"><br>\n"
				"<h4>WIFI PASSWORD</h4>\n"
				"<input type=\"text\" name=\"wifi_password\"><br>\n"
				"<h4>MQTT SERVER</h4>\n"
				"<input type=\"text\" name=\"mqtt_server\"><br>\n"
				"<input type=\"submit\" value=\"Save\"><br>\n"
			"</form><br>\n"
			"<form action=\"/reset_register\">\n"
				"<h3>RESET REGISTER</h3>\n"
				"<input type=\"submit\" value=\"RESET\"><br>\n"
			"</form><br>\n"
	    "</body>\n"
    "</html>";
void handleRoot(AsyncWebServerRequest *request) {
	char index_html[2048];
	snprintf_P(index_html, sizeof(index_html), index_html_handle_root_1, Value_Temperature);
	request->send_P(200, "text/html", index_html);
}



//motor 1 up
const char index_html_handle_setup_temperature[] PROGMEM = R"rawliteral(
	<!DOCTYPE HTML><html><head>
		<title>ESP Input Form</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		</head><body>
		<center>
			<h1>DONE SETUP TEMPERATURE UP</h1><br>
			<br><a href=\><button>Back to Main</button></a><br>
		</center>
	</body></html>)rawliteral";
void handleSetupTemperature(AsyncWebServerRequest *request) {
	request->send_P(200, "text/html", index_html_handle_setup_temperature);
    if (request->hasParam("setup_temperature")){
		String setup_temperature = request->getParam("setup_temperature")->value();
		ECHOLN("setup_temperature: " + setup_temperature);
		if(setup_temperature != ""){
            Value_Temperature = setup_temperature.toInt();
            EEPROM.write(EEPROM_VALUE_TEMPERATURE, char(Value_Temperature));
            EEPROM.commit();
            ECHOLN("Done writing temperature!");
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
		String mqtt_server = request->getParam("mqtt_server")->value();
		ECHOLN("wifi_ssid: " + wifi_ssid);
		ECHOLN("wifi_password: " + wifi_password);
		ECHOLN("mqtt_server: " + mqtt_server);
		if(wifi_ssid != "" && wifi_password != "" && mqtt_server != ""){
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
const char index_html_handle_reset_register[] PROGMEM = R"rawliteral(
	<!DOCTYPE HTML><html><head>
		<title>ESP Input Form</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		</head><body>
		<center>
			<h1>DONE RESET REGISTER</h1><br>
			<br><a href=\><button>Back to Main</button></a><br>
		</center>
	</body></html>)rawliteral";
void handleResetWifi(AsyncWebServerRequest *request) {
	request->send_P(200, "text/html", index_html_handle_reset_register);
	EEPROM.write(EEPROM_WIFI_IS_REGISTER, 0);
	EEPROM.commit();
	ECHOLN("Done Reset Register!");
	config_network.is_reset_wifi = true;

}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
