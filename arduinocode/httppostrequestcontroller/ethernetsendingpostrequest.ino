//ethernetsendingpostrequest
/*
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

IPAddress ip(192, 168, 1, 150);
IPAddress server_eth(192, 168, 1, 100);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient client;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Ethernet.begin(mac, ip);
  Serial.print("Arduino Static IP Address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  IPAddress server = server_eth;

  if (client.connect(server, 8000)) {
    Serial.println("Connected to Flask server");

    float temperature = 27.0;
    float humidity = 49.0;
    float heat_index = 51.5;

    StaticJsonDocument<200> jsonDoc;
    jsonDoc["temperature"] = temperature;
    jsonDoc["humidity"] = humidity;
    jsonDoc["heat_index"] = heat_index;

    String jsonString;
    serializeJson(jsonDoc, jsonString);

    client.println("POST /vehicle_data HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(jsonString.length());
    client.println();
    client.print(jsonString);

    delay(1000);

    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    client.stop();
  } else {
    Serial.println("Connection to Flask server failed");
  }

  delay(5000);
}
*/
/*
#include <SPI.h>
#include <Ethernet.h>

#define LOOP_A_PIN 2
#define LOOP_B_PIN 4
#define relayPin 9

unsigned long detectionTimestamp = 0;
unsigned long timeout = 180000;  // Timeout after 3 minutes
unsigned long lastBoomSigCheck = 0;  // Track the last check for boomsig
unsigned long boomSigCheckInterval = 5000;  // Check boomsig every 5 seconds

bool loopADetected = false;
bool loopBDetected = false;
bool sequenceComplete = false;

// Ethernet Configuration
//byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};  // Unique MAC address for this Arduino
//IPAddress ip(192, 168, 1, 158);  // Static IP of the Arduino

//gate-2
byte mac[] = {0xED, 0xAD, 0xBE, 0xEF, 0xFE, 0xDE};  // Unique MAC address for this Arduino
IPAddress ip(192, 168, 1, 158);  // Static IP of the Arduino
EthernetClient client;

void setup() {
  // Initialize Ethernet
  Ethernet.begin(mac, ip);
  Serial.begin(9600);

  while (!Serial) {
    ;  // Wait for serial connection
  }

  Serial.print("Machine Gate IP: ");
  Serial.println(Ethernet.localIP());

  pinMode(LOOP_A_PIN, INPUT_PULLUP);
  pinMode(LOOP_B_PIN, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);  // Set the relay to "closed" by default
}

void resetSequence() {
  loopADetected = false;
  loopBDetected = false;
  detectionTimestamp = 0;
}

void sendEntryExitData(bool isEntry) {
  String jsonPayload = "{";
  jsonPayload += "\"gate_id\": \"1\",";
  jsonPayload += "\"event\": \"" + String(isEntry ? "entry" : "exit") + "\"";
  jsonPayload += "}";

  Serial.print("Sending JSON: ");
  Serial.println(jsonPayload);

  if (client.connect("192.168.1.100", 8000)) {  // Connect to Flask server (change IP and port)
    client.print("POST /vehicle_data HTTP/1.1\r\n");
    client.print("Host: 192.168.1.100\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Content-Length: " + String(jsonPayload.length()) + "\r\n");
    client.print("\r\n");  // End of headers
    client.print(jsonPayload);  // Send the payload
    client.print("\r\n");

    unsigned long startTime = millis();
    while (!client.available()) {
      if (millis() - startTime > 5000) {
        Serial.println("Server not responding.");
        client.stop();
        return;
      }
    }

    // Read the server's response
    String response = "";
    while (client.available()) {
      response += (char)client.read();
    }

    Serial.print("Response: ");
    Serial.println(response);

    client.stop();
  } else {
    Serial.println("Connection failed.");
  }
}

void checkLoopSequence() {
  if (sequenceComplete) {
    resetSequence();
    sequenceComplete = false;
    return;
  }

  if (digitalRead(LOOP_A_PIN) == LOW && !loopADetected && !loopBDetected) {
    loopADetected = true;
    detectionTimestamp = millis();
    Serial.println("Loop A detected, waiting for Loop B...");
  }

  if (digitalRead(LOOP_B_PIN) == LOW && !loopBDetected && !loopADetected) {
    loopBDetected = true;
    detectionTimestamp = millis();
    Serial.println("Loop B detected, waiting for Loop A...");
  }

  if (loopADetected && !loopBDetected) {
    if (digitalRead(LOOP_B_PIN) == LOW) {
      Serial.println("Sequence: Loop A -> Loop B (Car Entry)");
      sendEntryExitData(true);  // Send entry event
      resetSequence();
      sequenceComplete = true;
    } else if (millis() - detectionTimestamp > timeout) {
      Serial.println("Timeout: Restarting sequence...");
      resetSequence();
      sequenceComplete = true;
    }
  }

  if (loopBDetected && !loopADetected) {
    if (digitalRead(LOOP_A_PIN) == LOW) {
      Serial.println("Sequence: Loop B -> Loop A (Car Exit)");
      sendEntryExitData(false);  // Send exit event
      resetSequence();
      sequenceComplete = true;
    } else if (millis() - detectionTimestamp > timeout) {
      Serial.println("Timeout: Restarting sequence...");
      resetSequence();
      sequenceComplete = true;
    }
  }
}

void openGate() {
  digitalWrite(relayPin, LOW);  // Trigger the relay to open the gate
  Serial.println("Gate Opened");
  delay(1000);  // Keep the gate open for 5 seconds
  digitalWrite(relayPin, HIGH);  // Close the gate again
}

void checkBoomSig() {
  if (millis() - lastBoomSigCheck >= boomSigCheckInterval) {
    // It's time to check for boomsig
    lastBoomSigCheck = millis();

    if (client.connect("192.168.1.100", 8000)) {  // Connect to Flask server (change IP and port)
      client.print("GET /check_boomsig HTTP/1.1\r\n");
      client.print("Host: 192.168.1.100\r\n");
      client.print("\r\n");  // End of headers

      unsigned long startTime = millis();
      while (!client.available()) {
        if (millis() - startTime > 5000) {
          Serial.println("Server not responding.");
          client.stop();
          return;
        }
      }
      // Read the server's response
      String response = "";
      while (client.available()) {
        response += (char)client.read();
      }

      if (response.indexOf("|OPENEN%") != -1) {
        openGate();
      }

      client.stop();
    }
  }
}

void loop() {
  checkLoopSequence();
  checkBoomSig();
}
*/

