#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, 9, 10, 8);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

const int baudrate=9600;
boolean broadcasting=false;
boolean LINK=false;
boolean EXTIN=false;
char tempBuffer[100]={0};
char IP[13]={0};
String ipstring;
String Speed;
char msgBuffer[13]={0};
int pos=1;
int Vel;
int fw[2];
int bk[2];
int tilt[2];

void clearBuffer(void){
  int i;
  for(i=0;i<100;i++){
    tempBuffer[i]=0;
    }
}

void clearmsgBuffer(void){
  int i;
  for(i=0;i<12;i++){
    msgBuffer[i]=0;
    }
}
  
void readtoBuffer(void){
  while(Serial1.available()) {        //keep reading RX to buffer
    tempBuffer[pos-1]=(char)Serial1.read();
    pos+=1;  
    }
    Serial1.clear();
    pos=1;    
}

void FW(void){
  int j;
  for(j=0;j<3;j++){
    Serial1.println("AT+CIPSEND=0,7");
    Serial1.flush();
    delay(100);
    Serial1.println("F"+Vel);
    delay(100);
    readtoBuffer();
    if((tempBuffer[0]==79)&&(tempBuffer[1]==75)){ //checks for "OK" message
        clearBuffer();
        j=3;
      } else { clearBuffer(); }
    Serial1.clear();
    delay(500);
  }
}

void BK(void){
  int j;
  for(j=0;j<3;j++){
    Serial1.println("AT+CIPSEND=0,7");
    Serial1.flush();
    delay(100);
    Serial1.println("B"+Vel);
    delay(100);
    readtoBuffer();
    if((tempBuffer[0]==79)&&(tempBuffer[1]==75)){ //checks for "OK" message
      clearBuffer();
      j=3;
    }
    else{ clearBuffer(); }
    Serial1.clear();
    delay(500);
  }
}

void LT(void){
  int j;
  for(j=0;j<3;j++){
    Serial1.println("AT+CIPSEND=0,7");
    Serial1.flush();
    delay(100);
    Serial1.println("L"+Vel);
    delay(100);
    readtoBuffer();
    if((tempBuffer[0]==79)&&(tempBuffer[1]==75)){ //checks for "OK" message
      clearBuffer();
      j=3;
    } else { clearBuffer(); }
    Serial1.clear();
    delay(500);
  }
}

void RT(void){
  int j;
  for(j=0;j<3;j++){
    Serial1.println("AT+CIPSEND=0,7");
    Serial1.flush();
    delay(100);
    Serial1.println("R"+Vel);
    delay(100);
    readtoBuffer();
    if((tempBuffer[0]==79)&&(tempBuffer[1]==75)){ //checks for "OK" message
      clearBuffer();
      j=3;
    }
    else { clearBuffer(); }
    Serial1.clear();
    delay(500);
  }
}

void ST(void){
  int j;
  for(j=0;j<3;j++){
    Serial1.println("AT+CIPSEND=0,7");
    Serial1.flush();
    delay(100);
    Serial1.println("S0000");
    delay(100);
    readtoBuffer();
    if((tempBuffer[0]==79)&&(tempBuffer[1]==75)){ //checks for "OK" message
      clearBuffer();
      j=3;
    } else { clearBuffer(); }
    Serial1.clear();
    delay(500);
  }
}
void SorT(void){
  int j = 0;
  delay(21);
  while(digitalRead(21) == LOW){
    j += 1;
  }
  if(j<3300){
    ST();
  }
  else {
    EXTIN = true;
  }
}
  
void setup(){
  attachInterrupt(17,FW,CHANGE );
  attachInterrupt(21,BK,CHANGE);
  attachInterrupt(19,RT,CHANGE);
  attachInterrupt(20,LT,CHANGE);
  attachInterrupt(18,SorT,CHANGE);
  
  analogWrite(6, 140);
  
  display.clearDisplay();
  display.begin();
  display.setContrast(60);
  display.setRotation(2);
  delay(100);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  
  Serial.begin(baudrate);
  Serial1.begin(baudrate);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  delay(500);
  Serial1.println("AT+CWMODE=2"); // set as AP mode
  Serial1.flush();
  delay(500);
  Serial1.clear();
  delay(500);
  Serial1.println("AT+CIPMUX=1"); //allow multiple conections
  Serial1.flush();
  delay(500);
  Serial1.clear();
  delay(500);
  CWSAP: 
  Serial1.println("AT+CWSAP=\"KATHY\",\" \",1,0"); //allow multiple conections
  Serial1.flush();
  delay(500);
  readtoBuffer();
  if((tempBuffer[29]==79)&&(tempBuffer[30]==75)){ //if AT+CIFSR returned "OK" parse out ip and continue
    clearBuffer();
  }
  else{
    clearBuffer();
    delay(500);
    goto CWSAP;
  }
  
  Serial1.clear();
  delay(500);
  Serial1.println("AT+CIPSERVER=1,1336"); //Start server on port 1336
  Serial1.flush();
  delay(500);
  Serial1.clear();
  delay(500);
  Serial1.println("AT+CIFSR"); //returns IP if running
  Serial1.flush();
  delay(500);
  readtoBuffer();
  if((tempBuffer[27]==75)&&(tempBuffer[26]==79)){ //if AT+CIFSR returned "OK" parse out ip and continue
    broadcasting=true;
    int i;
    for(i=0;i<11;i+=1){
    IP[i]=tempBuffer[i+11];
    }
    clearBuffer();
  }
  else{
    broadcasting=false;
  }
  delay(300);
  ipstring = String(IP);
}

void loop()
{
  readtoBuffer();
  if((tempBuffer[0]==76)&&(tempBuffer[3]==107)){ //checks for "Link" message
    LINK=true;
    clearBuffer();
  }
  if((tempBuffer[0]==85)&&(tempBuffer[5]==107)){ //checks for "Unlink" message
    LINK=false;
    clearBuffer();
  }
  if((tempBuffer[2]==43)&&(tempBuffer[5]==68)){ //checks if message is a TCP message if so reads to msgBuffer     
    int i;
    for(i=0;i<7;i++){
      msgBuffer[i]=tempBuffer[i+11];
    }
    clearBuffer();
  }
  if(EXTIN) {
    Vel= analogRead(22);
    fw[1]=fw[0];
    bk[1]=bk[0];
    tilt[1]=tilt[0];
    fw[0]= analogRead(14);
    bk[0]= analogRead(15);
    tilt[0]= analogRead(16);
    if(Vel>=1023*2/3){
      Speed=String(100);
      Vel=1023;
    }
    if(Vel<=1023/3){
      Speed=String(33);
      Vel=1023/3;
    }
    else{
      Speed=String(66);
      Vel=1023*2/3;
    }
    if((fw[0]&bk[0]&tilt[0])<=200){
      ST();
    }
    if((fw[0]-fw[1])>333){
      FW();
    }
    if((bk[0]-bk[1])>333){
      BK();
    }
    if((tilt[0]-tilt[1])>333){
      RT();
    }
    if((tilt[0]-tilt[1])>-333){
      LT();
    }
  } else {
    Speed=String(analogRead(23)/10.23); 
    Vel=analogRead(23);
  }
  if(LINK){
    display.clearDisplay();
    display.println("Robot-Linked");
    if(EXTIN){
      display.println("EXT-INPUT");
    }
    display.println("Speed:"+Speed+"%");
    display.println();
    display.println(msgBuffer);
    display.display();
  }
  else {
    display.clearDisplay();
    display.println(ipstring);
    display.println("Waiting... ");
    clearmsgBuffer();
    display.display();
  }
  delay(100);
}
