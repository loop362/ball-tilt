//#include <WiFiS3.h>

const char* ssid = "Aloha_Guest";          // Your Wi-Fi SSID
const char* password = "punahou73";       // Your Wi-Fi password
const char* iftttKey = "YqCRIIanL8Q3ds7n1lbYe"; // Your IFTTT key
const char* iftttEventName = "tilt_switch_alert";   // Your IFTTT event name

// Pins
const int buttonPin = 2;          // Push button pin
const int builtInLedPin = 13;     // Built-in LED pin
const int breadboardLedPin = 8;   // Breadboard LED pin

bool ledState = false;
bool lastButtonState = HIGH;

void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(builtInLedPin, OUTPUT);
  pinMode(breadboardLedPin, OUTPUT);

  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected.");
}

void loop() {
  bool buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && lastButtonState == HIGH) {
    ledState = !ledState;

    // Toggle both LEDs
    digitalWrite(builtInLedPin, ledState);
    digitalWrite(breadboardLedPin, ledState);

    if (ledState) {
      sendIFTTTEvent();
    }
    delay(200);
  }

  lastButtonState = buttonState;
}

void sendIFTTTEvent() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;

    // Build the URL
    String url = String("/trigger/") + iftttEventName + "/with/key/" + iftttKey;

    // Connect to IFTTT server
    if (client.connect("maker.ifttt.com", 80)) {
      Serial.println("Connected to IFTTT.");

      // Send HTTP request
      client.println("GET " + url + " HTTP/1.1");
      client.println("Host: maker.ifttt.com");
      client.println("Connection: close");
      client.println();

      // Wait for server response
      while (client.connected() || client.available()) {
        if (client.available()) {
          String response = client.readString();
          Serial.println("IFTTT Response: " + response);
          break;
        }
      }
      client.stop();
      Serial.println("Disconnected from IFTTT.");
    } else {
      Serial.println("Connection to IFTTT failed.");
    }
  } else {
    Serial.println("Wi-Fi not connected.");
  }
}