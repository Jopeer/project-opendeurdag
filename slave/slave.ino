#include <Wire.h>

// geen delays bij de slave!

const int adress = 3;

struct
{
    int ButtonPin[4];
    int button;
    int line;
    bool wasPressed;
} ButtonToPress;

void setup()
{
    Serial.begin(9600);

    Wire.begin(adress);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);

    for (int i = 3; i < 13; i++)
    {
        pinMode(ButtonToPress.ButtonPin[i], INPUT);
    }
}

void loop()
{
    // if buttonPressed => buttonPressed();
}

void receiveEvent(int numBytes)
{
    switch (Wire.read()) // de eerste byte bepaald het doel van de boodschap
    {
    case 0:
        receiveButton();
        break;

    case 1:
        reset();
        break;

    case 2:
        gameEnd();
        break;
    }
}

void receiveButton()
{
    ButtonToPress.line = Wire.read();
    ButtonToPress.button = Wire.read();

    light(ButtonToPress.line, ButtonToPress.button, true);
}

void requestEvent()
{
    Wire.write(ButtonToPress.wasPressed);
    if (ButtonToPress.wasPressed)
    {
        ButtonToPress.wasPressed = false; // de drukknop terug op false zetten voor de volgende
    }
}

void buttonPressed() // word opgeroepen als de juiste knop ingedrukt wordt
{
    light(ButtonToPress.line, ButtonToPress.button, false); // ingedrukte knop uitzetten
    ButtonToPress.wasPressed = true;
}

void light(int line, int button, bool ledON)
{
}

void reset()
{
    Serial.print("reset");
}

void gameEnd()
{
    switch (Wire.read())
    {
    case true:
        /* gewonne */
        unsigned long completionTime = Wire.read();

        break;

    case false:
        /* verloren */
        Wire.read(); // clear buffer
        break;
    }
}