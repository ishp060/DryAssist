#include <SPI.h>

#define CS 2  // Chip Select pin on D2
#define buzzer 5 //need PWM for tone
//built-in pins: MISO=D12, SCK=D13 (not using MOSI/D11)
float pressureRaw = 0.0;

void setup() {
  Serial.begin(115200);
  SPI.begin();
  pinMode(buzzer, OUTPUT);
  noTone(buzzer);
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);  // Deselect sensor
}

float convertPressure(uint16_t pressureRaw) {
  return 0.375 * pressureRaw - 18.75;
  //Might need to change this; it seems like it outputs the raw decimal value (out of 2^14) instead of the %
}

void loop() {
    digitalWrite(CS, LOW);  // Select the sensor
    delay(10);
    SPI.beginTransaction(SPISettings(250000, MSBFIRST, SPI_MODE0));  // 250 kHz, Mode 0

    byte MSB = SPI.transfer(0x00);  // Read first byte (2 bits of status, 6 of pressure MSB)
    byte pressureLSB = SPI.transfer(0x00);  // Read pressure LSB
    byte temperature = SPI.transfer(0x00);  // Read temperature

    SPI.endTransaction();
    digitalWrite(CS, HIGH);  // Deselect the sensor

    //THESE NEXT 3 LINES OF CODE IS WHERE THE MISTAKE LIKELY LIES. I'M PROBABLY REARRANGING THE DATA WRONG.
    byte status = (MSB >> 6) & 0x03;  // Extract S1 and S0 bits

    uint16_t pressureMSB = MSB & 0x3F;  // Mask the last 6 bits of the first byte (pressure bits)
    // Extract 12-bit pressure value
    uint16_t pressureRaw = ((pressureMSB << 8) | pressureLSB) & 0x0FFF;


    // Debug print
    //Serial.print("Status Byte: "); Serial.println(status, HEX);
    //Serial.print("Pressure MSB: "); Serial.print(pressureMSB, HEX);
    //Serial.print(" Pressure LSB: "); Serial.println(pressureLSB, HEX);

    // Check the status bits (S1 and S0 in the first byte)
    if (status == 0) {  // Normal operation, valid data
        // Print raw pressure value
        Serial.print("Raw Pressure: "); Serial.print(pressureRaw);

        float pressurePSI = convertPressure(pressureRaw);
        if (pressurePSI > 50000.0) { //change pressure threshold value after testing
          //shut vacuum off
          //implement some kind of "while" function or some way to stop the buzzer and restart the machine when a switch is flipped (look into making an interrupt)
          //tone(buzzer,440);
          //delay(1000);
          //noTone(buzzer);
          //delay(3000);
          Serial.print(" | HIGH PRESSURE WARNING: Converted Pressure: "); Serial.print(pressurePSI, 2); Serial.println(" PSI");
        }
        else {
          Serial.print(" | Converted Pressure: "); Serial.print(pressurePSI, 2); Serial.println(" PSI");
        }
        
    } else {
        if (status == 1) {
            Serial.println("Stale data or diagnostic condition detected.");
        }
    }

    delay(500);
}
