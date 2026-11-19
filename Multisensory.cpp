
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#define TRIG 3
#define ECHO 4
#define BUZZER 8
#define LDR A0

float temperature;
float distance;
int lightValue;

unsigned long startTime;

void setup() {
  Serial.begin(9600);

  dht.begin();

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUZZER, OUTPUT);

  startTime = millis();

  Serial.println("MULTI SENSOR DATA LOGGER");
  Serial.println("Time(s),Temp(C),Distance(cm),Light,Alert");
}

void loop() {

  // Read temperature
  temperature = dht.readTemperature();

  // Read distance
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000);
  distance = duration * 0.0343 / 2;

  // Read light
  lightValue = analogRead(LDR);

  // Threshold checking
  bool alert = false;

  if (!isnan(temperature) && temperature > 35)
    alert = true;

  if (duration > 0 && distance < 10)
    alert = true;

  if (lightValue < 200)
    alert = true;

  digitalWrite(BUZZER, alert ? HIGH : LOW);

  // UART data logging
  unsigned long timeSeconds =
      (millis() - startTime) / 1000;

  Serial.print(timeSeconds);
  Serial.print(",");
  if (isnan(temperature))
    Serial.print("ERROR");
  else
    Serial.print(temperature);

  Serial.print(",");
  if (duration == 0)
    Serial.print("OUT_OF_RANGE");
  else
    Serial.print(distance);

  Serial.print(",");
  Serial.print(lightValue);

  Serial.print(",");
  Serial.println(alert ? "ALERT" : "NORMAL");

  delay(1000);
}
