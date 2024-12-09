#include <Arduino.h>
#include <Wifi.h>
#include <Firebase_ESP_Client.h>

// Replace with your network credentials
#define WIFI_SSID "TP-LINK Archer AX72"
#define WIFI_PASSWORD "cugicugi"

// Firebase credentials
#define API_KEY "AIzaSyBoV8ds0RXsnZUTorLcRE4BU2aBEuGp9oU"
#define DATABASE_URL "https://ardzz-ce3d3.firebaseio.com/"

FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;

int patient_1 = 15;
int patient_2 = 16;

void initWiFi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi...");
    while (WiFiClass::status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Connected!");
}

void blinkLed(int ledPin, int times, int interval) {

    for (int i = 0; i < times; i++) {
        digitalWrite(ledPin, HIGH);
        delay(interval);
        digitalWrite(ledPin, LOW);
        delay(interval);
    }

}

void multiBlinkLed(int ledPin1, int ledPin2, int times, int interval) {

    for (int i = 0; i < times; i++) {
        digitalWrite(ledPin1, HIGH);
        digitalWrite(ledPin2, HIGH);
        delay(interval);
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
        delay(interval);
    }

}

void setup() {
    Serial.begin(9600);
    pinMode(patient_1, OUTPUT); // Set the built-in LED pin as output
    pinMode(patient_2, OUTPUT); // Set the built-in LED pin as output
    initWiFi();

    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;

    if (Firebase.signUp(&config, &auth, "", "")){
        Serial.println("ok");
    }
    else{
        Serial.printf("%s\n", config.signer.signupError.message.c_str());
    }

    Firebase.begin(&config, &auth);
}

void loop() {
    if (Firebase.ready()) {

        Firebase.RTDB.getBool(&firebaseData, "patients/satu_state");

        int satu_state = firebaseData.boolData();


        Firebase.RTDB.getBool(&firebaseData, "patients/dua_state");

        int dua_state = firebaseData.boolData();

        Serial.print("satu_state: ");
        Serial.println(satu_state);
        Serial.print("dua_state: ");
        Serial.println(dua_state);

        if(satu_state && dua_state) {
            multiBlinkLed(patient_1, patient_2, 5, 500);
        }else{
            if (satu_state) {
                blinkLed(patient_1, 5, 500);
            }
            if (dua_state) {
                blinkLed(patient_2, 5, 500);
            }
        }
    }
}