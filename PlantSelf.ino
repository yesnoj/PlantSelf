/*
 Desktop plant watering project
*/
#include "DHT.h"
 
 
#define echoPin D2 // Echo Pin white wire
#define trigPin D3 // Trigger Pin blue wire
 
const int relayPin = D1;
const int mositurePin = A0;
DHT dht(D4, DHT11);
 
const int moistureThreshold = 10;
const int waterLevelThreshold = 4;
 
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(relayPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  dht.begin();
  Serial.begin(9600);
}
 
// the loop function runs over and over again forever
void loop() {
   
  int moisture = 0;
  moisture = SoilMoisture();
 
  long waterLevel;
  waterLevel = WaterLevel();
 
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  float computeHeatIndex = dht.computeHeatIndex(temperature, humidity, false);
 
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  else{
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" Temperature: ");
    Serial.print(temperature);
    Serial.print(" *C ");
    Serial.print(" Heat index: ");
    Serial.print(computeHeatIndex);
    Serial.println("");
  }
 
   
  if (moisture < moistureThreshold && waterLevel > waterLevelThreshold)
  {
    digitalWrite(relayPin, HIGH);
    Serial.println("Pump on");
  }
  else
  {
    digitalWrite(relayPin, LOW);
    Serial.println("Pump off");
  }
  Serial.println("");
  delay(2000);                       // wait for a second
}
 
int SoilMoisture()
{
  int soilMositure = 0;
  int rawSoilMoisture = analogRead(mositurePin); 
  soilMositure = map(rawSoilMoisture, 550, 1024, 100, 0);
   
  Serial.print("Mositure: ");
  Serial.print(soilMositure);
  Serial.println("%");
  //Serial.print("% Raw value: ");  
  //Serial.println(rawSoilMoisture); 
 
  return soilMositure;
}
 
long WaterLevel()
{
  int maximumRange = 200; // Maximum range needed
  int minimumRange = 0; // Minimum range needed
  long duration, distance; // Duration used to calculate distance
  /* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
  
  if (distance >= maximumRange || distance <= minimumRange){
    Serial.println("Error: Out of range");
  }
  else {
    Serial.print("Water level: ");
    Serial.print(distance);
    Serial.println("cm");
  }
  return distance;
}
