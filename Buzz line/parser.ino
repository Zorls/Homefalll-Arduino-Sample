/*
* Author: Overfalll Unipessoal Lda
* Desc: Sample Arduino Buuz Line Receiver Homefalll
* Version: 0.0.1
* Date: 22/11/2021
*/

#define PARSE_DATA_TYPE int
struct SReceiverType { PARSE_DATA_TYPE dataInformation; uint8_t pinNumber; };
static const struct SReceiverType dataConfiguration[] = {
	/*			-Configuration Zone-			*/
	{	.dataInformation = 2,	.pinNumber = 3	},
	{	.dataInformation = 21,	.pinNumber = 4	},
	{	.dataInformation = 31,	.pinNumber = 5	},
	{	.dataInformation = 41,	.pinNumber = 6	}
};

static const uint32_t STANDART_BAUDRATE = 9600;
static const uint8_t OUTPUT_PINS_COUNT = sizeof(dataConfiguration)/sizeof(dataConfiguration[0]);

bool setupPinModes();
bool handleIncomingInformation(int incomingData);
bool getConfigurationFromIncomingInformation(int incomingData, struct SReceiverType* data);

void setup() {
	Serial.begin(STANDART_BAUDRATE);
	Serial1.begin(STANDART_BAUDRATE);
	
	if (!setupPinModes()) {
		// Nothing for now
	}
}

void loop() {
	bool _availableSerial = (Serial1.available() > 0);
	if (!_availableSerial) {
		// Nothing for now
	} else {
		int incomingData = Serial1.read();
		if (!handleIncomingInformation(incomingData)) {
			// Nothing for now
		}
	}
}

bool setupPinModes() {
	for (uint8_t idx = 0; idx < OUTPUT_PINS_COUNT; ++idx) {
		struct SReceiverType _data = dataConfiguration[idx];
		pinMode(_data.pinNumber, OUTPUT);
	}
	
	return true;
}

bool handleIncomingInformation(int incomingData) {
	struct SReceiverType data;
	if (!getConfigurationFromIncomingInformation(incomingData, &data)) {
		// Nothing for now
	} else {
		uint8_t _pinNumber = data.pinNumber;
		digitalWrite(_pinNumber, !digitalRead(_pinNumber));
		
		return true;
	}
	
	return false;
}

bool getConfigurationFromIncomingInformation(int incomingData, struct SReceiverType* data) {
	for (uint8_t idx = 0; idx < OUTPUT_PINS_COUNT; ++idx) {
		struct SReceiverType _data = dataConfiguration[idx];
		if (_data.dataInformation == incomingData) {
			*data = _data;
			return true;
		}
	}
	
	return false;
}