#include <SPI.h>
#include <Ethernet.h>

#define LOOP_A_PIN 2
#define LOOP_B_PIN 4
#define relayPin 9

unsigned long detectionTimestamp = 0;
unsigned long timeout = 180000;  // Timeout after 3 minutes
unsigned long lastBoomSigCheck = 0;  // Track the last check for boomsig
unsigned long boomSigCheckInterval = 5000;  // Check boomsig every 5 seconds

bool loopADetected = false;
bool loopBDetected = false;
bool sequenceComplete = false;

// for gate-1
//byte mac[] = {0xED, 0xAD, 0xBE, 0xEF, 0xFE, 0xDE};  // Change for each device
//IPAddress ip(192, 168, 1, 157);  // Change IP for each device

//forgate-2
byte mac[] = {0xDE, 0xAD, 0xBE,0xFE,0xEF, 0xED};  // Change for each device
IPAddress ip(192, 168, 1, 158);  // Change IP for each device

EthernetClient client;

const int gate_id = 2;  // Change this for each device (1, 2, 3)

void setup() {
  Ethernet.begin(mac, ip);
  Serial.begin(9600);

  while (!Serial) {
    ;  // Wait for serial connection
  }

  Serial.print("Machine Gate IP: ");
  Serial.println(Ethernet.localIP());

  pinMode(LOOP_A_PIN, INPUT_PULLUP);
  pinMode(LOOP_B_PIN, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);  // Set the relay to "closed" by default
}

void resetSequence() {
  loopADetected = false;
  loopBDetected = false;
  detectionTimestamp = 0;
}

void sendEntryExitData(bool isEntry) {
  String jsonPayload = "{";
  jsonPayload += "\"gate_id\": \"" + String(gate_id) + "\",";
  jsonPayload += "\"event\": \"" + String(isEntry ? "entry" : "exit") + "\"";
  jsonPayload += "}";

  Serial.print("Sending JSON: ");
  Serial.println(jsonPayload);

  if (client.connect("192.168.1.100", 8000)) {
    client.print("POST /vehicle_data HTTP/1.1\r\n");
    client.print("Host: 192.168.1.100\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Content-Length: " + String(jsonPayload.length()) + "\r\n");
    client.print("\r\n");
    client.print(jsonPayload);
    client.print("\r\n");

    unsigned long startTime = millis();
    while (!client.available()) {
      if (millis() - startTime > 5000) {
        Serial.println("Server not responding.");
        client.stop();
        return;
      }
    }

    String response = "";
    while (client.available()) {
      response += (char)client.read();
    }

    Serial.print("Response: ");
    Serial.println(response);

    client.stop();
  } else {
    Serial.println("Connection failed.");
  }
}

