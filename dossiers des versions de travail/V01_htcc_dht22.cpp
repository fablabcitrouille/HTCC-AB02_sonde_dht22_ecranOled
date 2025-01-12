/*
 * Ce programme utilise un module Heltec CubeCell HTCC-AB02 pour lire la température et l'humidité d'un capteur DHT22 et envoyer ces données sous forme de trame hexadécimale.
 * La trame de données est envoyée via la communication série et contient les valeurs de température et d'humidité en hexadécimal.
 * 
 * Démarche :
 * 1. Connexion au capteur DHT22 : Le capteur DHT22 est connecté à la broche GPIO5 (D1) et alimenté par la broche VEXT.
 * 2. Lecture des données du capteur DHT22 : La température et l'humidité sont lues toutes les 20 secondes.
 * 3. Création de la trame de données : Les valeurs de température et d'humidité sont converties en hexadécimal et concaténées pour former la trame de données.
 * 4. Envoi de la trame de données : La trame de données est envoyée via la communication série et imprimée pour le débogage.
 * 5. Affichage des données : Les valeurs de température et d'humidité sont affichées sur un écran OLED et sur la sortie série terminal.
 * 6. Gestion de l'alimentation : La broche VEXT est utilisée pour activer et désactiver l'alimentation du capteur DHT22 afin de réduire la consommation d'énergie.
 * 7. La trame préparée pourra être ensuite envoyée sur le serveur LORAWAN TTN et fera l'objet d'un autre code.
 * 8. La librairie du HTCC-AB02 permet de déclarer Vext sans spésifier le GPIO.
 * 9. La mesure température et humidité, laisser un délai de 1 seconde entre les deux mesures.
 * 
 * 
 * Bibliothèques utilisées :
 * - DHT sensor library : Pour lire les mesures du capteur DHT22.
 * - Adafruit Unified Sensor : Bibliothèque de capteurs unifiés d'Adafruit.
 * - HT_SH1107Wire : Pour gérer l'affichage OLED.
 * 
 * Auteur : [Votre Nom]
 * Date : [Date]
 */
#include <Arduino.h>

#include <DHT.h>
#include <Adafruit_Sensor.h>

#include "HT_SH1107Wire.h"
#include <Wire.h>

// Initialisation de l'affichage
SH1107Wire display(0x3c, 500000, SDA, SCL, GEOMETRY_128_64, GPIO10); // addr, freq, sda, scl, resolution, rst

// Définir les broches et le type de capteur DHT
#define DHTPIN GPIO5     // GPIO5 (D1) broche à laquelle le capteur DHT est connecté
#define DHTTYPE DHT22    // Définir le type de capteur DHT
#define BATTERY_PIN ADC1 // Broche ADC1 pour la détection de la puissance de la batterie

// Créer une instance du capteur DHT
DHT dht(DHTPIN, DHTTYPE);

// Déclaration des fonctions
void setup();
void loop();
void readDHT22(float &temperature, float &humidity);
String createDataFrame(float temperature, float humidity);
void sendDataFrame(const String &dataFrame);
float readBatteryVoltage();
void VextON(void);
void VextOFF(void);
void displayData(float temperature, float humidity);

void VextON(void) {
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);//pour activer l'alim Vext
}

void VextOFF(void) { // Vext default OFF
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, HIGH);
}

void setup() {
  // Initialiser la communication série
  Serial.begin(9600);

  // Configurer la broche VEXT pour alimenter le capteur
  VextON();
  delay(100);

  // Initialiser le capteur DHT
  dht.begin();

  // Initialiser l'affichage
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.clear();
  display.display();
}

void loop() {
  float temperature = 0.0;
  float humidity = 0.0;

  // Lire les valeurs de température et d'humidité du capteur DHT22
  readDHT22(temperature, humidity);


  // Créer une trame de données
  String dataFrame = createDataFrame(temperature, humidity);

  // Envoyer la trame de données
  sendDataFrame(dataFrame);

  // Afficher les données sur l'écran OLED et sur la sortie série terminal
  displayData(temperature, humidity);

  // Attendre avant de lire à nouveau
  delay(20000);
}

void readDHT22(float &temperature, float &humidity) {

  VextON();
  delay(1000);

  
  // Lire la température et l'humidité du capteur DHT22
  temperature = dht.readTemperature();
  delay(1000);//délai semble nécessaire entre les mesures temp et hum
  humidity = dht.readHumidity();

  // Vérifier si les lectures ont échoué et réessayer
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    temperature = 0.0;
    humidity = 0.0;
  }

  VextOFF();
}

String createDataFrame(float temperature, float humidity) {
  // Créer une trame de données au format hexadécimal
  String dataFrame = "";

  // Convertir chaque valeur en hexadécimal et les ajouter à la trame
  dataFrame += String((int)(temperature * 100), HEX); // Multiplier par 100 pour conserver deux décimales
  dataFrame += ",";
  dataFrame += String((int)(humidity * 100), HEX); // Multiplier par 100 pour conserver deux décimales
  
  return dataFrame;
}

void sendDataFrame(const String &dataFrame) {
  // Envoyer la trame de données via la communication série
  Serial.println(dataFrame);
}

void displayData(float temperature, float humidity) {
  // Afficher les données sur l'écran OLED
  display.clear();
  display.drawString(0, 0, "Temperature: " + String(temperature) + " C");
  display.drawString(0, 16, "Humidity: " + String(humidity) + " %");
 
  display.display();

  // Afficher les données sur la sortie série terminal
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
  
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  
}