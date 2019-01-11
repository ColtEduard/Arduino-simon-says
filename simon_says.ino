int buttons[] = {3, 4, 5, 6}; //The four push button pins
int leds[] = {8, 9, 10, 11}; //The LED pins

// the sequence of LEDs
int sequence[100];

// current round
int largestIndex = 0;

const int START = 0;
const int PLAY = 1;
const int GAMEOVER = 2;

// time in player has to press a button (ms)
int speed = 1000;

int gameState;
boolean running = true;

long currentMillis = 0;
long previousMillis = 0;

// setup routine runs once when you press reset
void setup()
{  
  for(int pin=0; pin<4; pin++)
  {
  	pinMode(leds[pin], OUTPUT);
  }
  
  for(int pin=0; pin<4; pin++)
  {
  	pinMode(buttons[pin], INPUT);
  }
  
    //Setup serial communications through the USB
    Serial.begin(9600);
    
    // Start the game with the START.
    gameState = START; 
    
    // reads noise from an unused analog pin
    // initializes the psude-random number generator
    // the random will be different each time the code is run
    randomSeed(analogRead(40));
}


// the loop routine runs over and over again forever:
void loop() 
{
  if(gameState == START)
  {
    Serial.println("Waiting to start");
  	waitToStart();
  }
  else if(gameState == PLAY)
  {
    Serial.println("Playing"); 
    showSequence();
    readSequence();
  }
  else if(gameState == GAMEOVER)
  {
    Serial.println("Game Over");
    blinkAll(3);
    // restarts the game
    gameState = START;
  }
}

void waitToStart()
{
  int buttonPressed = -1;
  for(int pin = 0; pin < 4; pin++)
  {
    if(digitalRead(buttons[pin]) == HIGH)
      {
        buttonPressed == pin;
      }
      
    if(buttonPressed == -1)
    
      // plays a random pattern of lights while waiting to start
      digitalWrite(leds[pin], HIGH);
      buttonPressed = waitForButton(500);
      digitalWrite(leds[pin], LOW);
  }
  
  
  if(buttonPressed != -1)
  {
    // A button was pushed so wait then start playing.
    delay(2000);
    largestIndex = 0; // Restart
   	gameState = PLAY; 
  }
}


void readSequence()
{
  
  int positionPressed;
  boolean madeMistake = false;
  
  for(int index=0; (index<largestIndex) & (madeMistake == false); index++)
  {
    Serial.println(""); 
    Serial.println("Should push"); 
    Serial.println(sequence[index]);
    
    positionPressed = waitForButton(speed); // 0, 1, 2, or 3

    Serial.println("Pressed"); 
    Serial.println(positionPressed); 
    if((positionPressed == -1) | (positionPressed != sequence[index]))
    {
      	madeMistake = true; // Exit the loop.
		gameState = GAMEOVER;
    }
  }
}

void showSequence()
{
  sequence[largestIndex] = random(0,4);
  Serial.println("Next out"); 
  Serial.println(sequence[largestIndex]); 
  largestIndex++;
  
  for(int index=0; index<largestIndex; index++)
  {
     delay(300);
   	 digitalWrite(leds[sequence[index]], HIGH);
     delay(500);
     digitalWrite(leds[sequence[index]], LOW);
  }
}




// Returns the position of Button pressed (0, 1, 2, or 3) or -1 if no button is pressed in the time period.
int waitForButton(int delay)
{
  int buttonPressed = -1;
  int input;
  boolean buttonBackUp = false;
  
  currentMillis = millis();  		// The number of ms since the program started running
  previousMillis = currentMillis; 	// Records the point when we start spinning the loop.
  
  // Keep spinning the loop until "delay" seconds have passed.
  while (((currentMillis - previousMillis) < delay) & (buttonBackUp == false))
  {
    // Read the button and record when it has been pushed down.
    for(int pin = 0; (pin < 4) & (buttonBackUp == false); pin++)
    {
      if(digitalRead(buttons[pin]) == HIGH)
      {
        buttonPressed = pin;
        
        // Show the LED pushed.
        digitalWrite(leds[pin], HIGH);
              
        // It is possible the button is still being pushed.
        // This loop spins until the button is let up.
        while (((currentMillis - previousMillis) < delay) & (buttonBackUp == false))
        {
          input = digitalRead(buttons[pin]);
          if(input == LOW)
          {
            buttonBackUp = true;
          }
          currentMillis = millis();
        }
        
        // Turn the LED pushed off.
        digitalWrite(leds[pin], LOW);
        
        // See if they took to long.
        if(currentMillis - previousMillis > delay)
        {
          buttonPressed = -1; // They took to long to let the button up so they lose.
        }
      }
    }

    currentMillis = millis();
  }
  
  return buttonPressed;
}

// Checks if a button was pressed and if it exceeded the delay
boolean readAnyButton(int delay, int button)
{
  boolean buttonDown = false;
  
  currentMillis = millis();  		// The number of ms since the program started running
  previousMillis = currentMillis; 	// Records the point when we start spinning the loop.
  
  // Keep spinning the loop until "delay" seconds have passed.
  while (((currentMillis - previousMillis) < delay) & (buttonDown == false))
  {
    // Read the button and record when it has been pushed down.
    for(int pin = 0; pin < 4; pin++)
    {
      if(digitalRead(buttons[pin]) == HIGH)
      {
        buttonDown = true;
      }
    }
    // currentMillis = millis();
  }
  return buttonDown;
}





// Turns all LEDs off
void allOff()
{
  for(int pin = 0; pin < 4; pin++)
  {
    digitalWrite(leds[pin], LOW);
  }
}

// Turns all LEDs on
void allOn()
{
  for(int pin = 0; pin < 4; pin++)
  {
    digitalWrite(leds[pin], HIGH);
  }
}

void blinkAll(int times)
{
  for(int count = 0; count < times; count++)
  {
    allOn();
    delay(250);
    allOff();
    delay(250);
  } 
}

void blinkRed(int times)
{
  for(int count = 0; count < times; count++)
  {
    digitalWrite(leds[0], HIGH);
    delay(300);
    digitalWrite(leds[0], LOW);
    delay(300);
  } 
}

void blinkYellow(int times)
{
  for(int count = 0; count < times; count++)
  {
    digitalWrite(leds[1], HIGH);
    delay(300);
    digitalWrite(leds[1], LOW);
    delay(300);
  } 
 } 
 
 
void blinkBlue(int times)
{
  for(int count = 0; count < times; count++)
  {
    digitalWrite(leds[2], HIGH);
    delay(300);
    digitalWrite(leds[2], LOW);
    delay(300);
  } 
}

void blinkGreen(int times)
{
  for(int count = 0; count < times; count++)
  {
    digitalWrite(leds[3], HIGH);
    delay(300);
    digitalWrite(leds[3], LOW);
    delay(300);
  } 
}

void blinkLed(int pin, int times)
{
    for (int count = 0; count < times; count++)
    {
        digitalWrite(leds[pin], HIGH);
        delay(300);
        digitalWrite(leds[pin], LOW);
        delay(300);
    }
}
