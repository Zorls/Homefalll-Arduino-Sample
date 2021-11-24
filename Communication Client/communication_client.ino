/*
* Author: Overfalll Unipessoal Lda
* Desc: Sample Arduino Communication Client Homefalll
* Version: 0.0.2
* Date: 01/08/2021
*/

#include <homefalll_vector.h>

/*
	Simple data packages strings:
		-> setup_hardware:
			:: setup_hardware # !WORKING_PIN # PIN_TYPE! # ... ::
		-> manage_basic:
			:: manage_basic # !WORKING_PIN # PIN_STATE! ::
		-> manage_blind:
			:: manage_blind # !WORKING_PIN # BLIND_TIME! ::
		-> manage_dimmer:
			:: manage_dimmer # !WORKING_PIN # PWM_VALUE! ::
*/

/*------------------------------------------------*/

#define HOMEFALLL_HARDWARE_VERSION 1
#define WORKING_BAUD_RATE 9600
#define MAX_DIMMER_PWM 255
#define PACKAGES_SPLITTER "#"

#define _DEBUG //Print debug information

typedef HomefalllVector<String> gen_func_args;
typedef void (*gen_func)(gen_func_args);
typedef struct {String key; gen_func func;} TAction;
typedef struct {uint8_t workingPin, finish_state; int ms_time;} TTimer;

void handle_data();
void print_homefalll_version(gen_func_args);
void hardware_setup(gen_func_args);
void basic_handler(gen_func_args);
void blind_handler(gen_func_args);
void dimmer_handler(gen_func_args);
void refresh_blind_timer();

HomefalllVector<TTimer> blind_v_check = {};
TAction actions_list[] = {
	{"homefalll_hardware_version",	print_homefalll_version},
	
	{"setup_hardware",				hardware_setup},
	{"manage_basic",				basic_handler},
	{"manage_blind",				blind_handler},
	{"manage_dimmer",				dimmer_handler},
};

/*------------------------------------------------*/

void setup() {
	Serial.begin(WORKING_BAUD_RATE);
}

void loop() {
	refresh_blind_timer();
	if (Serial.available() > 0) {
		String inData = Serial.readString();
#if defined(_DEBUG)
		Serial.println("Received data string: " + inData);
#endif
		handle_data(inData);
	}
}

void print_homefalll_version(gen_func_args) {
	Serial.println("Homefalll hardware version: " + String(HOMEFALLL_HARDWARE_VERSION));
}

void handle_data(String inData) {
	char m_strData[inData.length()];
	memset(m_strData, 0, sizeof(m_strData));
	inData.toCharArray(m_strData, (sizeof(m_strData)/sizeof(m_strData[0])));
	
	char* token = NULL;
	if (!(token = strtok(m_strData, PACKAGES_SPLITTER))) {
#if defined(_DEBUG)
		Serial.println("Cant get the received package data type");
#endif
	} else {
		const String inDataKey = String(token);
		const size_t actionsLength = (sizeof(actions_list)/sizeof(actions_list[0]));
		
		for (uint8_t i = 0; i < actionsLength; ++i) {
			const TAction actionData = actions_list[i];
			if (inDataKey.equals(actionData.key)) {
				gen_func_args list;
				while ((token = strtok(NULL, PACKAGES_SPLITTER)) != NULL)
					list.Append(String(token));
#if defined(_DEBUG)
				if(list.moveToStart()) {
					Serial.println("Passed args to handler:");
					do { Serial.println("-> " + list.getCurrent());
					} while(list.next());
				}
#endif
				actionData.func(list);
			}
		}
	}
}

