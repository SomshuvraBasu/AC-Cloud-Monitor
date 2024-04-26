#include <SPI.h>
#include <ZMPT101B.h>
#include "EmonLib.h"

#define SENSITIVITY 605.0f    // Voltage sensor sensitivity
#define NUM_SAMPLES 10    // Number of samples for averaging

// ZMPT101B sensor output connected to analog pin A2
// and the voltage source frequency is 50 Hz.
ZMPT101B voltageSensor(A2, 50.0);

// EnergyMonitor instance for current measurement
EnergyMonitor emon1;

void setup() {
    Serial.begin(115200);

    // Change the sensitivity value based on the calibration
    voltageSensor.setSensitivity(SENSITIVITY);

    // Current sensor setup
    emon1.current(A0, 640); // Current: input pin, calibration.

    delay(1000);
}

void loop() {
    // Read the voltage and current
    double Vrms = voltageSensor.getRmsVoltage();
    double Irms = emon1.calcIrms(1480); // Calculate Irms only

    // Calculate apparent power
    double apparentPower = Vrms * Irms / 1000;

    // Calculate real power and power factor
    double sumInstantPower = 0;
    for (int n = 0; n < NUM_SAMPLES; n++) {
        float instantVoltage = Vrms * (1.414) * (0.7);
        float instantCurrent = emon1.calcIrms(1);
        float instantPower = instantVoltage * instantCurrent / 1000;
        sumInstantPower += instantPower;
    }

    double realPower = sumInstantPower / NUM_SAMPLES;
    double reactivePower = (pow(apparentPower, 2) - pow(realPower, 2));
    double powerFactor = realPower / apparentPower;

    // Print the values
    Serial.print("Vrms: ");
    Serial.print(Vrms);
    Serial.print(" V, Irms: ");
    Serial.print(Irms);
    Serial.print(" mA, Apparent Power: ");
    Serial.print(apparentPower);
    Serial.print(" VA, Real Power: ");
    Serial.print(realPower);
    Serial.print(" W, Power Factor: ");
    Serial.print(powerFactor); // Print filtered power factor
    Serial.print(" , Reactive Power: ");
    Serial.print(reactivePower);
    Serial.println(" VAR");

    delay(1000);
}
