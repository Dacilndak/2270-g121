#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, 9, 10, 8);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define STPIN 18
#define FWPIN 17
#define BKPIN 21
#define RTPIN 19
#define LTPIN 20

const int baudrate=9600;
boolean broadcasting=false;
boolean LINK=true;
boolean EXTIN=true;
char  tempBuffer[100]={0};
char  IP[13]={0};
String ipstring;
String Speed;
char  msgBuffer[13]={0};
int   pos=1;
int FLEX;
int Vel;
int fw;
int bk;
int tilt;
char last = 'n';

int ST_button = 0, FW_button = 0, BK_button = 0, LT_button = 0, RT_button = 0;

void sort(void){
  if((580<tilt) & (tilt<640) & (fw>600) & (bk>650)){
    ST();
    last = 's';
  }
  else if((fw<=550) & (bk>650) & (580<tilt) & (tilt<640)){
    FW();
    last = 'f';
  }
  else if((bk<=600)&(fw>600)&(580<tilt)&(tilt<640)){
    BK();
    last = 'b';
  }
  else if((tilt>=670)&(fw>600)&(bk>650)){
    LT();
    last = 'l';
  }
  else if((tilt<=550)&(fw>600)&(bk>650)){
    RT();
    last = 'r';
  }
  else{ ST(); last = 's'; }
}

void ST_trigger() { EXTIN = !EXTIN; }

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
void linkstatus(void){
  if((tempBuffer[0]==76)&&(tempBuffer[3]==107)){ //checks for "Link" message
      LINK=true;
      clearBuffer();
    }
    if((tempBuffer[0]==85)&&(tempBuffer[5]==107)){ //checks for "Unlink" message
      LINK=false;
      clearBuffer();
    }
}
void  NC(void){
  Serial1.println("AT+CIPSEND=0,7");
  Serial1.flush();
  if (Serial1.find(">")) {
    Serial1.println("N0000");
    Serial1.flush();
  }
  Serial.find("OK");
  Serial1.clear();
}
void  FW(void){
  Serial1.println("AT+CIPSEND=0,7");
  Serial1.flush();
    if (Serial1.find(">")) {
    Serial1.print("F");
    Serial1.print((Vel/1000)%10);
    Serial1.print((Vel/100)%10);
    Serial1.print((Vel/10)%10);
    Serial1.println(Vel%10);
    Serial1.flush();
  }
  Serial1.find("OK");
  Serial1.clear();
}
void  BK(void){
  Serial1.println("AT+CIPSEND=0,7");
  Serial1.flush();
  if (Serial1.find(">")) {
    Serial1.print("B");
    Serial1.print((Vel/1000)%10);
    Serial1.print((Vel/100)%10);
    Serial1.print((Vel/10)%10);
    Serial1.println(Vel%10);
    Serial1.flush();
  }
  Serial1.find("OK");
  Serial1.clear();
}
void  RT(void){
  Serial1.println("AT+CIPSEND=0,7");
  Serial1.flush();
  if (Serial1.find(">")) {
    Serial1.print("R");
    Serial1.print((Vel/1000)%10);
    Serial1.print((Vel/100)%10);
    Serial1.print((Vel/10)%10);
    Serial1.println(Vel%10);
    Serial1.flush();
  }
  Serial1.find("OK");
  Serial1.clear();
}
void  LT(void){
  Serial1.println("AT+CIPSEND=0,7");
  Serial1.flush();
  if (Serial1.find(">")) {
    Serial1.print("L");
    Serial1.print((Vel/1000)%10);
    Serial1.print((Vel/100)%10);
    Serial1.print((Vel/10)%10);
    Serial1.println(Vel%10);
    Serial1.flush();
  }
  Serial1.find("OK");
  Serial1.clear();
}
void  ST(void){
  Serial1.println("AT+CIPSEND=0,7");
  Serial1.flush();
  if (Serial1.find(">")) {
    Serial1.println("S0000");
    Serial1.flush();
  }
  Serial1.find("OK");
  Serial1.clear();
}
void setup(){
analogWrite(6, 140);
  pinMode(14,INPUT);
  pinMode(15,INPUT);
  pinMode(16,INPUT);
  pinMode(23,INPUT);
  display.clearDisplay();
  display.begin();
  display.setContrast(60);
  display.setRotation(2);
  delay(100);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  
  pinMode(STPIN, INPUT);
  pinMode(FWPIN, INPUT);
  pinMode(LTPIN, INPUT);
  pinMode(RTPIN, INPUT);
  pinMode(BKPIN, INPUT);
  
  attachInterrupt(STPIN, ST_trigger, FALLING);
  
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
    int i;
    for(i=0;i<11;i+=1){
    IP[i]=tempBuffer[i+11];
    }
    clearBuffer();
  }
  else{
  }
  delay(300);
  ipstring= String(IP);
}
void loop()
{
  readtoBuffer();
  delay(50);
  //linkstatus();
  if(EXTIN) {
    FLEX=analogRead(22);
    if(FLEX<250 ){
      Vel=341;
    }
    else if(FLEX>650) {
      Vel=1023;
    }
    else {
      Vel=682;
    }
  }
  else {
    Vel= analogRead(23);
    FLEX=0;
  }
  
  fw = analogRead(14);
  bk = analogRead(15);
  tilt = analogRead(16);

  if(LINK) {
    display.clearDisplay();
    display.println("Speed:"+String(Vel/10.23)+"%");
    if (EXTIN) {
      display.println("EMG");
    } else {
      display.println("Pot");
    }
    display.println(fw);
    display.println(bk);
    display.println(tilt);
    display.println(last);
    display.display();
    sort();
  }
  else {
    display.clearDisplay();
    display.println(ipstring);
    display.println("Waiting... ");
    display.display();
  }
}