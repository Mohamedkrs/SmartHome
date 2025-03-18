#include <Servo.h>

#define TRIG_PIN 6
#define ECHO_PIN 7
#define SERVO_PIN 9
#define GREEN_LED 10
#define RED_LED 11
#define BUZZER 12
#define TEMP_SENSOR A0

Servo gateServo;
const int parkingSpots = 2;  
int occupiedSpots = 0;
bool overheating = false;

void setup() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    
    gateServo.attach(SERVO_PIN);
    gateServo.write(0); // Gate closed
}

void loop() {
    checkTemperature();
    
    if (overheating) {
        // Alarm: Gate stays open, LEDs flash, buzzer sounds
        gateServo.write(90);
        digitalWrite(BUZZER, HIGH);
        flashLEDs();
    } else {
        digitalWrite(BUZZER, LOW);
        checkParking();
    }

    delay(1000);
}

// 🛠 Check Distance for Parking
void checkParking() {
    long duration;
    int distance;

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration * 0.034 / 2;

    if (distance < 10) {  
        gateServo.write(90); 
        delay(3000);
        gateServo.write(0);
        occupiedSpots++;
    }

    if (occupiedSpots >= parkingSpots) {
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
    } else {
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
    }
}

// 🛠 Check Temperature
void checkTemperature() {
    int sensorValue = analogRead(TEMP_SENSOR);
    float voltage = sensorValue * (5.0 / 1023.0);
    float temperatureC = (voltage - 0.5) * 100.0;

    if (temperatureC > 30.0) {
        overheating = true;
    } else {
        overheating = false;
    }
}

// 🛠 Flash LEDs if overheating
void flashLEDs() {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, HIGH);
    delay(500);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
    delay(500);
}
