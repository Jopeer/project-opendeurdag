#include <Adafruit_NeoPixel.h>
#include <Wire.h>

Adafruit_NeoPixel ledStrip0(5, A0, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ledStrip1(5, A1, NEO_GRB + NEO_KHZ800);

const int testButton = A2;
const int gameButtons = 10; // aantal knoppen per game

unsigned long currentMillis;
unsigned long previousMillis;
unsigned long GameStartMillis;     // de tijd wanneer een nieuw spel gestart is
unsigned long MaxplayTime = 30000; // de max tijd om een knop in te drukken [ms]

int Slave_A = 3; // adress van de slave

struct
{
    int ButtonPin[10];
    int button;
    int line;
    int arduino;
    bool wasPressed;
} ButtonToPress;

void setup()
{
    Serial.begin(9600);
    // Wire.begin();

    ledStrip0.begin();
    ledStrip1.begin();

    randomSeed(analogRead(A2));

    pinMode(testButton, INPUT_PULLUP);
    for (int j = 0; j < 2; j++)
    {
        for (int i = 2; i <= 7; i++)
        {
            ButtonToPress.ButtonPin[i - 2 + 5 * j] = i + (5 * j);
            pinMode(ButtonToPress.ButtonPin[i - 2 + 5 * j], INPUT);
        }
    }
}

void loop()
{
    light(ButtonToPress.line, ButtonToPress.button, false);
    light(0, 1, true);

    ButtonToPress.button = 1;
    ButtonToPress.line = 0;
    light(ButtonToPress.line, ButtonToPress.button, true);
    while (!wasButtonPressed(ButtonToPress.arduino))
    {
        if (digitalRead(ButtonToPress.ButtonPin[ButtonToPress.button + 5 * ButtonToPress.line]))
        {
            buttonPressed();
        }
        delay(10);
    }
    ButtonToPress.wasPressed = false;
    Serial.print("pressed");

    GameStartMillis = millis();
    for (int i = 0; i < gameButtons; i++)
    {
        randButton();
        while (!wasButtonPressed(ButtonToPress.arduino))
        {
            if (digitalRead(ButtonToPress.ButtonPin[ButtonToPress.button + 5 * ButtonToPress.line]))
            {
                Serial.print("pressed");
                buttonPressed();
            }

            if (millis() - GameStartMillis >= MaxplayTime)
            {
                // gameEnd(false); // verloren
                return;
            }

            if (!digitalRead(testButton))
            {
                test_Buttons();
            }
            
        }
        ButtonToPress.wasPressed = false;
    }
    // gameEnd(true);
}

void randButton()
{
    // int arduino = random(0, 1);

    int arduino = 0;
    int line = random(0, 1);
    int button = random(0, 5);
    Serial.println(button);
    switch (arduino)
    {
    case 0:

        ButtonToPress.line = line;
        ButtonToPress.button = button;
        light(line, button, true);

        break;

    case 1:

        // sendButton(arduino, line, button);

        break;
    }
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
        // code lampen op line 0 te laten branden
        if (ledON == true)
        {
            ledStrip0.setPixelColor(button, ledStrip0.Color(0, 225, 0));
        }
        else
        {
            ledStrip0.setPixelColor(button, ledStrip0.Color(0, 0, 0));
        }
        ledStrip0.show();
        break;
    case 1:
        // code lampen op line 1 te laten branden
        if (ledON == true)
        {
            ledStrip1.setPixelColor(button, ledStrip1.Color(0, 225, 0));
        }
        else
        {
            ledStrip1.setPixelColor(button, ledStrip1.Color(0, 0, 0));
        }
        ledStrip1.show();
        break;
    }
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
            // sendLight(i, j, false);
        }
    }
}

void gameEnd(bool hasWon)
{
    // sendGameEnd(hasWon, millis() - GameStartMillis);
    reset();
}

void test_Buttons()
{
    for (int j = 0; j < 2; j++)
    {
        for (int i = 0; i < 5; i++)
        {
            light(j, i, true);
            while (!digitalRead(ButtonToPress.ButtonPin[i + 5 * j]))
            {
                delay(10);
            }
            light(j, i, false);
        }
    }
}

// void sendButton(int arduino, int line, int button)
// {
//     Wire.beginTransmission(Slave_A);
//     Wire.write(0); // case 0: receiveButton() (zie slave)
//     Wire.write(line);
//     Wire.write(button);
//     Wire.endTransmission();
// }

// void sendGameEnd(bool hasWon, unsigned long completionTime)
// {
//     Wire.beginTransmission(Slave_A);
//     Wire.write(2); // case 2: gameEnd() (zie slave)
//     Wire.write(hasWon);
//     Wire.write(completionTime);
//     Wire.endTransmission();
// }

// void sendLight(int line, int button, bool ledON)
// {
//     Wire.beginTransmission(Slave_A);
//     Wire.write(1); // case 1 light() (zie slave)
//     Wire.write(line);
//     Wire.write(button);
//     Wire.write(ledON);
//     Wire.endTransmission();
// }
