const int buttonPin = 2;
const int ledPinMain =  4;  // indicateur état système
// relais actionnant les LEDs
const int relayPin1 =  8;
const int relayPin2 =  9;
const int relayPin3 =  10;
const int relayPin4 =  11;
// pins du capteur ultra-sons
const int trigPin = 12;
const int echoPin = 13;

int photocellPin = 0; // the cell and 10K pulldown are connected to a0
int photocellReading; // the analog reading from the analog resistor divider

int buttonState = 0;         // variable for reading the pushbutton status

// https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/
// variables ultra-sons
long duration;
int distance;

unsigned long prevMillis = 0;
unsigned long currentMillis = 0;

void setup() {
  pinMode(ledPinMain, OUTPUT);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(relayPin3, OUTPUT);
  pinMode(relayPin4, OUTPUT);
  
  pinMode(buttonPin, INPUT);
  
  digitalWrite(ledPinMain, HIGH); // système actif par défaut
  // relais actionné par défaut pour éteindre les 4 LEDS
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, HIGH);
  digitalWrite(relayPin3, HIGH);
  digitalWrite(relayPin4, HIGH);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  photocellReading = analogRead(photocellPin);

  currentMillis = millis();
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2000);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10000);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance = duration * 0.034 / 2;
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  if (buttonState == HIGH && digitalRead(ledPinMain) == HIGH) {
    // cas contrôle manuel
    // indicateur éteins
    digitalWrite(ledPinMain, LOW);  
    // actionner les relais
    digitalWrite(relayPin1, LOW);
    digitalWrite(relayPin2, LOW);
    digitalWrite(relayPin3, LOW);
    digitalWrite(relayPin4, LOW);
    delay(10000);
  } else {
    // turn LED off:
    digitalWrite(ledPinMain, HIGH);  
  }
  if (buttonState == LOW && digitalRead(ledPinMain) == HIGH && distance<10) {  // contrôle automatique
    Serial.print("Analog reading = ");
    Serial.print(photocellReading); // valeur codée sur 10 bits (0-1023)
    
    if (photocellReading < 350) {
      if((currentMillis-prevMillis)>10000) {
        digitalWrite(relayPin1, LOW);
        digitalWrite(relayPin2, LOW);
        digitalWrite(relayPin3, LOW);
        digitalWrite(relayPin4, LOW);
        Serial.println(" - Noir");
        prevMillis = currentMillis;
      }
    } else if (photocellReading < 420) {
      if((currentMillis-prevMillis)>10000) {
        digitalWrite(relayPin1, LOW);
        digitalWrite(relayPin2, LOW);
        digitalWrite(relayPin3, LOW);
        digitalWrite(relayPin4, HIGH);
        Serial.println(" - Sombre");
        prevMillis = currentMillis;
      }
    } else if (photocellReading < 500) {
      if((currentMillis-prevMillis)>10000) {
        digitalWrite(relayPin1, LOW);
        digitalWrite(relayPin2, LOW);
        digitalWrite(relayPin3, HIGH);
        digitalWrite(relayPin4, HIGH);
        Serial.println(" - Lumiere");
        prevMillis = currentMillis;
      }
    } else if (photocellReading < 600) {
      if((currentMillis-prevMillis)>10000) {
        digitalWrite(relayPin1, LOW);
        digitalWrite(relayPin2, HIGH);
        digitalWrite(relayPin3, HIGH);
        digitalWrite(relayPin4, HIGH);
        Serial.println(" - Lumineux");
        prevMillis = currentMillis;
      }
    } else {
      if((currentMillis-prevMillis)>10000) {
        digitalWrite(relayPin1, HIGH);
        digitalWrite(relayPin2, HIGH);
        digitalWrite(relayPin3, HIGH);
        digitalWrite(relayPin4, HIGH);
        Serial.println(" - Tres lumineux");
        prevMillis = currentMillis;
      }
    }
  } else {
      Serial.println("Salle deserte");
      digitalWrite(relayPin1, HIGH);
      digitalWrite(relayPin2, HIGH);
      digitalWrite(relayPin3, HIGH);
      digitalWrite(relayPin4, HIGH);
  }
}
