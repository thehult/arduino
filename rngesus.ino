

int button = 1;

#define DISPLAYS 2
#define SEGMENTS 8

int leds[DISPLAYS][SEGMENTS] = {
  { 7, 6, 4, 3, 2, 8, 9, 5 },
  { 18, 15, 14, 16, 10, 19, 20, 21 }
};

int a = 0;
int b = 1;
int c = 2;
int d = 3;
int e = 4;
int f = 5;
int g = 6;
int DP = 7;
  
int num[10][8] = {
  {6, a, b, c, d, e, f, 0}, //0
  {2, b, c, 0, 0, 0, 0, 0}, //1
  {5, a, b, g, e, d, 0, 0}, //2
  {5, a, b, g, c, d, 0, 0}, //3
  {4, f, g, b, c, 0, 0, 0}, //4
  {5, a, f, g, c, d, 0, 0}, //5
  {6, a, f, g, c, d, e, 0}, //6
  {3, a, b, c, 0, 0, 0, 0}, //7
  {7, a, b, c, d, e, f, g}, //8
  {6, a, b, f, g, c, d, 0} //9 
};

int ANODE = 0;

int writeDots[] = {0, 0};
void setup() {
  for(int j = 0; j < DISPLAYS; j++) {
    for(int i = 0; i < SEGMENTS; i++) {
      pinMode(leds[j][i], OUTPUT);
    }
  }
  
  pinMode(ANODE, OUTPUT);
  pinMode(button, INPUT);
  
  digitalWrite(ANODE, HIGH);
  setAll(-1, HIGH);
}

int lastButtonState = LOW;
int buttonState;

long lastDebounceTime = 0;
long debounceDelay = 50;

long stateChangeDelay = 2000;

int randomNumber=0;
int randomNumberMax=6;

int state=0;
int noStates=2;

int writeValue = 0;

void loop() {
  int reading = digitalRead(button);
  if(reading != lastButtonState) {
     lastDebounceTime = millis(); 
  }
  
  if((millis() - lastDebounceTime) > debounceDelay) {
     if(reading != buttonState) {
        buttonState = reading;
        if(buttonState == HIGH) {
          if((millis() - lastDebounceTime) > stateChangeDelay) {
           state = (state + 1) % noStates; 
           switch(state) {
            case 0:
             writeDots[0] = 0;
             writeDots[1] = 0;
             break;
            case 1:
              writeDots[0] = 1;
              writeDots[1] = 1;
              randomNumberMax = 2;
              break;
           }
          } else {
           switch(state) {
            case 0:
              randomNumber = random(randomNumberMax) + 1;
              writeValue = randomNumber;
              break;
            case 1:
              randomNumberMax = (randomNumberMax + 1) % 99;
              writeValue = randomNumberMax;
              break;
           } 
          }
        }
     } 
  }
  writeNum(writeValue, writeDots);
  lastButtonState = reading;
}




void setAll(int seg, uint8_t val) {
  if(seg == -1) {
    for(int j = 0; j < DISPLAYS; j++) {
      for(int i = 0; i < SEGMENTS; i++) {
        digitalWrite(leds[j][i], val);
      }
    }
  } else {
   for(int i = 0; i < SEGMENTS; i++) {
    digitalWrite(leds[seg][i], val); 
    }
  }
}

void writeDigit(int seg, int dig, int dot) {
  setAll(seg, HIGH);
  for(int i = 1; i <= num[dig][0]; i++) {
   digitalWrite(leds[seg][num[dig][i]], LOW); 
  }
  if(dot == 1) {
   digitalWrite(leds[seg][DP], LOW); 
  }
}

void writeNum(int num, int dots[]) {
  if(num == -1) {
    setAll(-1, HIGH);
   return; 
  }
  writeDigit(0, num % 10, dots[0]);
  writeDigit(1, num / 10, dots[1]);
}
