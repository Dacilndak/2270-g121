void clear_serial()
{
  while (Serial2.available()) { Serial2.read(); }
}

void clear_serial3()
{
  while (Serial3.available()) { Serial2.print((char)Serial3.read()); }
}

void esp_disconnect()
{
  digitalWrite(13, HIGH);
  Serial3.flush();
  Serial3.println("AT+CIPCLOSE");
  Serial3.flush();
  Serial2.flush();
  delay(200);
  clear_serial3();
  digitalWrite(13, LOW);
  
  delay(100);
  
  digitalWrite(13, HIGH);
  Serial3.flush();
  Serial3.println("AT+CWQAP");
  Serial3.flush();
  Serial2.flush();
  delay(200);
  clear_serial3();
  digitalWrite(13, LOW);
}

void esp_config()
{
  digitalWrite(13, HIGH);
  digitalWrite(2, LOW);
  Serial3.flush();
  delay(100);
  digitalWrite(2, HIGH);
  Serial3.println("AT+RST");
  Serial3.flush();
  Serial2.flush();
  delay(1000);
  clear_serial3();
  digitalWrite(13, LOW);
  
  delay(100);
  
  digitalWrite(13, HIGH);
  Serial3.flush();
  Serial3.println("AT+CWMODE=1");
  Serial3.flush();
  Serial2.flush();
  delay(1000);
  clear_serial3();
  digitalWrite(13, LOW);
  
  delay(100);
  
  digitalWrite(13, HIGH);
  Serial3.println("AT+CIPMUX=1");
  Serial3.flush();
  Serial2.flush();
  delay(1000);
  clear_serial3();
  digitalWrite(13, LOW);
}

void esp_connect()
{
  digitalWrite(13, HIGH);
  Serial3.flush();
  Serial3.println("AT+CWJAP=\"KATHY\",\"\"");
  Serial3.flush();
  Serial2.flush();
  delay(5000);
  clear_serial3();
  digitalWrite(13, LOW);
  delay(100);
  digitalWrite(13, HIGH);
  Serial3.println("AT+CIPSTART=0,\"TCP\",\"192.168.4.1\",1336");
  Serial3.flush();
  Serial2.flush();
  delay(5000);
  clear_serial3();
  digitalWrite(13, LOW);
}

void setup()
{
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  pinMode(15, INPUT);
  pinMode(16, INPUT);
  
  pinMode(2, OUTPUT);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  digitalWrite(2, HIGH);
  
  delay(1000);
  
  esp_config();
  delay(100);
  esp_connect();
  
  Serial2.flush();
  Serial3.flush();
}

char inputBuffer[100] = {0};
String cmdString, istring;
int spd = 0;

void readToBuffer()
{
  for (int i = 0; Serial3.available() && i < 100; i++) {
    inputBuffer[i] = (char)Serial3.read();
  }
}

void clearBuffers()
{
  for (int i = 0; i < 100; i++) {
    inputBuffer[i] = 0;
  } 
}

void loop()
{  
  while (Serial2.available()) {
    digitalWrite(13,HIGH);
    Serial3.write(Serial2.read());
  }
  if (digitalRead(15)) { esp_disconnect(); esp_config(); }
  if (digitalRead(16)) { esp_disconnect(); esp_connect(); }
  while (Serial3.available()) {
    // Serial2.write(Serial3.read());
    readToBuffer();
    // checks if message is a TCP message
    // if so reads to cmdBuffer
    istring = inputBuffer;
    istring = istring.replace("\n","").replace("\r","").replace("\0","");
    istring.append("\0");
    if(istring.indexOf("+") != -1 && istring.indexOf(":") != -1) {
      cmdString = istring.substring(istring.indexOf(":") + 1, istring.length() - 1);
      spd = atoi(cmdString.substring(1, 5).c_str());
    } else {
      istring = "";
      cmdString = "";
    }
  }
  switch (cmdString[0]) {
  case 'S':
    Serial2.print("S");
    Serial2.print(spd / 1000);
    Serial2.print((spd / 100) % 10);
    Serial2.print((spd / 10) % 10);
    Serial2.println((spd / 1) % 10);
    break;
  case 'F':
    Serial2.print("F");
    Serial2.print(spd / 1000);
    Serial2.print((spd / 100) % 10);
    Serial2.print((spd / 10) % 10);
    Serial2.println((spd / 1) % 10);
    break;
  case 'B':
    Serial2.print("B");
    Serial2.print(spd / 1000);
    Serial2.print((spd / 100) % 10);
    Serial2.print((spd / 10) % 10);
    Serial2.println((spd / 1) % 10);
    break;
  case 'R':
    Serial2.print("R");
    Serial2.print(spd / 1000);
    Serial2.print((spd / 100) % 10);
    Serial2.print((spd / 10) % 10);
    Serial2.println((spd / 1) % 10);
    break;
  case 'L':
    Serial2.print("L");
    Serial2.print(spd / 1000);
    Serial2.print((spd / 100) % 10);
    Serial2.print((spd / 10) % 10);
    Serial2.println((spd / 1) % 10);
    break;
  default:
    if (istring.length() > 1) { Serial2.println(istring); }
    break;
  }
  if (cmdString.length() > 1) { Serial.println(istring); }
  Serial.flush();
  istring = "";
  cmdString = "";
  spd = 0;
  clearBuffers();
  Serial3.flush();
  Serial2.flush();
  digitalWrite(13,LOW);
  delay(50);
}