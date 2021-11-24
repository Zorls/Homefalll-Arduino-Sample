/*
* Author: Overfalll Unipessoal Lda
* Desc: Sample Arduino Buuz Line Looper Homefalll
* Version: 0.0.1
* Date: 22/11/2021
*/

#define _EXTENDED_TRACES

uint8_t controllerIndex = 0;

/*Both this numbers can change*/
static const uint8_t ledPinNumber = 3;
static const uint8_t pullUpPinNumber = 4;
static const uint32_t STANDART_BAUDRATE = 9600;

void setup() {
	Serial1.begin(STANDART_BAUDRATE);
	Serial.begin(STANDART_BAUDRATE);
	
	pinMode(ledPinNumber, OUTPUT);
	pinMode(pullUpPinNumber, INPUT_PULLUP);
}

void loop() {
	if(digitalRead(pullUpPinNumber) != LOW) {
		// Nothing for now
	} else {
		Serial1.write(controllerIndex);
#if defined(_EXTENDED_TRACES)
		Serial.println(controllerIndex);
#endif
		
		digitalWrite(ledPinNumber, HIGH);
		delay(500);
		
		digitalWrite(ledPinNumber, LOW);
		delay(500);
		
		if(controllerIndex >= 3) { controllerIndex = 0; }
		else { ++controllerIndex; }
	}
}
