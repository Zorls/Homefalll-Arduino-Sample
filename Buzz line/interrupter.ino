/*
* Author: Overfalll Unipessoal Lda
* Desc: Sample Arduino Buuz Line Interrupter Homefalll
* Version: 0.0.1
* Date: 22/11/2021
*/

#define PAUSE_TIME 300
#define PARSE_DATA_TYPE int

static long l_timeStampController = 0;
static bool controlPullupInterrupter = false;

/*Both this numbers can change*/
static const uint8_t interrupterPinNumber = 2;
static const PARSE_DATA_TYPE dataInformation = 41;
static const uint32_t STANDART_BAUDRATE = 9600;

void setup() {
	Serial1.begin(STANDART_BAUDRATE);
	pinMode(interrupterPinNumber, INPUT_PULLUP);
}

void loop() {
	bool _availableSerial = (Serial1.available() > 0);
	if (_availableSerial) {
		int _incomingBytes = Serial1.read();
		Serial1.write(_incomingBytes);
	}
	
	long _l_currentTimeStamp = millis();
	bool _canProceedSignal = (_l_currentTimeStamp - l_timeStampController > PAUSE_TIME);
	
	if(digitalRead(interrupterPinNumber) == LOW && _canProceedSignal) {
		if (!controlPullupInterrupter) {
			Serial1.write(dataInformation);
			
			l_timeStampController = _l_currentTimeStamp;
			controlPullupInterrupter = true;
		}
	} else if (digitalRead(interrupterPinNumber) == HIGH) {
		controlPullupInterrupter = false;
	}
}
