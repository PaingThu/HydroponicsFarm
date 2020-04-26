
#include <SoftwareSerial.h>

#define SENSORPIN A0          // the pH meter Analog output is connected with the Arduino’s Analog A0
#define BASE 13               // red
#define PERFECT 2             // green
#define ACID 4                // yellow
#define BUZZER 3              // buzzer
unsigned long int avgValue;   // to store the average value of the sensor feedback
int buf[10], temp, sameValueCount=0;
boolean start = true, analyseAgain;
float phValue,tmpValue = 0.0;
char led;

void setup()
{
  defineOutputPin();
  Serial.begin(9600);
  initSetup();
}

void loop()
{
  while(sameValueCount < 30){
    start = true;
    getSampleValue();
    getSortedValue();
    getPhValue();
    definePHLevel();
  }
  if(start){
    for(int i = 0; i < 200; i=i+20){
      digitalWrite(BUZZER, LOW);
      delay(100);
      digitalWrite(BUZZER, HIGH);
      delay(100);
    }

    Serial.println("PH Level Analysing.....");
    delay(3000);
    start = false;
  }
  switchOnLed();
  analysePhLevel();
  //current pH level is shown only three time 
  if(analyseAgain) {
    sameValueCount = 0;    //reset sameValueCount
    Serial.println("pH meter will restart again.");
    delay(3000);
  }else{
    for(int i = 0 ;i < 10; i++){
      Serial.println("Switch off your device or press reset button to restart again.");
      delay(10000);
    }
  }
}


void defineOutputPin(){
  pinMode(BASE, OUTPUT);
  pinMode(PERFECT, OUTPUT);
  pinMode(ACID, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void initSetup(){
  digitalWrite(BASE, HIGH);
  digitalWrite(PERFECT, HIGH);
  digitalWrite(ACID, HIGH);
  digitalWrite(BUZZER, LOW);
  delay(500);
  digitalWrite(BASE, LOW);
  digitalWrite(PERFECT, LOW);
  digitalWrite(ACID, LOW);
  digitalWrite(BUZZER, HIGH);
  
  Serial.println("Welcome to pH Level Analysing Department of MPT's Hydroponics Home Farm");    //Test the serial monitor
  delay(5000);
  Serial.println("Analysing pH Level will be started soon");
  delay(3000);
  
}

void getSampleValue() {
  for (int i = 0; i < 10; i++) //Get 10 sample value from the sensor for smooth the value
  {
    buf[i] = analogRead(SENSORPIN);
    delay(10);
  }
}

void getSortedValue(){
  for (int i = 0; i < 9; i++) //sort the analog from small to large
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buf[i] > buf[j])
      {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
}

void getPhValue(){
  avgValue = 0;
  for (int i = 2; i < 8; i++)               //take the average value of 6 center sample
    avgValue += buf[i];
  phValue = (float)avgValue * 5.0 / 1024 / 6; //convert the analog into millivolt
  phValue = 3.5 * phValue;
  Serial.print("    pH:");
  Serial.print(phValue, 2);
  Serial.println(" ");//convert the millivolt into pH value
}

void definePHLevel(){
  if(tmpValue == phValue) sameValueCount++; //increase the same value count
  else tmpValue = phValue;
  
  if (phValue > 6.5) {
    led = 'b';
    switchOnLed();
    delay(10);
    digitalWrite(BASE, LOW);

  } else if (phValue < 5.5) {
    led = 'a';
    switchOnLed();
    delay(10);
    digitalWrite(ACID, LOW);

  } else {
    led = 'p';
    switchOnLed();
    delay(10);
    digitalWrite(PERFECT, LOW);

  }
}


void analysePhLevel(){
  Serial.print("Current pH Value of water is ");
  Serial.print(phValue,2);
  Serial.println(".");
  delay(2000);
  if(led == 'b'){
    Serial.println("Not Good!");
    delay(3000);
    Serial.println("This pH Level will damage root of plant.");
    delay(6000);
    analyseAgain = true;
  }
  if(led == 'a'){
    Serial.println("Not Good!");
    delay(3000);
    Serial.println("This pH Level is toxic for plant. Too much acid.");
    delay(6000);
    analyseAgain = true;
  }
  if(led == 'p'){
    if(phValue < 5.8 || phValue > 6.2){
      Serial.println("It is Good for Plant.");
      delay(3000);
      Serial.println("Congratulation! You got good pH Level for your plant.");
    }
    else{
      Serial.println("Greate!!! It is Perfect for Plant.");
      delay(3000);
      Serial.println("Congratulation! You got best pH Level for your plant.");
    }
    delay(6000);
    analyseAgain = false;
  }
}

void switchOnLed(){
  if(led == 'p'){
    digitalWrite(PERFECT, HIGH);
    digitalWrite(BASE, LOW);
    digitalWrite(ACID, LOW);
  }
  if(led == 'b'){
    digitalWrite(PERFECT, LOW);
    digitalWrite(BASE, HIGH);
    digitalWrite(ACID, LOW);
  }
  if(led == 'a'){
    digitalWrite(PERFECT, LOW);
    digitalWrite(BASE, LOW);
    digitalWrite(ACID, HIGH);
  }
}
