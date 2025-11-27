#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define WATER_PIN A0

#define RED_LED 8 
#define BLUE_LED 10
#define PUMP 9
#define COOLER 11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(COOLER, OUTPUT);

  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(PUMP, LOW);
  digitalWrite(COOLER, LOW);
}

void loop() {
  float temperature = dht.readTemperature();
  int waterLevel = analogRead(WATER_PIN);

  // ---- SET THESE ----
  int waterThreshold = 767;   //.75*1024(it's range)=767
  float coldTemp = 25.0;      // temp threshold for cooling
  // --------------------

  // Print diagnostics
  Serial.print("Water Level: ");
  Serial.print(waterLevel);
  Serial.print("  |  Temp: ");
  Serial.print(temperature);
  Serial.print(" C  |  Threshold: ");
  Serial.print(waterThreshold);

  // decide water state
  bool isFull = (waterLevel >= waterThreshold); //if true -> yes, is full
  Serial.print("  |  isFull? ");
  Serial.print(isFull ? "YES" : "NO");

  // Water/pump logic
  if (!isFull) {
    digitalWrite(RED_LED, HIGH);   // show empty
    digitalWrite(PUMP, HIGH);      // try to turn pump ON
    Serial.print("  |  Pump: ON");
  } else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(PUMP, LOW);
    Serial.print("  |  Pump: OFF");
  }

  // Cooling logic (if already exist and it not cold)
  if (!isnan(temperature) && temperature > coldTemp) {
    digitalWrite(BLUE_LED, HIGH);
    digitalWrite(COOLER, HIGH);
    Serial.print("  |  Cooler: ON");
  } else {
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(COOLER, LOW);
    Serial.print("  |  Cooler: OFF");
  }

  Serial.println();
  delay(1000);
}
