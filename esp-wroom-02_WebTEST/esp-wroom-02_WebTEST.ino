#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "ESP-WROOM-02"; // APのSSIDを設定
const char *password = "12345678"; // APのパスワード設定

ESP8266WebServer server(80);

// ホームページデータを記載
String htmldata =
"<html>\
<head><title>WedTitle</title></head>\
<body>Hello World</body>\
</html>";

void setup() {
Serial.begin(115200);

// AP初期化処理
WiFi.softAP(ssid, password);
IPAddress myIP = WiFi.softAPIP();
Serial.print("AP IP address: ");
Serial.println(myIP);

// Webサーバ初期化処理
server.on("/", handleRoot);
server.begin();
Serial.println("HTTP server started");
}

void loop() {
server.handleClient();
}

void handleRoot() {
server.send(200, "text/html", htmldata);
}