void checkLoopSequence() {
  if (sequenceComplete) {
    resetSequence();
    sequenceComplete = false;
    return;
  }

  if (digitalRead(LOOP_A_PIN) == LOW && !loopADetected && !loopBDetected) {
    loopADetected = true;
    detectionTimestamp = millis();
    Serial.println("Loop A detected, waiting for Loop B...");
  }

  if (digitalRead(LOOP_B_PIN) == LOW && !loopBDetected && !loopADetected) {
    loopBDetected = true;
    detectionTimestamp = millis();
    Serial.println("Loop B detected, waiting for Loop A...");
  }

  if (loopADetected && !loopBDetected) {
    if (digitalRead(LOOP_B_PIN) == LOW) {
      Serial.println("Sequence: Loop A -> Loop B (Car Entry)");
      sendEntryExitData(true);
      resetSequence();
      sequenceComplete = true;
    } else if (millis() - detectionTimestamp > timeout) {
      Serial.println("Timeout: Restarting sequence...");
      resetSequence();
      sequenceComplete = true;
    }
  }

  if (loopBDetected && !loopADetected) {
    if (digitalRead(LOOP_A_PIN) == LOW) {
      Serial.println("Sequence: Loop B -> Loop A (Car Exit)");
      sendEntryExitData(false);
      resetSequence();
      sequenceComplete = true;
    } else if (millis() - detectionTimestamp > timeout) {
      Serial.println("Timeout: Restarting sequence...");
      resetSequence();
      sequenceComplete = true;
    }
  }
}

void openGate() {
  digitalWrite(relayPin, LOW);
  Serial.println("Gate Opened");
  delay(1000);
  digitalWrite(relayPin, HIGH);
}

void checkBoomSig() {
  if (millis() - lastBoomSigCheck >= boomSigCheckInterval) {
    lastBoomSigCheck = millis();

    if (client.connect("192.168.1.100", 8000)) {
      String request = "GET /check_boomsig?gate_id=" + String(gate_id) + " HTTP/1.1\r\n";
      request += "Host: 192.168.1.100\r\n";
      request += "\r\n";
      client.print(request);

      unsigned long startTime = millis();
      while (!client.available()) {
        if (millis() - startTime > 5000) {
          Serial.println("Server not responding.");
          client.stop();
          return;
        }
      }

      String response = "";
      while (client.available()) {
        response += (char)client.read();
      }

      if (response.indexOf("|OPENEN%") != -1) {
        openGate();
      }

      client.stop();
    }
  }
}

void loop() {
  checkLoopSequence();
  checkBoomSig();
}

// flask code 
#Example-2 this code will run for all the arduino and working fine. just make sure one thing in that is we need to make my server ip same like server otherwise it will not work. means my system ip should be 192.168.1.100 subnet mask- 255.255.255.0 and the gateway should be 192.168.1.1 because my arduino is sending the data on the server 192.168.1.100 and port 8000 
from flask import Flask, request, jsonify
import mysql.connector

app = Flask(__name__)

def get_db_connection():
    conn = mysql.connector.connect(host='localhost', user='root', password='', database='vcd')
    return conn

@app.route('/vehicle_data', methods=['POST'])
def receive_vehicle_data():
    data = request.get_json()

    gate = data.get('gate_id')  
    action = data.get('event')  

    if not gate or not action:
        return jsonify({"error": "Missing gate_id or event parameter"}), 400

    try:
        conn = get_db_connection()
        cursor = conn.cursor()
        cursor.execute('''
            INSERT INTO dashboard_carlog (gate, action, created_at, updated_at) 
            VALUES (%s, %s, NOW(), NOW())
        ''', (gate, action))
        conn.commit()
        conn.close()

        print(f"Inserted Data → Gate: {gate}, Action: {action}")

        return jsonify({"message": "Event data received"}), 200

    except mysql.connector.Error as err:
        return jsonify({"error": f"MySQL Error: {err}"}), 500

@app.route('/check_boomsig', methods=['GET'])
def check_boom_signal():
    gate_id = request.args.get('gate_id')

    if not gate_id:
        return jsonify({"error": "Missing gate_id parameter"}), 400

    try:
        conn = get_db_connection()
        cursor = conn.cursor()

        cursor.execute('SELECT entryboom FROM boomsig WHERE id = %s', (gate_id,))
        row = cursor.fetchone()
        print(f"Fetched Row for Gate {gate_id}: {row}")  

        if row and row[0] == 'Y':          
            cursor.execute('UPDATE boomsig SET entryboom = "N" WHERE id = %s', (gate_id,))
            conn.commit()
            conn.close()

            return jsonify({"command": "|OPENEN%"}), 200

        conn.close()
        return jsonify({"command": "NO_ACTION"}), 200

    except mysql.connector.Error as err:
        print(f"MySQL Error: {err}")  
        return jsonify({"error": f"MySQL Error: {err}"}), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8000, debug=True, threaded=True)
