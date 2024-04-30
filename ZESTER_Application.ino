#define BLYNK_PRINT Serial // Use Serial as the output for Blynk debugging messages
// Define BLYNK_PRINT to output debugging messages to the Serial port

#include "EmonLib.h"   //https://github.com/openenergymonitor/EmonLib
// Include the EmonLib library for energy monitoring

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h> // Include the Blynk library for wireless communication
// Include necessary libraries for WiFi and Blynk communication

EnergyMonitor emon; // Create an instance of the EnergyMonitor class for energy monitoring
#define vCalibration 83.3  // Voltage calibration factor
#define currCalibration 1.2  // Current calibration factor
// Define calibration factors for voltage and current readings

BlynkTimer timer; // Create a timer object to run the myTimerEvent function periodically
// Create a BlynkTimer object to schedule periodic tasks

char auth[] = "dWXD5Peyp21EYt1kdkxdL4Y2z1L5YfSf"; // Blynk authentication code
// Blynk authentication code for connecting to the Blynk server

char ssid[] = "Carl"; // WiFi SSID
char pass[] = "pengemunabente"; // WiFi password
// WiFi network credentials

float kWh = 0; // Variable to store the accumulated energy consumption in kWh
unsigned long lastmillis = millis(); // Variable to store the time of the last myTimerEvent execution
// Initialize variables for energy consumption tracking

void myTimerEvent() { // Function executed periodically by the timer
    emon.calcVI(20, 2000); // Calculate voltage and current values
    Serial.print("Vrms: "); // Print voltage value to serial monitor
    Serial.print(emon.Vrms, 2); // Print voltage value to serial monitor with 2 decimal places
    Serial.print("V"); // Print unit of voltage to serial monitor
    Blynk.virtualWrite(V0, emon.Vrms); // Send voltage data to Blynk
    Serial.print("\tIrms: "); // Print current value to serial monitor
    Serial.print(emon.Irms, 4); // Print current value to serial monitor with 4 decimal places
    Serial.print("A"); // Print unit of current to serial monitor
    Blynk.virtualWrite(V1, emon.Irms); // Send current data to Blynk
    Serial.print("\tPower: "); // Print power value to serial monitor
    Serial.print(emon.apparentPower, 4); // Print power value to serial monitor with 4 decimal places
    Serial.print("W"); // Print unit of power to serial monitor
    Blynk.virtualWrite(V2, emon.apparentPower); // Send power data to Blynk
    Serial.print("\t\tkWh: "); // Print energy consumption value to serial monitor
    kWh = kWh + emon.apparentPower*(millis()-lastmillis)/3600000000.0;  // Calculate energy consumption
    // kWh (updated) = kWh (last update) + kWh (new reading)
    Serial.print(kWh, 4); // Print energy consumption value to serial monitor with 4 decimal places
    Serial.println("kWh"); // Print unit of energy consumption to serial monitor
    lastmillis = millis(); // Update lastmillis
    Blynk.virtualWrite(V3, kWh); // Send accumulated energy data to Blynk
}

void setup() {  // This function gets called once at the beginning of the program.
  Serial.begin(9600); // Initialize serial communication at 9600 baud rate
  emon.voltage(35, vCalibration, 1.7); // Initialize voltage sensor
  emon.current(34, currCalibration); // Initialize current sensor
  Blynk.begin(auth, ssid, pass); // Connect to Blynk server
  timer.setInterval(5000L, myTimerEvent); // Set timer interval for myTimerEvent function
}

void loop() { // This function gets called repeatedly after the setup function.
  Blynk.run(); // Run Blynk
  timer.run(); // Run timer
}
