#include <Wire.h>

// geen delays bij de slave!

const int adress = 3;

struct
{
    int ButtonPin[4];
    int button;
    int line;
    bool wasPressed;
} Button;

void setup()
{
    Serial.begin(9600);

    Wire.begin(adress);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);

    for (int i = 3; i < 13; i++)
    {
        pinMode(Button.ButtonPin[i], INPUT);
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
    Button.line = Wire.read();
    Button.button = Wire.read();

    light(Button.line, Button.button, true);
}

void requestEvent()
{
    Wire.write(Button.wasPressed);
    if (Button.wasPressed)
    {
        Button.wasPressed = false; // de drukknop terug op false zetten voor de volgende
    }
}

void buttonPressed() // word opgeroepen als de juiste knop ingedrukt wordt
{
    light(Button.line, Button.button, false); // ingedrukte knop uitzetten
    Button.wasPressed = true;
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