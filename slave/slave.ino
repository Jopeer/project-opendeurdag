#include <Adafruit_NeoPixel.h>
#include <Wire.h>

// geen delays bij de slave!
Adafruit_NeoPixel ledLine0(5, A0, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ledLine1(5, A1, NEO_GRB + NEO_KHZ800);

const int adress = 3;

struct
{
    int ButtonPin[9];
    int button;
    int line;
    bool wasPressed;
} ButtonToPress;

void setup()
{
    Serial.begin(9600);

    ledLine0.begin();
    ledLine1.begin();

    Wire.begin(adress);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);

    for (int i = 3; i < 13; i++)
    {
        ButtonToPress.ButtonPin[i] = i;
    }
    for (int i = 3; i < 13; i++)
    {
        pinMode(ButtonToPress.ButtonPin[i], INPUT);
    }
}

void loop()
{
    // if buttonPressed => buttonPressed();
    if (!digitalRead(ButtonToPress.ButtonPin[ButtonToPress.button]))
    {
        buttonPressed();
    }
    
}

void receiveEvent(int numBytes)
{
    switch (Wire.read()) // de eerste byte bepaald het doel van de boodschap
    {
    case 0:
        receiveButton();
        break;

    case 1:
        light(Wire.read(), Wire.read(), Wire.read());

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