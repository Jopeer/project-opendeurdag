#include <Adafruit_NeoPixel.h>
#include <Wire.h>

Adafruit_NeoPixel ledLine0(5, A0, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ledLine1(5, A1, NEO_GRB + NEO_KHZ800);

const int gameButtons = 10; // aantal knoppen per game

unsigned long currentMillis;
unsigned long previousMillis;
unsigned long GameStartMillis;     // de tijd wanneer een nieuw spel gestart is
unsigned long MaxplayTime = 30000; // de max tijd om een knop in te drukken [ms]

int Slave_A = 3; // adress van de slave

struct
{
    int ButtonPin[9];
    int button;
    int line;
    int arduino;
    bool wasPressed;
} ButtonToPress;

void setup()
{
    Serial.begin(9600);
    Wire.begin();

    for (int i = 3; i < 13; i++)
    {
        pinMode(ButtonToPress.ButtonPin[i], INPUT);
    }
}

void loop()
{
    // sendButton(1, 0, 4); // startknop
    // while (wasButtonPressed(1))
    // {
    //     // wachtanimatie
    //     delay(100);
    // }
    // GameStartMillis = millis();
    // for (int i = 0; i < gameButtons; i++)
    // {
    //     randButton();
    //     while (!wasButtonPressed(ButtonToPress.arduino))
    //     {

    //         if (millis() - GameStartMillis >= MaxplayTime)
    //         {
    //             gameEnd(false); // verloren
    //             return;
    //         }
    //     }
    // }
    // gameEnd(true);

    sendButton(1, 0, 0);
    // while (wasButtonPressed(1))
    // {
    //     delay(100);
    // }
}

void randButton()
{
    int arduino = random(0, 1);
    int line = random(0, 1);
    int button = random(0, 5);

    switch (arduino)
    {
    case 0:

        ButtonToPress.line = line;
        ButtonToPress.button = button;
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
    Wire.write(0); // case 0: receiveButton() (zie slave)
    Wire.write(line);
    Wire.write(button);
    Wire.endTransmission();
}

bool wasButtonPressed(int arduino)
{
    switch (arduino)
    {
    case 0:
        return ButtonToPress.wasPressed;

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
switch (line)
    {
    case 0:
        ledLine0.setPixelColor(button, 255, 0, 0);
        ledLine0.show();
        break;

    case 1:
        ledLine1.setPixelColor(button, 255, 0, 0);
        ledLine1.show();
        break;
    }
    
}

void sendGameEnd(bool hasWon, unsigned long completionTime)
{
    Wire.beginTransmission(Slave_A);
    Wire.write(2); // case 2: gameEnd() (zie slave)
    Wire.write(hasWon);
    Wire.write(completionTime);
    Wire.endTransmission();
}

void sendLight(int line, int button, bool ledON)
{
    Wire.beginTransmission(Slave_A);
    Wire.write(1); // case 1 light() (zie slave)
    Wire.write(line);
    Wire.write(button);
    Wire.write(ledON);
    Wire.endTransmission();
}

void buttonPressed() // word opgeroepen als de juiste knop ingedrukt wordt
{
    light(ButtonToPress.line, ButtonToPress.button, false); // ingedrukte knop uitzetten
    ButtonToPress.wasPressed = true;
}

void reset()
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            light(i, j, false);
            sendLight(i, j, false);
        }
    }
}

void gameEnd(bool hasWon)
{
    sendGameEnd(hasWon, millis() - GameStartMillis);
    reset();
}