char incomingByte[12];	// for incoming serial data

#include "Energia.h"

// Include application, user and local libraries
#include "SPI.h"
#include "LCD_SharpBoosterPack_SPI.h"
#define LED RED_LED
LCD_SharpBoosterPack_SPI myScreen;
uint8_t k = 0;

void setup() {
  
  //LCD start
  #if defined(__MSP430__)
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    
#elif defined(__LM4F120H5QR__)
    SPI.Select(2);
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV128); // for LM4F120H5QR DIV2 = 4 MHz !
#endif
    
    myScreen.begin();
    
    myScreen.setFont(1);
    myScreen.text(10, 10, "Hello!");
    myScreen.flush();  
    
    delay(1000);
    myScreen.clear();
  //LCD end
    pinMode(LED, OUTPUT);
  
	Serial.begin(9600);	// opens serial port, sets data rate to 9600 bps
}

void loop() {

	// send data only when you receive data:
	if (Serial.available() > 0) {
  digitalWrite(LED, HIGH);
		// read the incoming byte:
                 Serial.readBytes(incomingByte, 12);
		

    myScreen.setFont(0);
    myScreen.text(10, 10, incomingByte);
    myScreen.flush(); 
	}
digitalWrite(LED, LOW);

}
