// Wireless input signal pulse ranges (in units of microseconds)
// Forward band
// Ranges experimentally determined
const unsigned long F_MIN = 17000;
const unsigned long F_MAX = 19500;
// Range of t_on to move left
const unsigned long R_MIN = 3000;
const unsigned long R_MAX = 4500;
// Range of t_on to move forward
const unsigned long B_MIN = 13500;
const unsigned long B_MAX = 15500;
// Range of t_on to move right
const unsigned long L_MIN = 7000;
const unsigned long L_MAX = 9500;
// Stop if it's above here
const unsigned long STOP_HIGHTHRESH = 100000;  // Above highest control signal

// Time to execute each function, in milliseconds
// Smaller gives more responsiveness at the cost of potentially not moving the motors enough
const int default_increment = 80;

// Relative maximum speeds of left and right wheel
// Determined experimentally
const int MAX_RIGHT = 210;        // Right wheel max speed at 210/255 duty cycle
const int MAX_LEFT = 153;         // Left wheel max speed at 153/255 duty cycle

// Number of encoder pulses per degree of rotation for each wheel
const float TURN_RIGHT = 4.29;      // Pulses per degree, right
const float TURN_LEFT = 4.29;       // Pulses per degree, left

// Pin definitions
const int ENCODER_L = 2;    // external interrupt 0, connect left encoder output
const int ENCODER_R = 3;    // external interrupt 1, connect right encoder output

const int pinON = 6;        // Connect output from ON button at front of robot

const int pinCW_L = 7;      // Connect CW speed control enable for left wheel
const int pinCC_L = 8;      // Connect CCW speed control enable for left wheel
const int pinREF_L = 9;     // Connect reference voltage PWM output for left wheel

const int pinREF_R = 10;    // Connect CW speed control enable for right wheel
const int pinCW_R = 11;     // Connect CCW speed control enable for right wheel
const int pinCC_R = 12;     // Connect reference voltage PWM output for right wheel

const int LED = 13;         // Arduino onboard status LED is connected to pin 13

void forward(float spd)
{
  digitalWrite(pinCC_L, LOW);
  digitalWrite(pinCW_R, LOW);
  digitalWrite(pinCW_L, HIGH);
  digitalWrite(pinCC_R, HIGH);
  // Write voltage reference PWM signals
  analogWrite(pinREF_L, (int)(spd*MAX_LEFT));
  analogWrite(pinREF_R, (int)(spd*MAX_RIGHT));
}

void backward(float spd)
{
  digitalWrite(pinCC_R, LOW);
  digitalWrite(pinCW_L, LOW);
  digitalWrite(pinCC_L, HIGH);
  digitalWrite(pinCW_R, HIGH);
  // Write voltage reference PWM signals
  analogWrite(pinREF_L, (int)(spd*MAX_LEFT));
  analogWrite(pinREF_R, (int)(spd*MAX_RIGHT));
}

void left(float spd)
{
  digitalWrite(pinCW_L, LOW);
  digitalWrite(pinCW_R, LOW);
  digitalWrite(pinCC_L, HIGH);
  digitalWrite(pinCC_R, HIGH);
  // Write voltage reference PWM signals
  analogWrite(pinREF_L, (int)(spd*MAX_LEFT));
  analogWrite(pinREF_R, (int)(spd*MAX_RIGHT));
}

void right(float spd)
{
  digitalWrite(pinCC_L, LOW);
  digitalWrite(pinCC_R, LOW);
  digitalWrite(pinCW_L, HIGH);
  digitalWrite(pinCW_R, HIGH);
  // Write voltage reference PWM signals
  analogWrite(pinREF_L, (int)(spd*MAX_LEFT));
  analogWrite(pinREF_R, (int)(spd*MAX_RIGHT));
}

void halt()
{
  digitalWrite(pinCW_L,LOW);      // stop left wheel
  digitalWrite(pinCC_L,LOW);      
  digitalWrite(pinCW_R,LOW);      // stop right wheel
  digitalWrite(pinCC_R,LOW);      
  analogWrite(pinREF_L,0);        // zero speed references
  analogWrite(pinREF_R,0);
}

void setup() {
  pinMode(pinON,INPUT);            // set the ON pin to be an input
  
  pinMode(pinCW_L,OUTPUT);         // set the CW_L pin to be an output
  pinMode(pinCC_L,OUTPUT);         // set the CC_L pin to be an output
  pinMode(pinREF_L,OUTPUT);        // set the REF_L pin to be an output
  
  pinMode(pinCW_R,OUTPUT);         // set the CW_R pin to be an output
  pinMode(pinCC_R,OUTPUT);         // set the CC_R pin to be an output
  pinMode(pinREF_R,OUTPUT);        // set the REF_R pin to be an output
  
  pinMode(A0, INPUT);
  digitalWrite(A0, LOW);
  pinMode(A1, INPUT);
  digitalWrite(A0, LOW);
  
  pinMode(LED,OUTPUT);             // on-board LED is connected to pin 13
  
  digitalWrite(13,LOW);            // turn onboard LED off
  halt();                          // zero all motor control outputs
  Serial.begin(9600);              // begin serial communication at 9600 baud
}

char tempBuffer[100] = {0};
String cmdString = "S0000", istring = "N0000";
int pos = 0;
float spd = 0.0;
int timeout = 0;

void clearTempBuffer() {
  for(int i=0; i<100; i++) {
    tempBuffer[i] = 0;
    }
}

void serialEvent() {
  if (!Serial.available()) { return; }
  digitalWrite(13, HIGH);
  pos = 0;
  while (Serial.available() && pos < 100) {
    tempBuffer[pos]=(char)Serial.read();
    pos += 1;
  }
  istring = tempBuffer;
  for (int i = 0; i < istring.length(); i++) {
    switch(istring[i]) {
    case 'S':
    case 'F':
    case 'B':
    case 'L':
    case 'R':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      break;
    default:
      istring[i] = '0';
    }
  }
  if (istring.length() >= 5) {
    cmdString = istring;
    spd = ( (float)atoi(cmdString.substring(1, 5).c_str()) ) / 1023;
  }
  digitalWrite(13, LOW);
}

void loop() {
  switch (cmdString[0]) {
  // Move forward
  case 'F':
  case 'f':
    forward(spd);
    break;
  // Move back
  case 'B':
  case 'b':
    backward(spd);
    break;
  // Move left
  case 'L':
  case 'l':
    left(spd);
    break;
  // Move right
  case 'R':
  case 'r':
    right(spd);
    break;
  // No action or hold
  case 'N':
  case 'n':
    break;
  // Stop
  case 'S':
  case 's':
    halt();
    break;
  default:
    break;
  }
  delay(100);
}