// send an (240 * 320) image as a binary file with 16 bit colour
// as a post request to ip_address/upload
// eg. 
//      $curl -F 'data=@img.bin' 192.168.1.142/upload
//

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

#ifndef STASSID
#define STASSID "BT-HKAJW6"
#define STAPSK  "QLptENUrT4MkpT"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int led = 13;

void handleStartWrite() {
  tft.startWrite();
  tft.setWindow(0, 0, tft.width(), tft.height()); // todo: this is probs default
  server.send(200, "text/plain", "started writing to lcd");
  
}
void handleEndWrite() {
  tft.endWrite();
  server.send(200, "text/plain", "stopped writing to lcd");
}

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void handleFileUpload(){ // upload a new file to the SPIFFS
  HTTPUpload& upload = server.upload();
  Serial.println("hello upload");
  Serial.println(upload.status);
  if(upload.status == UPLOAD_FILE_START){
    String filename = upload.filename;
    Serial.print("handleFileUpload Name: "); Serial.println(filename);
  } else if(upload.status == UPLOAD_FILE_WRITE){
    //Serial.println(upload.buf);
    tft.pushColors(upload.buf, upload.currentSize);
    tft.pushColors(upload.buf, upload.currentSize);
    tft.pushColors(upload.buf, upload.currentSize);
    Serial.println(upload.currentSize); // Write the received bytes to the file
  } else if(upload.status == UPLOAD_FILE_END){
      Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
      server.sendHeader("Location","/success.html");      // Redirect the client to the success page
      server.send(303);

  }
}

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  tft.init();
  tft.setRotation(3);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/startwrite", handleStartWrite);
  server.on("/endwrite", handleEndWrite);

  server.on("/upload", HTTP_POST,                       // if the client posts to the upload page
    [](){ server.send(200); },                          // Send status 200 (OK) to tell the client we are ready to receive
    handleFileUpload                                    // Receive and save the file
  );

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}


#define chunk_size 160
void loop(void) {
  //tft.startWrite();
  server.handleClient();
  MDNS.update();
  int wait_limit = 3 * 240 + 10;
  
  uint8_t col[chunk_size];
  for (uint8_t i = 0; i < chunk_size / 2; ++i){
    col[2*i] = i >> 8;
    col[2*i + 1] = i;
  }
  for (int px = 0; px < 240; px++){
    for (int chunk=0; chunk< (2 * tft.width()) / chunk_size; chunk++){
      //tft.pushColors(col, chunk_size);
    }
  }
  //tft.endWrite();
  //Serial.write("n");
}
