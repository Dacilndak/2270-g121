char  tempBuffer1[100]={0};
char  tempBuffer2[100]={0};
char  cmdBuffer[12]={0};
char  cmd[4]={0};
char  CMD;
int   pos=1;
int   Vel;
boolean LINK;

void clearBuffer(int k) {
  switch (k) {
  case 0:
    for(int i=0;i<100;i++){
      tempBuffer1[i]=0;
      tempBuffer2[i]=0;
    }
    break;
  case 1:
    for(int i=0;i<100;i++){
      tempBuffer1[i]=0;
    }
    break;
  case 2:
    for(int i=0;i<100;i++){
      tempBuffer2[i]=0;
    }
  }
}

void clearcmdBuffer(void){
  int i;
  for(i=0;i<12;i++){
    cmdBuffer[i]=0;
    }
}

void readtoBuffer(void){
  while(Serial2.available()) {          //read message to buffer returned by AT+CIFSR
    tempBuffer2[pos-1]=(char)Serial2.read();
    pos += 1;
  }
  pos = 1;
  Serial2.clear();
  while(Serial1.available()) {          //read message to buffer returned by AT+CIFSR
    tempBuffer1[pos-1]=(char)Serial1.read();
    pos += 1;
  }
  pos = 1;
  Serial1.clear();
}

boolean joined = false, tcp_connected = false;

void setup(){
  delay(300);
  Serial1.println("AT+CWMODE=1"); // set as client mode
  Serial1.flush();
  delay(500);
  Serial1.clear();
  delay(500);
  Serial1.println("AT+CIPMUX=1"); //allow multiple conections
  Serial1.flush();
  delay(500);
  Serial1.clear();
  delay(500);
  Serial1.println("AT+CWJAP=\"KATHY\",\" \""); //join TX wifi
  Serial1.flush();
  delay(500);
  Serial1.clear();
  delay(500);
  
  do {
    Serial1.println("AT+CWJAP=\"KATHY\",\"robo\""); //join TX wifi
    Serial1.flush();
    delay(500);
    readtoBuffer();
    if((tempBuffer1[27]==75)&&(tempBuffer1[26]==79)){
      clearBuffer(1);
      Serial1.clear();
      delay(500);
      joined = true;
    } else {
      Serial1.clear();
      clearBuffer(1); 
      delay(500);
      joined = false;
    }
  } while (!joined);
  
  do {
    Serial1.println("AT+CIPSTART=0,\"TCP\",\"192.168.4.1\",1336"); //join TX wifi
    Serial1.flush();
    delay(500);
    readtoBuffer();
    if((tempBuffer1[27]==75)&&(tempBuffer1[26]==79)&&(tempBuffer1[26]==79)&&(tempBuffer1[26]==79)){ //if AT+CWJAP returned "OK" & "Linked" continue
      LINK=true;
    }
    else{
      LINK=false;
    }
    clearBuffer(1);
    if(!LINK){
      Serial1.clear();
      delay(500);
      tcp_connected = false;
    } else { tcp_connected = true; }
  } while (!tcp_connected);
  
  Serial1.clear();
  delay(500);
}

void loop(){
   readtoBuffer();
   int i;
   if((tempBuffer1[2]==43)&&(tempBuffer1[5]==68)) { //checks if message is a TCP message if so reads to cmdBuffer
     for(i = 0; i<7; i++) {
       cmdBuffer[i] = tempBuffer1[i+11];
     }
     if(cmdBuffer[i] == 0){
       i=4;
     }
     clearBuffer(1);
     clearcmdBuffer();
   }
   if(CMD==83){
   //stop
   }
   if(CMD==70){
   //forward
   }
   if(CMD==66){
   //back
   }
   if(CMD==76){
   //left
   }
   if(CMD==82){
   //right
   }
   CMD=0;
   for(int i=0;i<3;i++){
     Serial2.println(cmdBuffer);
     Serial2.flush();
     delay(500);
     readtoBuffer();
     if((tempBuffer2[0]==79)&&(tempBuffer2[1]==75)) {
       i=3;
       clearcmdBuffer();
       clearBuffer(2);
     } else { clearBuffer(2); }
     delay(500);
  }
}
