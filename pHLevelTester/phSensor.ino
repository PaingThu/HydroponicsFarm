
#define SensorPin A0          // the pH meter Analog output is connected with the Arduino’s Analog
#define BASE 13               // red
#define PERFECT 2             // green
#define ACID 4                // yellow
#define BUZZER 3              // buzzer
unsigned long int avgValue;   //Store the average value of the sensor feedback
float b;
int buf[10], temp, count=0;
boolean start = true;
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
  while(count<30){
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
  analysePhLevel();
  Serial.print("PH Value :");
  Serial.println(phValue,2);
  switchOnLed();
  delay(4000);
}

void analysePhLevel(){
  if(led == 'b'){
    Serial.print("Not Good. This PH Level will damage root of plant.");
  }
  if(led == 'a')
    Serial.print("Not Good. This PH Level is toxic for plant. Too much acid.");
  if(led == 'p'){
    if(phValue < 5.8 || phValue > 6.2)
      Serial.print("It is Good for Plant.");
    else
      Serial.print("Greate!!! It is Perfect for Plant.");
  }
}

void defineOutputPin(){
  pinMode(BASE, OUTPUT);
  pinMode(PERFECT, OUTPUT);
  pinMode(ACID, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void initSetup(){
  Serial.println("Ready");    //Test the serial monitor
  digitalWrite(BASE, HIGH);
  digitalWrite(PERFECT, HIGH);
  digitalWrite(ACID, HIGH);
  digitalWrite(BUZZER, LOW);
  delay(500);
  digitalWrite(BASE, LOW);
  digitalWrite(PERFECT, LOW);
  digitalWrite(ACID, LOW);
  digitalWrite(BUZZER, HIGH);
}

void getSampleValue() {
  for (int i = 0; i < 10; i++) //Get 10 sample value from the sensor for smooth the value
  {
    buf[i] = analogRead(SensorPin);
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
  if(tmpValue != phValue) tmpValue = phValue;
  else count++;
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
