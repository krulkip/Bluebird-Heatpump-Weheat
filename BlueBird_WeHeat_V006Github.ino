#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>  // add this library to your project

const char* ssid = "...........";// Youir wifi ssid and password
const char* password = "..........";

// Server URL and endpoint
const char* host = "auth.weheat.nl";
const char* logshost = "api.weheat.nl";

const int httpsPort = 443;
const char* tokenURL = "/auth/realms/Weheat/protocol/openid-connect/token";
const char* urlFormat = "/api/v1/heat-pumps/%s/logs/latest";
                  
// Replace with your username and password
const char* username = "email used for weheat app logon";
const char* user_password = "password for weheat app logon";
const char* heatPumpId = "............................";//
const char* interval = "FiveMinute";  // Change to the required interval

WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected");

  // Verify fingerprint (optional, depends on your server's security setup)
  client.setInsecure(); // Use this only for testing; in production, use the server certificate or fingerprint

  // Connect to server
  Serial.print("Connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection failed!");
    return;
  }

  // Create the HTTP POST request
  String postData = "grant_type=password&client_id=WeheatCommunityAPI&scope=openid&username=";
  postData += username;
  postData += "&password=";
  postData += user_password;

  String request = String("POST ") + tokenURL + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Content-Type: application/x-www-form-urlencoded\r\n" +
                   "Accept: application/json\r\n" +
                   "Content-Length: " + postData.length() + "\r\n" +
                   "Connection: close\r\n\r\n" +
                   postData;

  // Send the request
  Serial.println(request);
  client.print(request);

  // Read the response
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Headers received");
      break;
    }
  }

  // Print the response
  String response = client.readString();
  //Serial.println("Response:");
  //Serial.println(response);
  // Parse the JSON response
  DynamicJsonDocument jsonDoc(3500);
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(jsonDoc, response);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  serializeJsonPretty(jsonDoc, Serial); Serial.println();
  String access_token = jsonDoc["access_token"];
  Serial.print("Access_token = ");
  Serial.println(access_token);
  Serial.println();
  
//*************************************************************************

  // Connect to the server
  Serial.print("Connecting to ");
  Serial.print(logshost);
  Serial.println("...");
  client.setInsecure(); // Use with caution, ideally verify the server certificate in production
  if (!client.connect(logshost, httpsPort)) {
    Serial.println("Connection failed!");
    return;
  }
  // Format the URL with the heat pump ID
  char logsURL[150];
  snprintf(logsURL, sizeof(logsURL), urlFormat, heatPumpId, interval);


  // Create the HTTP GET request
  String logsrequest = String("GET ") + logsURL + " HTTP/1.1\r\n" +
                   "Host: " + logshost + "\r\n" +
                   "Authorization: Bearer " + access_token + "\r\n" +
                   "Content-Type: application/json\r\n" +
                   "Connection: close\r\n\r\n";
  // Send the request
  Serial.println(logsrequest);
  while (client.read()>0);//clean out buffer
  {
  }
  
  client.print(logsrequest);

  // Wait for the response
  while (client.connected()) {
    String logsline = client.readStringUntil('\n');
    if (logsline == "\r") {
      Serial.println("Headers received");
      break;
    }
  }

  // Read and print the response
  String logsresponse = client.readString();
  //Serial.println("Response:");
  //Serial.println(logsresponse);
  String answer = logsresponse.substring(logsresponse.indexOf('{'), logsresponse.lastIndexOf('}')+1);
  Serial.println("Response2:");
  Serial.println(answer);
  //DynamicJsonDocument jsonDoc(3500);
  // Deserialize the JSON document
  DeserializationError error2 = deserializeJson(jsonDoc, answer);
  // Test if parsing succeeds.
  if (error2) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  serializeJsonPretty(jsonDoc, Serial); Serial.println();
  
}

void loop() {
  // Nothing to do here
}
