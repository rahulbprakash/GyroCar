int rrf1=A0;
int rrf2=A1;
int rrf3=A3;

#define ledPin 10
#define ledPin2 3
#define speakerPin 12
int motor_left[] = {4, 5};
int motor_right[] = {7, 8};

void setup()
{
  Serial.begin(9600);

  pinMode(rrf1,INPUT);
  pinMode(rrf2,INPUT);
  pinMode(rrf3,INPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  int i;
  for(i = 0; i < 2; i++)
  {
    pinMode(motor_left[i], OUTPUT);
    pinMode(motor_right[i], OUTPUT);
  }
}

void loop()
{
  int value=analogRead(rrf1);
  int value2=analogRead(rrf2);
  int value3=analogRead(rrf3);
  if(value > 160 && value2 > 160 && value3<160)
  {
    drive_forward();
    play_tone();
  }
  else if(value < 160 && value2< 160 && value3<160)
  {
    drive_backward();
  }
  else if(value<160 && value2 >160 && value3<160)
  {
    turn_right();
  }
  else if(value>160 && value2 < 160 && value3<160)
  {
    turn_left();
  }
  else if(value3>160)
  {
    motor_stop();
  }
}

void motor_stop()
{
  digitalWrite(motor_left[0], LOW);
  digitalWrite(motor_left[1], LOW);
  digitalWrite(motor_right[0], LOW);
  digitalWrite(motor_right[1], LOW);
  noTone(speakerPin);
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin2, LOW);
}

void drive_forward()
{
  digitalWrite(motor_left[1], HIGH);
  digitalWrite(motor_left[0], LOW);
  digitalWrite(motor_right[1], HIGH);
  digitalWrite(motor_right[0], LOW);
  noTone(speakerPin);
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin2, LOW);
}

void drive_backward()
{
  digitalWrite(motor_left[1], LOW);
  digitalWrite(motor_left[0], HIGH);
  digitalWrite(motor_right[1], LOW);
  digitalWrite(motor_right[0], HIGH);
}

void turn_left()
{
  digitalWrite(motor_left[1], LOW);
  digitalWrite(motor_left[0], HIGH);
  digitalWrite(motor_right[1], HIGH);
  digitalWrite(motor_right[0], LOW);
  noTone(speakerPin);
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin2, LOW);
}

void turn_right()
{
  digitalWrite(motor_left[1], HIGH);
  digitalWrite(motor_left[0], LOW);
  digitalWrite(motor_right[1], LOW);
  digitalWrite(motor_right[0], HIGH);
  noTone(speakerPin);
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin2, LOW);
}

void play_tone()
{
  tone(speakerPin,494);
  digitalWrite(ledPin, HIGH);
  digitalWrite(ledPin2, LOW);
  delay(250);
  noTone(speakerPin);
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin2, HIGH);
  delay(250);
}
