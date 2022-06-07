# Datalogger_V2
![example workflow](https://github.com/burakduru/dataloggerv2/actions/workflows/main.yml/badge.svg) [![Outlook Badge](https://img.shields.io/badge/email--000?style=social&logo=microsoft-outlook&logoColor=0078d4&link=mailto:burak.duru@cnrs.fr)](mailto:burak.duru@cnrs.fr) [![Github Badge](https://img.shields.io/badge/GitHub--000?style=social&logo=Github&logoColor=black&link=https://github.com/burakduru)](https://github.com/burakduru)  

__Ce répertoire Github regroupe la partie électronique et informatique embarquée de la carte d'acquistion Datalogger_V2 rédigé par Burak DURU en tant qu'apprenti ingénieur en électronique à la [Division Technique de l'INSU Meudon](https://www.dt.insu.cnrs.fr/).__

## Carte d'acquisition 

Réalisation et mise en œuvre d'une carte d'acquisition avec un microcontrôleur [Teensy 3.6](https://www.pjrc.com/store/teensy36.html) dans le but de recevoir/traiter des valeurs de thermistances/résistances chauffantes et transmettre/reçevoir des informations via plusieurs mode de communications

Fonctionnalités de la carte d’acquisition :
* Microcontrôleur [Teensy 3.6](https://www.pjrc.com/store/teensy36.html) :
  * Processeur 32 bits: Cortex-M4 @ 180 MHz 
  * Mémoires: 1M Flash, 256K RAM et 4K EEPROM
  * Microcontrôleur MK66FX1M0VMD18
  * Port USB High Speed (480 Mbit/sec)
  * 2 port Bus CAN
  * 22 sorties PWM
  * 4 ports I2C
  * 62 broches E/S (dont 48 format DIL compatible breadboard - non tolérante au +5 V). Les entrées/sorties fonctionnent en 3.3 V 
  * 25 entrées analogiques (à 2 ADC résolution 13 bits)
  * 2 sorties analogiques (DAC résolution 12 bits)
  * Port USB Full Speed (12 Mbit/sec)
  * Ethernet jusqu'à 100 Mbits/sec
  * Port intégré (4-bit SDIO) pour carte microSD™ (carte non livrée)
  * Port Audio numérique I2S, 4 canaux Entrée & Sortie
  * 14 Timers Hardware
  * 3 ports SPI (dont 1 avec FIFO)
  * 6 port série (dont 2 FIFO et Baud Rates rapides)
  * Horloge temps réelle
  * Dimensions: 62,3 x 18 x 4,2 mm

  ![teensy3 6](https://user-images.githubusercontent.com/106671909/172415934-2c0cb4e4-b0e8-4aea-b337-f747bfd64cdf.png)
 
* [Socket MikroBUS](https://www.mikroe.com/mikrobus/) pour l’utilisation des modules “Click”
* Ethernet ([Module WIZ820-IO](https://www.wiznet.io/product-item/wiz820io/))
* Sortie DAC, GPIO et commande Interlock Laser
* Bus CAN
* Liaison RS232
* Connectique pour le réseau de thermistances/tensions
* Alimentation externe
* Relais

![datalogger_v2_3D](https://user-images.githubusercontent.com/106671909/172331493-1acb83ba-82b5-4351-945c-fa104018d79a.png)

## Programmes utilisés

* __[Datalogger](https://github.com/burakduru/dataloggerv2/blob/main/Programmes/microdial_datalogger_V2/microdial_datalogger_V2.ino)__ : *Permettant de récupérer les informations de températures et de tensions des connectiques thermistances YSI[...] et de tensions V[...] via une transmission RS232*  
* __[Bus CAN](https://github.com/burakduru/dataloggerv2/blob/main/Programmes/bus_can/bus_can.ino)__ : *Permettant de recevoir/transmettre des trames de données depuis le bus de donnée CAN. (Dans notre exemple, transmission d’une donnée de compteur et de thermistances)*
* __[Ethernet](https://github.com/burakduru/dataloggerv2/blob/main/Programmes/wiz820io_mqtt_ethernet/wiz820io_mqtt_ethernet.ino)__ : *Programme d’exemples pour l’initialisation module Ethernet / Envoi et réception de trames UDP / Connexion à un broker MQTT et le statut de connexion* 
* __[Programmes de tests](https://github.com/burakduru/dataloggerv2/tree/main/Programmes/click_exemples)__ : *Modules “Click” utilisés*
* __[WIFI](https://github.com/burakduru/dataloggerv2/blob/main/Programmes/adafruitio_wifi/adafruit_io_wifi_mqtt_16_04.ino)__ : *Programme pour le scan/connexion des réseaux et connexion à un serveur/broker MQTT* 
* __[LoRa](https://github.com/burakduru/dataloggerv2/blob/main/Programmes/LoRa_microdial_send_temp/LoRa_microdial_send_temp.ino)__ : *Plusieurs programmes permettant d’initialiser et d’envoyer des trames de données via le protocol de communication bas débit LoRa* 
* __[AdafruitIO | ThinkSpeak](https://github.com/burakduru/dataloggerv2/tree/main/Programmes)__ : *Envoi/réception de données en utilisant une connexion Internet (via Ethernet ou WIFI) à un serveur MQTT* 

## Matériel

* __[PCB](https://github.com/burakduru/dataloggerv2/tree/main/PCB)__: *Fichiers schématique (.schDoc) et PCB (.pcbDoc) sous Altium Designer*

## Schéma fonctionnement complet

* __[Schématique](https://github.com/burakduru/dataloggerv2/blob/main/MicroDial_Data_Log_V2_SCH.pdf)__ : *Fichier PDF regroupant la schématique de la carte* d'acquisition Datalogger_V2

Guide d'utilisation 
===================

<!--ts-->
* [Installation de Teensyduino](#installation-de-teensyduino)    
* [Installation de bibliothèques sous Teensyduino](#installation-de-bibliothèques-sous-teensyduino)  
* [Modules Click Board](#modules-click-board)
* [Pinout DataloggerV2](#pinout-dataloggerV2)
* [Liaison RS232](#liaison-rs232)
* [Visual Studio](#visual-studio) 
* [Bus CAN](#bus-can)  
* [Ethernet](#ethernet)  
* [WIFI](#wifi)
* [AdafruitIO](#adafruitio) 
* [Thingspeak](#thingspeak)
* [LoRa](#lora)  
* [Bluetooth Low Energy](#bluetooth-low-energy)
<!--te-->

Installation de Teensyduino
---------------------------

Télécharger l’Add-On pour l’IDE d’Arduino en utilisant ce lien sous Windows: https://www.pjrc.com/teensy/td_download.html.
Avec Windows 10, il n’y a pas de driver à installer : On choisit donc, le chemin d’accès où se trouve arduino.exe : 

![teensyduino1](https://user-images.githubusercontent.com/106671909/172332083-79fcae2e-37c1-4916-aa26-3ab04869ae83.png)

Choisir comme type de carte sur Arduino :
Teensy 3.6 / USB Type : “Serial” / CPU Speed : “180MHz” / Port : “Numéro de Port du Teensy”

![teensyduino2](https://user-images.githubusercontent.com/106671909/172332130-be41e609-0de8-48ad-a56c-abf7fe5d57f6.png)

Installation de bibliothèques sous Teensyduino
----------------------------------------------

Toutes les bibliothèques nécessaires à l’implémentation des programmes sont disponibles dans le fichier “Bibliothèques”. Copier-coller ces librairies dans les bibliothèques source de la carte Teensy :

![biblitotheque_teensy](https://user-images.githubusercontent.com/106671909/172332763-e17d26d9-7418-440c-8dd6-14237c40ee09.png)

Programmes de tests : 
- Programme de test pour chacun des Click Board présentés
- LED RGB/ Relais/ DAC/ Bus CAN
- Programme de tests des thermistances/résistance chauffante (relevé de température et de tension)
- Plusieurs programmes pour tester la liaison RJ45 (Ethernet) (connexion à un broker MQTT/ UDP/ Webserver et l’état de l’ethernet)
- Test de la liaison WIFI Click Board (Vérification mise à jour firmware/ scan et connexion à des réseaux).
- Connection au broker MQTT IO.Adafruit et ThinkSpeak (Ethernet et WIFI)
- Envoi données de température en passant par le réseau LoRa (LoRa Click)
- Application sous Visual Studio afin de récupérer les valeurs de températures/tensions et contrôler les relais/led RGB et le DAC

Modules Click Board
-------------------

Liste des modules MikroElektronika testés et compatible avec la carte d'acquisition : 
- [GNSS 4 Click](https://www.mikroe.com/gnss-4-click/)
- [GNSS Click](https://www.mikroe.com/gnss-click/)
- [Temp-Hum 4 Click](https://www.mikroe.com/temp-hum-4-click/)
- [Air-Quality 4 Click](https://www.mikroe.com/air-quality-4-click/)
- [Oled-W/B Click](https://www.mikroe.com/oled-w-click/)
- [WIFI 7 Click](https://www.mikroe.com/wifi-7-click/)
- [CAN SPI Click](https://www.mikroe.com/can-spi-33v-click/)
- [BLE ⅔ Click](https://www.mikroe.com/ble-p-click/)
- [LoRa Click](https://www.mikroe.com/lr-click/)

Pinout DataloggerV2
-------------------

![pinout_datalogger](https://user-images.githubusercontent.com/106671909/172334709-dba5243d-9064-475c-aca5-53261286cae3.png)

Liaison RS232
-------------

Programme __[Datalogger](https://github.com/burakduru/dataloggerv2/blob/main/Programmes/microdial_datalogger_V2/microdial_datalogger_V2.ino)__ regroupé en plusieurs fonctions permettant la conversion d’une valeur analogique à numérique puis cette valeur en tension, de cette tension en une valeur de résistance puis en température. Finalement ces valeurs sont transmises via la liaison RS232 ci-dessous :

![rs232](https://user-images.githubusercontent.com/106671909/172340766-75d503e6-aabd-4293-be31-35496c7b558d.png)

Visual Studio
-------------

Pour l’[utilisation](https://visualstudio.microsoft.com/fr/) de l’application Windows Forms “DataLoggerNMEA”, il faut tout d’abord implémenté le code “[interface_C_sharp.ino](https://github.com/burakduru/dataloggerv2/blob/main/Programmes/interface_C_sharp/interface_C_.ino)” dans la carte Data_Logger. L’envoi de la trame NMEA comportant les valeurs de températures des thermistances et de tensions se fait toutes les 5 secondes :

Pour la réception de données envoyées depuis l’application DataLoggerNMEA, le décodage est effectué par la fonction “decoder_message_operateur” et vérifié par “crc_check”.
Depuis le moniteur série, on peut confirmer la donnée reçu par la vérification de l’entête du message (exemple : #RELAIS;2;0 ce qui correspond à une commande d’état bas au relais n°2.) et l’envoi de la trame NMEA.

![moniteur_serie](https://user-images.githubusercontent.com/106671909/172342110-12c74c61-65fc-4e02-b386-ce03dea6783a.png)

Interface de l'application :

![visual_studio](https://user-images.githubusercontent.com/106671909/172341977-24833648-a93b-49fd-901d-f0b4b682870a.png)


Bus CAN
-------

Exemple d'application du bus CAN : Implémentation d’un code de transmission et de réception de trames de données en utilisant un identificateur pour chacune des cartes avec des données de température et de compteurs pour réaliser des tests  

![datalogger_buscan_1](https://user-images.githubusercontent.com/106671909/172338673-980f9af3-078b-422a-92e9-5cd07e13cfd2.png)

![datalogger_buscan](https://user-images.githubusercontent.com/106671909/172338438-1c823222-5f7a-4a7c-a5c3-8f21b0a64516.png)

![image](https://user-images.githubusercontent.com/106671909/172338918-d32aca89-d908-472b-9454-c5b0193fbb7e.png)

Nous avons ici implémenté le code “[bus_can.ino](https://github.com/burakduru/dataloggerv2/blob/main/Programmes/bus_can/bus_can.ino)” pour la carte Data_Logger et “[can_teensy4_1.ino](https://github.com/burakduru/dataloggerv2/blob/main/Programmes/can_teensy4_1/can_teensy4_1.ino)” pour la Teensy 4.1. Lorsque l’on observe le signal CAN_H/CAN_L à l’oscilloscope, on constate deux trames de données. La première trame correspond à l’envoi depuis la carte Data_Logger et la seconde à l’envoi depuis la Teensy 4.1. On peut identifier ces 2 trames par leur ID, “0x7DF” et “0x7E8”. Pour la trame “0x7DF”, data[0] correspond à une donnée de température et data[1..7] correspond à  “2” à “8” en ASCII. Les 4 derniers octets correspondent au CRC (“cyclic redundancy check”).
Pour la trame “0x7E8”, data[0..5] correspond à la donnée recopiée à la réception de “0x7DF” et data[6..7] correspondent à des compteurs qui s’incrémentent à chaque envoi de données.

Ethernet
--------

Installation de la librairie Ethernet :
* Librairie disponible lors de l'installation de Teensyduino  
* Avec la Teensy 3.6 : 2 ports SPI matériels disponible pour notre cas, WIZNET est connecté à SPI1. Pour le bon fonctionnement du module WIZNET, il faut récupérer le fichier "[Ethernet](https://github.com/burakduru/dataloggerv2/blob/main/Libraries%20Altium/Ethernet.zip)" et le copier-coller ici :  

![ethernet](https://user-images.githubusercontent.com/106671909/172344124-17801747-908b-47a3-8511-138c516a800c.png)

Initialisation du module WIZNET :
* Le code ci-dessous permet de réinitialiser le WIZNET :

![code_wiznet](https://user-images.githubusercontent.com/106671909/172344491-011cbab6-65be-4b1a-85f5-7cde4f0c7a6a.png)

* Fixer une adresse MAC et une adresse IP pour un réseau local en utilisant la fonction begin

![adresse_mac](https://user-images.githubusercontent.com/106671909/172344818-f23e0f02-5614-4126-8a7c-3df93deb1182.png)

Programmes d'exemples disponibles : 
* __[wiz820io_linkstatus](https://github.com/burakduru/dataloggerv2/blob/main/Programmes/wiz820io_linkstatus/wiz820io_linkstatus.ino)__ : *Confirmation connexion Ethernet*
* __[wiz820io_mqtt_ethernet](https://github.com/burakduru/dataloggerv2/blob/main/Programmes/wiz820io_mqtt_ethernet/wiz820io_mqtt_ethernet.ino)__ : *Exemple connexion serveur MQTT adafruit.io*
* __[wiz820io_udp_send_receive](https://github.com/burakduru/dataloggerv2/blob/main/Programmes/wiz820io_udp_send_receive/wiz820io_udp_send_receive.ino)__ : *Envoi/Réception trames UDP via Hercules*
* __[wiz820io_webserver](https://github.com/burakduru/dataloggerv2/blob/main/Programmes/wiz820io_webserver/wiz820io_webserver.ino)__ : *Création d’un serveur WEB qui renvoi des valeurs des pins analogiques*

WIFI 
----

Utilisation du module [ATWINC1510](https://www.mikroe.com/wifi-7-click/) :
* Installation de la bibliothèque du module ATWINC1510 depuis le gestionnaire de bibliothèque

![atwinc1510](https://user-images.githubusercontent.com/106671909/172352384-a76fdfb2-3c82-4efc-a831-fe7f0613be19.png)

* Vérification de la version du firmware en compilant le programme "__[CheckWifi101FirmwareVersion.ino](https://github.com/burakduru/dataloggerv2/blob/main/Programmes/wifi_7click_checkwifi_FirmwareVersion/CheckWifi101FirmwareVersion.ino)"__
* Installation du firmware WIFI depuis Arduino :

![firmware_wifi](https://user-images.githubusercontent.com/106671909/172353071-97e8f645-1cf1-4306-bd9e-ff015d17b0a6.png)

* Scan des différents réseaux disponibles en compilant le programme  "__[ScanNetworksAdvanced.ino](https://github.com/burakduru/dataloggerv2/blob/main/Programmes/wifi_7click_scanNetworks/ScanNetworksAdvanced.ino)__"

![scan_wifi](https://user-images.githubusercontent.com/106671909/172353435-a53d1b6d-dbe0-4c37-8b7e-a88d5f95bfe1.png)

On obtient ici plusieurs informations concernant les réseaux :
- Nom du réseau (SSID)
- Amplitude du signal en dBm
- Channel disponible 
- Protocol de chiffrement 

* Connexion à un réseau WIFI WPA en compilant le programme "__[ConnectWithWPA.ino](https://github.com/burakduru/dataloggerv2/blob/main/Programmes/wifi_7click_connectWPA/ConnectWithWPA.ino)__"
Ouvrir tout d’abord “arduino_secrets.h” puis définir le nom du réseau (secret_ssid) puis le mot de passe (secret_pass)  
Compiler finalement le programme le programme : si la connection est bien effectué, on obtient plusieurs information dont une adresse IP attribuée automatiquement au module  

* Connexion au broker MQTT IO.Adafruit via une connexion WIFI en compilant le programme "__[adafruitio_wifi.ino](https://github.com/burakduru/dataloggerv2/blob/main/Programmes/adafruitio_wifi/adafruit_io_wifi_mqtt_16_04.ino)__"

AdafruitIO
----------

[AdafruitIO](https://io.adafruit.com/) est une plateforme IoT qui propose un service cloud permettant à la fois d’afficher/d’envoyer des informations sur une interface web avec une affichage en temps réels de données comme par exemple le contrôle de relais/LEDs pour notre cas

Utilisation d'IO.Adafruit.com :

* Création d’un compte adafruitIO sur https://io.adafruit.com
* Récupération à partir de l’interface “My Key” de l’username et de l’active key

![ioadafruit](https://user-images.githubusercontent.com/106671909/172347334-0758e24b-9c18-4d0f-bfe5-2447485c1ba9.png)

* Utilisation du programme d’exemple IOAdafruit : Contrôle des relais / led et affichage d’une température 
Avant l'utilisation, remplacer la bibliothèque : __8. Ethernet__   
Puis, ajout des bibliothèques AdafruitIO : (dossier “Bibliothèques” sinon “Adafruit IO Arduino” dans le gestionnaire de bibliothèque)

![biblitotheque_adafruit](https://user-images.githubusercontent.com/106671909/172348082-0582ec85-0281-4b48-be03-dcfc316d525c.png)

__Architecture du broker MQTT AdafruitIO :__  

![broker_mqtt](https://user-images.githubusercontent.com/106671909/172346036-7d6264b6-0e51-4404-89f0-e9b7b56e5306.png)

* Température comme data et io.adafruit.com comme MQTT-Broker
* Données de températures acquise par la carte Teensy
* Envoi de data depuis la Teensy vers le broker MQTT en utilisant la bibliothèque Ethernet du module WIZ820-IO et AdafruitMQTT pour publier/souscrire

__Dashboard IO.Adafruit pour gérer/revoir les informations de la carte d'acquisition :__  

Pour comprendre la création/utilisation du dashboard IO.Adafruit se référer aux slides 21 et 22 du guide d'utilisation *"[MicroDial_Data_Logger_V2.pdf](https://github.com/burakduru/dataloggerv2/blob/main/MicroDial_Data_Log_V2_SCH.pdf)"*  

![dashboard_adafruitio](https://user-images.githubusercontent.com/106671909/172346611-2a9bdb0a-44f6-4dc0-a7ad-8a38cd631114.png)

Thingspeak
----------

[Thingspeak](https://thingspeak.com/) est une plateforme IoT semblable à IO.Adafruit :  
* Création d’un compte Thingpseak via thingspeak.com  
* Création d’un channel afin d’afficher sur un dashboard les “fields” :

![channel_thingspeak](https://user-images.githubusercontent.com/106671909/172357852-b4e74429-14e9-43e6-a81f-8685408307a3.png)

* Récupération du Channel ID et de l’API Key pour une connexion à la plateforme 

![thingspeak_lora](https://user-images.githubusercontent.com/106671909/172367679-2a6120ce-828d-46e3-9491-8cccf8b9ccee.png)

LoRa
----

Protocole de télécommunication LoRa bas débit et à faible consommation électrique :

![lorawan](https://user-images.githubusercontent.com/106671909/172371094-faaf80fd-14d3-4c1f-9724-59f1fa514c7b.png)

La transmission LoRa est constitué de plusieurs étapes : 
* Configuration du transceiver RN2483 (LoRa Click) en utilisation la bibliothèque “rn2483”
* Communication RF à 868.5MHz entre le module Click LoRa et le Gateway LoRa de la DT
* Depuis le Gateway connection à Internet puis au serveur TTN (The ThingsNetwork)
* Configuration de TTN afin d’enregistrer le device et de recevoir/envoyer les trames entre les différents modules LoRa et le gateway
* Configuration depuis TTN vers un broker MQTT (ThingSpeak) afin d’afficher par exemple la courbe de la température

Configuration du Gateway Dragino LPS8 : 
* Cette passerelle multi-channel permet de relier le réseau sans fil LoRa(868 MHz) à un réseau IP via une connexion WiFi ou Ethernet

![LoRa_1](https://user-images.githubusercontent.com/106671909/172408047-7e881990-bb4f-4d25-a57e-fda7009c4ef4.png)

* Pour la configuration du gateway, nous disposons de plusieurs possibilités de connexion. Soit une connexion à Internet via un réseau WIFI et une connection LAN du PC vers le Gateway (RJ45) sinon une connexion à Internet via le port WAN (RJ45) et une connexion entre le PC et le gateway en se connectant au réseau WIFI du gateway (dragino-1f….)
* Connexion WIFI à Internet et LAN pour configurer le gateway (adresse 10.130.1.1)  : username : root / password : dragino
* Cliquer sur “LoRaWAN” puis choisir comme Service Provider “The Things Network” et comme Server Adress le serveur européen “TTN-routeur-EU, EU 433 and EU 863-870”.

![LoRa_2](https://user-images.githubusercontent.com/106671909/172408563-7060ef49-09d8-47ba-8f9a-66bb593e0eda.png)

* Se rendre sur “Network” puis “WIFI”, cochez enable WiFi WAN Client puis entrer le nom du réseau WIFI “Host WiFi SSID”, le mot de passe du réseau “Passphrase” et enfin choisir depuis WIFI Survey le nom du réseau puis le type de cryptage. Finalement, la connexion est validée en dessous

![LoRa_3](https://user-images.githubusercontent.com/106671909/172408787-740e07c3-e18b-4bd3-8adc-fcede9de3c62.png)

* Après avoir connecté le Gateway à Internet, vérifions la connection au serveur TTN. Pour cela, il faut se rendre dans “home”

![LoRa_4](https://user-images.githubusercontent.com/106671909/172409038-8b67e32f-fbbd-40c2-9b12-6811e616f734.png)

* Avant de configurer le gateway sur TTN, il faut récupérer le gateway ID. Se rendre sur “LoRaWAN” puis dans “LoRaWAN Configuration” et recopier le Gateway ID

![LoRa_5](https://user-images.githubusercontent.com/106671909/172409225-3fec98b8-37fb-4459-82a0-2428bcdcb50d.png)

* Se connecter à “thethingsnetwork.org”, puis dans “profil” cliquer sur “Console”. Choisir “Gateway” puis “register gateway”. Entrer le Gateway ID précédemment copié, choisir Europe pour Frequency Plan et Indoor pour Antenna Placement

![LoRa_6](https://user-images.githubusercontent.com/106671909/172409442-fba09460-68eb-4a82-be17-0f8b4d235422.png)

Impact du faceur d'étalement sur la portée et le débit (SF - Spreading Factor)

![LoRa_7](https://user-images.githubusercontent.com/106671909/172410244-2fedcb92-8c3f-412a-86b9-555b0d76a513.png)

Bluetooth Low Energy
--------------------------

[BLE P Click](https://www.mikroe.com/ble-p-click) :
* Puce Bluetooth Low Energy (BLE) nRF8001 
* Bus SPI (via le socket MikroBUS)
* Utilisation de bibliothèque open source Arduino => utilisation du module BLE
* Connection avec un smartphone en bluetooth via l’application Bluefruit Connect) 
* Envoi informations de températures et tensions en bluetooth 

![ble_click](https://user-images.githubusercontent.com/106671909/172410543-8e2f8916-d0e0-4beb-a8cc-5769ca41bdb0.png)





