#define RX2 16
#define TX2 17
#define BLYNK_TEMPLATE_ID "" // define your template id
#define BLYNK_TEMPLATE_NAME "" //define your template name
#include "secrets.h" //define your secrets
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "secrets.h"

#define RX2 16
#define TX2 17

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RX2, TX2); // Begin Serial2 communication
  Blynk.begin(auth, ssid, password);
}

void loop() {
  if (Serial2.available()) {
    String data = Serial2.readStringUntil('\n');
    Serial.println("Received data: " + data); // Print received data to Serial monitor
    // Parse the received data
    double Vrms, Irms, ApparentPower, RealPower, PowerFactor, ReactivePower;
    sscanf(data.c_str(), "Vrms: %lf V, Irms: %lf mA, Apparent Power: %lf VA, Real Power: %lf W, Power Factor: %lf , Reactive Power: %lf VAR", &Vrms, &Irms, &ApparentPower, &RealPower, &PowerFactor, &ReactivePower);
    // Print parsed values to Serial monitor
    Serial.print("Vrms: ");
    Serial.print(Vrms);
    Serial.print(", Irms: ");
    Serial.print(Irms);
    Serial.print(", ApparentPower: ");
    Serial.print(ApparentPower);
    Serial.print(", RealPower: ");
    Serial.print(RealPower);
    Serial.print(", PowerFactor: ");
    Serial.print(PowerFactor);
    Serial.print(", ReactivePower: ");
    Serial.println(ReactivePower);
    // Send parsed values to Blynk
    Blynk.virtualWrite(V1, Vrms);
    Blynk.virtualWrite(V2, Irms);
    Blynk.virtualWrite(V3, ApparentPower);
    Blynk.virtualWrite(V4, RealPower);
    Blynk.virtualWrite(V5, PowerFactor);
    Blynk.virtualWrite(V6, ReactivePower);
  }
  // Blynk.virtualWrite(V2, 404);
  Blynk.run();
}
