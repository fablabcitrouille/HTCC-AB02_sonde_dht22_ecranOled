Ce programme utilise un module Heltec CubeCell HTCC-AB02 pour lire la température et l'humidité d'un capteur DHT22 et envoyer ces données sous forme de trame hexadécimale.
La trame de données est envoyée via la communication série et contient les valeurs de température et d'humidité en hexadécimal.


Le cycle de lecture se fait dans le loop (delay)



Démarche :
1. Connexion au capteur DHT22 : Le capteur DHT22 est connecté à la broche GPIO5 (D1) et alimenté par la broche VEXT.
2. Lecture des données du capteur DHT22 : La température et l'humidité sont lues toutes les 20 secondes.
3. Création de la trame de données : Les valeurs de température et d'humidité sont converties en hexadécimal et concaténées pour former la trame de données.
4. Envoi de la trame de données : La trame de données est envoyée via la communication série et imprimée pour le débogage.
5. Affichage des données : Les valeurs de température et d'humidité sont affichées sur un écran OLED et sur la sortie série terminal.
6. Affichage du nom du fichier : Le nom du fichier source est affiché sur l'écran OLED au démarrage.
7. Gestion de l'alimentation : La broche VEXT est utilisée pour activer et désactiver l'alimentation du capteur DHT22 afin de réduire la consommation d'énergie.
8. La trame préparée pourra être ensuite envoyée sur le serveur LORAWAN TTN et fera l'objet d'un autre code.
9. La librairie du HTCC-AB02 permet de déclarer Vext sans spécifier le GPIO.
10. Pour la mesure de température et d'humidité, laisser un délai de 1 seconde entre les deux mesures.
11. Dans le futur, les données seront envoyées sur The Things Network (TTN) via le protocole LoRaWAN pour une communication longue portée et basse consommation.

Bibliothèques utilisées :
- DHT sensor library : Pour lire les mesures du capteur DHT22.
- Adafruit Unified Sensor : Bibliothèque de capteurs unifiés d'Adafruit.
- HT_SH1107Wire : Pour gérer l'affichage OLED.

*****************************
