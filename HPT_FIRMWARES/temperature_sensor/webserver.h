#ifndef webserver_h
#define webserver_h


char index_html_handle_root_1[]  = 
	"<!DOCTYPE HTML><html><head>\n"
		"<title>ESP Input Form</title>\n"
		"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
		"</head><body>\n"
            "<h1>CONFIG SETUP</h1><br>\n"
			"<h3>VALUE FROM 1(MIN) TO 255(MAX)</h3><br>\n"
			"<form action=\"/config_temperature\">\n"
				"<h3>MOTOR 1 UP</h3><br>\n"
				"<input type=\"text\" name=\"setup_temperature\">\n"
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



//TEMPERATURE
const char index_html_handle_setup_temperature[] PROGMEM = R"rawliteral(
	<!DOCTYPE HTML><html><head>
		<title>ESP Input Form</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		</head><body>
		<center>
			<h1>DONE SETUP TEMPERATURE</h1><br>
			<br><a href=\><button>Back to Main</button></a><br>
		</center>
	</body></html>)rawliteral";

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

//setup wifi
const char index_html_handle_config_wifi_error[] PROGMEM = R"rawliteral(
	<!DOCTYPE HTML><html><head>
		<title>ESP Input Form</title>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		</head><body>
		<center>
			<h1>NOT DONE, FILL ALL CHARACTERS</h1><br>
			<br><a href=\><button>Back to Main</button></a><br>
		</center>
	</body></html>)rawliteral";

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


#endif
