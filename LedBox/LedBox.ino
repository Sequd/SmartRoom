#define LEDPIN 11
#define PIRPIN 2
#define FADESPEED 10     // чем выше число, тем медленнее будет fade-эффект
int ledState = LOW; // состояние ленты
int pinPhoto = A0; // датчик света
int lightRaw = 0;
int lightPower = 200;

void setup() {
	Serial.begin(9600);
	Serial.println("Start");

	pinMode(LEDPIN, OUTPUT);  // лента
	pinMode(pinPhoto, INPUT); // датчик движения
	pinMode(PIRPIN, INPUT);   // для датчика движения
	digitalWrite(PIRPIN, LOW);
}

int ledMax = 200;
int ledCurrent = 0;
void loop() {
	// NEW ----------------------------

	bool needPIR = false;
	int globalLight = analogRead(pinPhoto);
	if (globalLight > 50) {
		// если светло в комнате.
		if (ledCurrent > 0)
		{
			// fade down
			ledCurrent--;
		}
	}
	else {
		// если темно
		needPIR = true;
	}

	if (needPIR) {
		int pirState = digitalRead(PIRPIN);
		if (pirState == HIGH) {
			// led enable
			if (ledCurrent < 200)
			{
				// fade up
				ledCurrent++;
			}
		}
		else {
			// led disable
			if (ledCurrent > 0)
			{
				// fade down
				ledCurrent--;
			}
		}
	}

	if (ledCurrent != 0) {
		analogWrite(LEDPIN, lightPower);
		delay(FADESPEED);
	}
	else {
		// отправляем спать
	}

	// OLD ----------------------------

	if (ledState == HIGH) {
		analogWrite(LEDPIN, lightPower);
		lightRaw = analogRead(pinPhoto);
		if (lightRaw > 50) { // если свет горит.
			lightOff();
		}
		if (digitalRead(PIRPIN) == LOW) {
			delay(500);
			if (digitalRead(PIRPIN) == LOW) {
				lightOff();
			}
		}
		else {
			Serial.println("Moved detected");
		}
	}
	else {
		lightRaw = analogRead(pinPhoto);
		if (lightRaw < 50) { // если света нет.
			Serial.println("Light nothing");
			lightProcessor();
		}
	}
	delay(500);
}

void lightProcessor() {
	Serial.println("lightProcessor");
	if (digitalRead(PIRPIN) == HIGH)
	{
		Serial.println("Moved detected");
		if (ledState == LOW) {
			lightOn();
		}
	}
	else
	{
		Serial.println("Moved nothing");
		if (ledState == HIGH) {
			lightOff();
		}
	}
}

void lightOn() {
	// разгорание света
	for (int r = 0; r <= 200; r++) {
		analogWrite(LEDPIN, r);
		delay(FADESPEED);
	}

	ledState = HIGH; // включаем
	Serial.println("lightOn");
}

void lightOff() {
	// затухание света
	for (int r = 200; r >= 0; r--) {
		analogWrite(LEDPIN, r);
		delay(FADESPEED);
	}
	ledState = LOW; // выключаем
	Serial.println("lightOff");
}