#include <Servo.h>

// Defines pins for Ultrasonic Sensor
const int trigPin = 10;
const int echoPin = 11;

// Variables for the duration and the distance
long duration;
int distance;

// Servo motor
Servo myServo;

void setup() {
  // Configurare pinii senzorului
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Pornirea comunicației seriale
  Serial.begin(9600);
  myServo.attach(12); // Atașarea servomotorului pe pinul 12
}

void loop() {
  // Rotește servomotorul între 15 și 165 grade
  for (int i = 15; i <= 165; i++) {  
    myServo.write(i);
    delay(30);

    distance = calculateDistance(); // Măsurare distanță
    Serial.print(i); // Sends the current degree into the Serial Port
    Serial.print(",");
    Serial.print(distance); // Trimite distanța pe Serial
    Serial.print("."); // Adaugă marcaj de sfârșit de mesaj
  }

  // Rotește servomotorul înapoi între 165 și 15 grade
  for (int i = 165; i > 15; i--) {  
    myServo.write(i);
    delay(30);

    distance = calculateDistance();
    Serial.print(i); // Sends the current degree into the Serial Port
    Serial.print(",");
    Serial.print(distance); // Trimite distanța pe Serial
    Serial.print("."); // Adaugă marcaj de sfârșit de mesaj
  }
}

// Funcție pentru calcularea distanței
int calculateDistance() { 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH); // Citește durata impulsului
  distance = duration * 0.034 / 2; // Calculare distanță în cm
  return distance;
}