void hardware_setup(gen_func_args args) {
	const size_t argsSize = args.getLength();
	if ((argsSize % 2) != 0) {
#if defined(_DEBUG)
		Serial.println("Cant setup the hardware, bad arguments!");
#endif
		return;
	}
	
	if(!args.moveToStart()) {
#if defined(_DEBUG)
		Serial.println("Cant setup the hardware, getting arguments!");
#endif
		return;
	}
	
	enum {OUTPUT_PIN, INPUT_PIN};  
	for (uint8_t i = 0; i < (argsSize/2); ++i) {
		uint8_t workingPin = static_cast<uint8_t>(args.getCurrent().toInt());
		uint8_t pinType = static_cast<uint8_t>(args.getNext().toInt());
		
#if defined(_DEBUG)
		Serial.println("PinMode in pin: {"+String(workingPin)+"} with type: {"+String(pinType)+"}");
#endif
		
		pinMode(workingPin, (pinType == OUTPUT_PIN ? OUTPUT : INPUT));
		args.next();
	}
}

void basic_handler(gen_func_args args) {
	const size_t argsSize = args.getLength();
	if (argsSize != 2) {
#if defined(_DEBUG)
		Serial.println("Cant handling the bool call, bad arguments!");
#endif
		return;
	}
	
	if(!args.moveToStart()) {
#if defined(_DEBUG)
		Serial.println("Cant handling the bool call, getting arguments!");
#endif
		return;
	}
	
	enum {SET_OFF, SET_ON, CHANGE_STATE};
	uint8_t workingPin = static_cast<uint8_t>(args.getCurrent().toInt());
	uint8_t pinState = static_cast<uint8_t>(args.getNext().toInt());
	
#if defined(_DEBUG)
	Serial.println("[BASIC] Action in pin: {"+String(workingPin)+"} with state: {"+String(pinState)+"}");
#endif
	
	digitalWrite(workingPin,\
		pinState == SET_ON ? HIGH : (pinState == CHANGE_STATE ? !digitalRead(workingPin): LOW));
}

void refresh_blind_timer() {
	enum {SET_OFF, SET_ON};
	if(blind_v_check.moveToStart()) {
		do {
			TTimer dataTimer = blind_v_check.getCurrent();
			if (dataTimer.ms_time <= 1) {
				digitalWrite(dataTimer.workingPin,\
					(dataTimer.finish_state == SET_ON ? HIGH : LOW));
			} else {
				--dataTimer.ms_time;
				Serial.println(String(dataTimer.ms_time));
				blind_v_check.Update(dataTimer);
			}
		} while (blind_v_check.next());
	}
}

void blind_handler(gen_func_args args) {
	const size_t argsSize = args.getLength();
	if (argsSize != 3) {
#if defined(_DEBUG)
		Serial.println("Cant handling the blind call, bad arguments!");
#endif
		return;
	}
	
	if(!args.moveToStart()) {
#if defined(_DEBUG)
		Serial.println("Cant handling the blind call, getting arguments!");
#endif
		return;
	}
	
	enum {SET_OFF, SET_ON};
	uint8_t workingPin = static_cast<uint8_t>(args.getCurrent().toInt());
	uint8_t pinState = static_cast<uint8_t>(args.getNext().toInt());
	int blindTime = args.getNext().toInt(); 
	
	digitalWrite(workingPin, (pinState == SET_ON ? HIGH : LOW));
	
	TTimer timer_to_change_state;
	timer_to_change_state.workingPin = workingPin;
	timer_to_change_state.finish_state = static_cast<uint8_t>(!pinState);
	timer_to_change_state.ms_time = 2000;
	blind_v_check.Append(timer_to_change_state);
}

void dimmer_handler(gen_func_args args) {
	const size_t argsSize = args.getLength();
	if (argsSize != 2) {
#if defined(_DEBUG)
		Serial.println("Cant handling the bool call, bad arguments!");
#endif
		return;
	}
	
	if(!args.moveToStart()) {
#if defined(_DEBUG)
		Serial.println("Cant handling the bool call, getting arguments!");
#endif
		return;
	}
	
	uint8_t workingPin = static_cast<uint8_t>(args.getCurrent().toInt());
	int pwmValue = args.getNext().toInt();
	if (pwmValue > MAX_DIMMER_PWM) pwmValue = MAX_DIMMER_PWM;
	
#if defined(_DEBUG)
	Serial.println("[DIMMER] Action in pin: {"+String(workingPin)+"} with pwm: {"+String(pwmValue)+"}");
#endif
	
	analogWrite(workingPin, pwmValue);
}
