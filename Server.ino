/*
  --TODO--
 * *Schalter zum Zurücksetzen
 * *Alle Schrittmotoren aktivieren
 * *Richtige Umdrehungszahl
 * *Namen richtigstellen
*/


// die verschiedenen Libraries werden geladen
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Stepper.h>
#include <EEPROM.h>

// Netzwerkzugangsdaten werden festgelegt
const char *ssid = "Verkaufsautomat";
const char *password = "verkaufsautomat";

// die Schritte pro Auswurf werden für den Schrittmotor festgelegt
int SPU = 2048;

// der Webserver wird initialisiert und bekommt einen Port zugewiesen
WebServer server(80);

// die vier Schrittmotoren werden initialisiert
Stepper Schrittmotor1(SPU, 12, 14, 27, 26);
//Stepper Schrittmotor2(SPU, 12, 14, 27, 26);
//Stepper Schrittmotor3(SPU, 12, 14, 27, 26);
//Stepper Schrittmotor4(SPU, 12, 14, 27, 26);

// Funktion, die ein Produkt auswirft
void dropProduct(int fachnummer) {
  // wenn die Fachnummern übereinstimmen, wird der jeweilige Schrittmotor angesteuert
  if (fachnummer == 1)Schrittmotor1.step(2048);
  //if (fachnummer == 2)Schrittmotor2.step(2048);
  //if (fachnummer == 3)Schrittmotor3.step(2048);
  //if (fachnummer == 4)Schrittmotor4.step(2048);
}

// Funktion, die der App alle Produkte zurückgibt
void getItems() {
  // HTML das zurückgegeben wird
  String html = "[{\"name\":\"Chips\",\"price\":\"4\",\"stock\":" + String(EEPROM.read(0)) + ",\"img\":\"\"},{\"name\":\"Schokolade\",\"price\":\"3\",\"stock\":" + String(EEPROM.read(1)) + ",\"img\":\"\"},{\"name\":\"Wasser\",\"price\":\"2\",\"stock\":" + String(EEPROM.read(2)) + ",\"img\":\"\"},{\"name\":\"Apfel\",\"price\":\"2\",\"stock\":" + String(EEPROM.read(3)) + ",\"img\":\"\"}]";

  // der Server schickt das HTML an den Client zurück
  server.send(200, "text/html", html);
}

// Funktion, die die Bestellung aufnimmt und die Produkte auswirft
void orderItems() {
  // für jedes Produkt(in der URL auch Argument genannt):
  for (int i = 0; i < server.args(); i++) {
    // wenn in der Argumentenliste Chips vorkommt:
    if (server.argName(i) == "Chips") {
      // wird der geforderte Wert ausgelesen
      String value = server.arg(i);

      // wenn die Anzahl der geforderten Produkte unter dem Bestand liegt:
      if (value.toInt() <= EEPROM.read(0)) {
        // Debug Ausgabe
        Serial.println("Chips: " + value);

        // Bestand wird angepasst
        EEPROM.update(0, EEPROM.read(0) - value.toInt());

        // für jedes ausgewählte Produkt der Sorte Chips:
        for (int j = 0; j < value.toInt(); j++) {
          // Fachnummer wird übergeben
          dropProduct(1);
        }
      }
    }

    // wenn in der Argumentenliste Schokolade vorkommt:
    if (server.argName(i) == "Schokolade") {
      // wird der geforderte Wert ausgelesen
      String value = server.arg(i);

      // wenn die Anzahl der geforderten Produkte unter dem Bestand liegt:
      if (value.toInt() <= EEPROM.read(1)) {
        // Debug Ausgabe
        Serial.println("Schokolade: " + value);

        // Bestand wird angepasst
        EEPROM.update(1, EEPROM.read(1) - value.toInt());

        // für jedes ausgewählte Produkt der Sorte Schokolade:
        for (int j = 0; j < value.toInt(); j++) {
          // Fachnummer wird übergeben
          dropProduct(2);
        }
      }
    }

    // wenn in der Argumentenliste Wasser vorkommt:
    if (server.argName(i) == "Wasser") {
      // wird der geforderte Wert ausgelesen
      String value = server.arg(i);

      // wenn die Anzahl der geforderten Produkte unter dem Bestand liegt:
      if (value.toInt() <= EEPROM.read(2)) {
        // Debug Ausgabe
        Serial.println("Wasser: " + value);

        // Bestand wird angepasst
        EEPROM.update(2, EEPROM.read(2) - value.toInt());

        // für jedes ausgewählte Produkt der Sorte Wasser:
        for (int j; j < value.toInt(); j++) {
          // Fachnummer wird übergeben
          dropProduct(3);
        }
      }
    }

    // wenn in der Argumentenliste Apfel vorkommt:
    if (server.argName(i) == "Apfel") {
      // wird der geforderte Wert ausgelesen
      String value = server.arg(i);

      // wenn die Anzahl der geforderten Produkte unter dem Bestand liegt:
      if (value.toInt() <= EEPROM.read(3)) {
        // Debug Ausgabe
        Serial.println("Apfel: " + value);

        // Bestand wird angepasst
        EEPROM.update(3, EEPROM.read(3) - value.toInt());

        // für jedes ausgewählte Produkt der Sorte Apfel:
        for (int j; j < value.toInt(); j++) {
          // Fachnummer wird übergeben
          dropProduct(4);
        }
      }
    }
  }

  // dem Client wird geantwortet, dass seine Bestellung erfolgreich war
  server.send(200, "text/html", "OK");
}

// Funktion zum Zurücksetzen des Bestands
void resetStock() {
  // Bestand wird wieder auf sechs aufgefüllt
  EEPROM.update(0, 6);
  EEPROM.update(1, 6);
  EEPROM.update(2, 6);
  EEPROM.update(3, 6);

  // dem Client wird zurückgegeben, dass das Zurücksetzen erfolgreich war
  server.send(200, "text/html", "Bestand wurde zurückgesetzt");
}

void setup() {
  // die Baudrate wird festgelegt
  Serial.begin(115200);

  // initialisiere EEPROM mit der Speicherkapazität
  EEPROM.begin(512);

  // die Schrittmotoren bekommen die Umdrehungen pro Minute zugewiesen
  Schrittmotor1.setSpeed(10);
  //Schrittmotor2.setSpeed(10);
  //Schrittmotor3.setSpeed(10);
  //Schrittmotor4.setSpeed(10);

  // dem Access Point werden Name(ssid) & Passwort(password) zugewiesen
  WiFi.softAP(ssid, password);

  Serial.println("Access Point erstellt...");

  // IP-Adresse wird ausgegeben
  Serial.print("IP Adresse: 192.168.4.1\n");

  // wird der Server auf diese Routen angesprochen, werden die Funktionen ausgelöst
  server.on("/getItems", getItems);
  server.on("/orderItems", orderItems);
  server.on("/resetStock", resetStock);

  // der Server wird gestartet
  server.begin();

  Serial.println("Server gestartet...");
}

void loop() {
  // der Server kontrolliert die definierten Routen
  server.handleClient();
}
