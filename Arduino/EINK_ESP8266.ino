#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SmartEink.h>
#include <SPI.h>

E_ink Eink;

const char* ssid = "Edge";
const char* password = "keysightintel0809";

char Buf1[22];
char Buf2[22];
char Buf3[22];
char Buf4[22];

ESP8266WebServer server(8844);
void handletest(){
  if (server.hasArg("Text1")){
      server.arg("Text1").toCharArray(Buf1,22);
   } 
  if (server.hasArg("Text2")){
      server.arg("Text2").toCharArray(Buf2,22);
   } 
   if (server.hasArg("Text3")){
      server.arg("Text3").toCharArray(Buf3,22);
   } 
   if (server.hasArg("Text4")){
      server.arg("Text4").toCharArray(Buf4,22);
   } 
  /*************E_INK Display************/        
  Eink.InitEink();
  Eink.ClearScreen();// clear the screen
  Eink.EinkP8x16Str(14,8,Buf1);
  Eink.EinkP8x16Str(10,8,Buf2);
  Eink.EinkP8x16Str(6,8,Buf3);
  Eink.EinkP8x16Str(2,8,Buf4);
  Eink.RefreshScreen(); 
  /************************************/
  handleRoot();
      
}
void handleRoot() {
   String content = "<html><head><title>Espresso Lite V2.0+Eink Demo by Vincent Kok</title>";
  content +="<style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088;}";
  content +="</style></head>";
  content += "<body><h1>Espresso Lite V2.0+Eink Demo by Vincent Kok</h1><form action='/test' method='POST'>Please type:<br>";
  content += "<input type='text' name='Text1' placeholder='text'><br>";
  content += "<input type='text' name='Text2' placeholder='text'><br>";
  content += "<input type='text' name='Text3' placeholder='text'><br>";
  content += "<input type='text' name='Text4' placeholder='text'><br>";
  content += "<input type='submit' name='OK' value='Submit'></form><br>";
  server.send(200, "text/html", content);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
//This is setup code
void setup(void){
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
 pinMode(15,OUTPUT);
  digitalWrite(15, LOW);
  Serial.begin(115200);
  Serial.println("Booting Sketch...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

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

  server.on("/", handleRoot);
  server.on("/test", handletest);
  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}

