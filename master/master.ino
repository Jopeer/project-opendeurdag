#include <Wire.h>

const int knop = 2;
const int led = 3;

const int gameButtons = 10; // aantal knoppen per game
int gameButtonProgression;

unsigned long currentMillis;
unsigned long previousMillis;
unsigned long MaxplayTime = 30000; // de max tijd om een knop in te drukken [ms]

int Slave_A = 3; // adress van de slave

struct
{
    int ButtonPin[4];
    int button;
    int line;
    int arduino;
    bool wasPressed;
} Button;

void setup()
{
    Serial.begin(9600);
    Wire.begin();

    for (int i = 3; i < 13; i++)
    {
        pinMode(Button.ButtonPin[i], INPUT);
    }
}

void loop()
{
    if (wasButtonPressed(Button.arduino))
    {
        gameButtonProgression += 1;
        randButton();
    }
    if (millis() - previousMillis >= MaxplayTime)
    {
        gameEnd(false);
    }
    if (gameButtonProgression == gameButtons)
    {
        gameEnd(true);
    }
}

void randButton()
{
    int arduino = random(0, 1);
    int line = random(0, 1);
    int button = random(0, 5);

    switch (arduino)
    {
    case 0:

        Button.line = line;
        Button.button = button;
        light(line, button, true);

        break;

    case 1:

        sendButton(arduino, line, button);

        break;
    }
}

void sendButton(int arduino, int line, int button)
{
    Wire.beginTransmission(Slave_A);
    Wire.write(0); // stuur het doel van de boodschap
    Wire.write(line);
    Wire.write(button);
    Wire.endTransmission();
}

bool wasButtonPressed(int arduino)
{
    switch (arduino)
    {
    case 0:
        return Button.wasPressed;

        break;

    case 1:

        Wire.requestFrom(Slave_A, 1);
        while (!Wire.available())
        {
        }
        return Wire.read();

        break;
    }
}

void light(int line, int button, bool ledON)
{
}

void sendReset(int slaveAdress)
{
    Wire.beginTransmission(slaveAdress);
    Wire.write(1); // case 1: reset() (zie slave)
    Wire.endTransmission();
}

void sendGameEnd(bool hasWon, unsigned long completionTime)
{
    Wire.beginTransmission(Slave_A);
    Wire.write(2); // case 2: gameEnd() (zie slave)
    Wire.write(hasWon);
    Wire.write(completionTime);
    Wire.endTransmission();
}

void buttonPressed() // word opgeroepen als de juiste knop ingedrukt wordt
{
    light(Button.line, Button.button, false); // ingedrukte knop uitzetten
    Button.wasPressed = true;
}

void reset()
{
    sendReset(Slave_A);
}

void gameEnd(bool hasWon)
{
    sendGameEnd(hasWon, millis() - previousMillis);
    reset();
}