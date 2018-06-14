#include <TimeLib.h>
#include <LiquidCrystal.h>
#include <pitches.h>

#define buzzerID 0 // Signal port for buzzer
#define scaler 1.0 // Scaler for note playing

const int redButton = 2;     // Corresponds with the button with a red signal wire
const int yellowButton = 3;  // Corresponds with the button with a yellow signal wire
const int greenButton = 4;   // Corresponds with the button with a green signal wire
const int blueButton = 5;    // Corresponds with the button with a blue signal wire
const int whiteButton = 6;   // Corresponds with the button with a white signal wire

const int ledPort = 0; // Port for the LED

// Defines the LCD
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
const LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int score = 0; // Keeps count of score

// Various Songs
int marySong[] = {3, 2, 1, 2, 3, 3, 3, 2, 2, 2, 3, 5, 5, 3, 2, 1, 2, 3, 3, 3, 3, 2, 2, 3, 2, 1};
int hotCrossSong[] = {3, 2, 1, 3, 2, 1, 3, 3, 3, 2, 2, 2, 3, 2, 1};
int scale[] = {1, 2, 3, 4, 5, 5, 4, 3, 2, 1};
int jingle[] = {3, 3, 3, 3, 3, 3, 3, 5, 1, 2, 3, 4, 4, 4, 4, 3, 3, 3, 3, 3, 2, 2, 3, 2, 5};
int spider[] = {5, 1, 1, 1, 2, 3, 3, 3, 2, 3, 1, 3, 3, 4, 5, 5, 4, 3, 4, 5, 3, 1, 1, 2, 3, 3, 2, 1, 2, 3, 1, 5, 5, 1, 1, 1, 2, 3, 3, 3, 2, 1, 2, 3, 1};

// An array that saves the letter values of the notes
const String noteValues[] = {"C", "D", "E", "F", "G"};

void setup() {
  // Set each button as an input
  pinMode(redButton, INPUT_PULLUP);
  pinMode(yellowButton, INPUT_PULLUP);
  pinMode(greenButton, INPUT_PULLUP);
  pinMode(blueButton, INPUT_PULLUP);
  pinMode(whiteButton, INPUT_PULLUP);

  pinMode(ledPort, OUTPUT); // Set the LED port to an output
  digitalWrite(1, LOW);     // Initially set the LED off (LOW)
  lcd.begin(16, 2);         // Begin the LCD display
}

void loop() {
  mainInterface(); // Run the main interface
}

void playSong(int *song) {   // this function plays the song passed in as a parameter
  bool correctPress = false; // set correctPress to false since the user hasn't hit the right button yet
  bool turn = false;         // set turn to false since the user hasn't taken a turn yet
  for (int i = 0; i < sizeof(song) / sizeof(song[0]) ; i++) { // loop through the notes of the song
    lcd.setCursor(0, 0);                                      // set the location of the cursor
    lcd.print("Note # :" + (String) (i + 1) + " - " + (String) noteValues[song[i] - 1] + "   "); // shows the current note on the LCD
    double currentMill = millis();                                                               // gets current time
    while (millis() - currentMill < 2000) {                           // this loop acts as a timer, since it will run until about 2 seconds has passed.
      if ((digitalRead(song[i] + 1) == LOW) and oneButtonPressed()) { // check if the user hit the correct button, and only one button was pressed at the time
        playNote(song[i]);     // play the note on the buzzer
        digitalWrite(1, HIGH); // turn the LED on
        correctPress = true;   // set correctPress to true
        break;                 // break out of this time loop
      }
      turn = true;             // the user has take their turn, so set turn to true
    }
    if (turn) {             // if the user took their turn
      lcd.setCursor(0, 1);  // set the location of the cursor to the second row
      if (correctPress) {   // if the user pressed the correct button
        score += 1;         // add one to their score
        lcd.print("Correct!   "); // print "Correct!" to the LCD
        correctPress = false;     // reset correctPress to false
      } else {                    // otherwise, they didn't press the right button
        lcd.print("Incorrect!");  // print "Incorrect!" to the LCD
        digitalWrite(1, LOW);     // turn the LED off
      }
      turn = false;               // reset turn to false
    }
  }
  double currentMill = millis();          // get current time in milliseconds
  while (millis() - currentMill < 2000) { // this loop acts as a timer, since it will run until about 2 seconds has passed.
    lcd.print("Song Finished!");          // print that the song has finished to the LCD
    lcd.setCursor(0, 1);                  // set the position of the cursor to the second row
    lcd.print("Final Score: " + (String) score + "/" + (String) (sizeof(song) / sizeof(song[0]))); // show the final score on the LCD
  }
  lcd.setCursor(0, 0);                                                                            // set the cursor to the first row
  lcd.print("Final Score: " + (String) score + "/" + (String) (sizeof(song) / sizeof(song[0])));  // show the final score on the LCD
  lcd.setCursor(0, 1);            // set the cursor to the second row
  lcd.print("Awaiting restart."); // print "Awaiting restart." to the LCD
  while (true) {                  // runs until a button was pressed
    if (anyButtonPressed()) {     // checks if a button was pressed
      score = 0;                  // resets score
      mainInterface();            //returns to main interface
    }
  }
}

// This function is the main interface of the program
void mainInterface() {
  lcd.setCursor(0, 0);  // Set the location of the cursor to the start of the first tow
  lcd.print("Select");  // Print select to the LCD
  if (oneButtonPressed()) { // Check if only one button was pressed
    // The following code plays a song based on the button pressed. 
    if (digitalRead(redButton) == LOW) {
      playSong(marySong);
    } else if (digitalRead(yellowButton) == LOW) {
      playSong(hotCrossSong);
    } else if (digitalRead(greenButton) == LOW) {
      playSong(scale);
    } else if (digitalRead(blueButton) == LOW) {
      playSong(jingle);
    } else if (digitalRead(whiteButton) == LOW) {
      playSong(spider);
    }
  }
}

// This function plays a note on a buzzer
void playNote(int note) {
  int noteID = 0; // holds the noteID that will be changed in the switch case
  float duration = 3000 / 6; // how long to play the note for
  switch (note) { // switch case that determines which note to play based on the note passed into the function
    case 1:       // plays a low C
      noteID = NOTE_C4;
      break;
    case 2:       // plays a low D
      noteID = NOTE_D4;
      break;
    case 3:       // plays a low E
      noteID = NOTE_E4;
      break;
    case 4:       // plays a low F
      noteID = NOTE_F4;
      break;
    case 5:       // plays a low G
      noteID = NOTE_G4;
      break;
  }
  tone(buzzerID, noteID, duration); // play the note on the buzzer
  delay(duration * scaler);         // add a delay so the note sounds proper
}

// This function checks if any button has been pressed
bool anyButtonPressed() {
  return digitalRead(redButton) == LOW or digitalRead(yellowButton) == LOW or digitalRead(greenButton) == LOW or digitalRead(blueButton) == LOW or digitalRead(whiteButton) == LOW;
}

// This function makes sure only one button was pressed (uses XOR)
bool oneButtonPressed() {
  return digitalRead(redButton) == LOW ^ digitalRead(yellowButton) == LOW ^ digitalRead(greenButton) == LOW ^ digitalRead(blueButton) == LOW ^ digitalRead(whiteButton) == LOW;
}

