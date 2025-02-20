#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h> 

// Configurația Wi-Fi
const char* ssid = "";
const char* password = "";

// Pini pentru comunicarea serială cu Mega
#define RXp2 16
#define TXp2 17

// Server web
AsyncWebServer server(80);

String serialData = "";
int distance = 0;
int angle = 0; // Variabilă pentru unghi

void setup() {
  // Pornirea comunicației seriale
  Serial.begin(115200);
  Serial.println("ESP32 pornit...");

  // Configurare Serial2 pentru comunicarea cu Mega
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);

  // Setare mod STA (Station) pentru a evita crearea unei rețele suplimentare
  WiFi.mode(WIFI_STA);

  // Conectare la rețeaua Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectare la Wi-Fi...");
  }
  Serial.println("Conectat la Wi-Fi!");
  Serial.print("IP-ul dispozitivului: ");
  Serial.println(WiFi.localIP());

  // Configurare server web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String htmlContent = "<h1>Pagina mea web pe ESP32</h1>";
    htmlContent += "<p>Bine ai venit! ESP32 este conectat la Wi-Fi.</p>";
    htmlContent += "<p>IP-ul ESP32: " + WiFi.localIP().toString() + "</p>";
    htmlContent += "<p>Unghi: " + String(angle) + "°</p>";
    htmlContent += "<p>Distanta masurata: " + String(distance) + " cm</p>";  
    request->send(200, "text/html", htmlContent); // Trimite pagina HTML
  });

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
    DynamicJsonDocument doc(1024); 
    doc["angle"] = angle; // Adaugă unghiul în JSON
    doc["distance"] = distance; // Adaugă distanța în JSON

    String jsonResponse;
    serializeJson(doc, jsonResponse);

    request->send(200, "application/json", jsonResponse);
  });

  // Pornire server
  server.begin();
}

void loop() {
  // Citire date de la Arduino Mega prin Serial2
  while (Serial2.available()) {
    char c = Serial2.read();
    if (c == '.') { // Sfârșitul unui mesaj
      // Spargem datele în unghi și distanță
      int commaIndex = serialData.indexOf(',');
      if (commaIndex > 0) {
        angle = serialData.substring(0, commaIndex).toInt();
        distance = serialData.substring(commaIndex + 1).toInt();
        Serial.print("Unghi: ");
        Serial.print(angle);
        Serial.print("°, Distanță: ");
        Serial.println(distance);
      }
      serialData = ""; // Resetează buffer-ul
    } else {
      serialData += c; // Adaugă caractere la buffer
    }
  }

  delay(10); // Mică întârziere pentru a evita suprasarcina
}
