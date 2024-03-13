#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel ledStrip0(5, A0, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ledStrip1(5, A1, NEO_GRB + NEO_KHZ800);

// parameters:
int BeginHealth = 5;               // hoevaak de spelers een knop mogen misse door tijdsdruk
int beginBadButtons = 0;           // #slechte knoppen aan het begin van het spel
int maxBadButtons = 2;             // maximum aantal slechte knoppen
int diffecultyChangeInterval = 10; // #rondes voor de moeilijkheidsgraad verhoogd
int beginWaitTime = 1500;          // de startwachttijd [ms]
int WaitTimeChangeRate = 250;      // #ms waarmee de wachttijd tussen rondes afneemd per interval [ms].
unsigned long BeginMaxReactionTime = 10000;
unsigned long ReactionTimeChangeRate = 500;
//

const int testButton = A2;

unsigned long currentMillis;
unsigned long previousMillis;
unsigned long maxReactionTime = BeginMaxReactionTime;

int ButtonPins[10]; // array met de pinnen van elke knop
const int ButtonPinsSize = 10;

int ButtonStates[5]; // array met de statusen van elke knop: (0 = uit, 1 = slecht, 2 = goed)
const int buttonStatesSize = 5;

int playerScore_0;
int playerScore_1;
int badButtons = beginBadButtons;
int waitTime = beginWaitTime;
int health = BeginHealth;
bool buttonPressed;

enum ButtonState
{
    off,
    bad,
    good
};

void setup()
{
    Serial.begin(9600);

    ledStrip0.begin();
    ledStrip1.begin();

    randomSeed(analogRead(A2));

    pinMode(testButton, INPUT_PULLUP);
    for (int line = 0; line <= 1; line++)
    {
        for (int button = 0; button < 5; button++)
        {
            ButtonPins[button + 5 * line] = (button + 5 * line) + 2; // geeft elke knop je juiste pin, +2 omdat pin 0,1 niet gaan
            pinMode(ButtonPins[button + 5 * line], INPUT);           // zet elke pin op input

            Serial.println("knop: " + String(button + 5 * line) +
                           " zit op pin: " + String((button + 5 * line) + 2));
        }
    }
}

void loop()
{
    gameReset();
    light();
    WaitForGameBegin();

    for (int i = 0; i < diffecultyChangeInterval; i++)
    {
        if (health == 0)
        {
            GameEnd();
            return;
        }

        randButtonStates(badButtons);
        light();

        previousMillis = millis();
        while (!buttonPressed)
        {
            readButtons();

            if (millis() - previousMillis >= maxReactionTime)
            {
                badButtonPressed(2); // case 2 bijde spelers te traag
                health -= 1;
                Serial.println("health: " + String(health));
            }
        }
        buttonPressed = false;
        delay(waitTime);
    }
    increaceDifficulty();
}

void WaitForGameBegin()
{
    unsigned long previousHoldMillis = 0;
    while (millis() - previousHoldMillis <= 3000)
    {
        if ((digitalRead(ButtonPins[0]) &&
             digitalRead(ButtonPins[4]) &&
             digitalRead(ButtonPins[5]) &&
             digitalRead(ButtonPins[9])))
        {
            if (previousHoldMillis == 0)
            {
                light();
                previousHoldMillis = millis();
            }
        }
        else
        {
            blink();
            previousHoldMillis = millis();
        }
    }
}

void gameReset()
{
    maxReactionTime = BeginMaxReactionTime;
    badButtons = beginBadButtons;
    waitTime = beginWaitTime;
    health = BeginHealth;
    buttonPressed = false;

    for (int i = 0; i < buttonStatesSize; i++)
    {
        ButtonStates[i] = 0;
    }
    ButtonStates[0] = good;
    ButtonStates[4] = good;
}

void increaceDifficulty()
{
    if (badButtons < maxBadButtons)
    {
        badButtons++;
    }
    if (waitTime - WaitTimeChangeRate > 0)
    {
        waitTime -= 250;
    }
    if (maxReactionTime - ReactionTimeChangeRate > 0)
    {
        maxReactionTime -= ReactionTimeChangeRate;
    }

    health = BeginHealth;
}

void readButtons()
{
    for (int button = 0; button < 5; button++)
    {
        for (int line = 0; line < 2; line++)
        {
            checkButtonState(button, line);
            if (buttonPressed)
            {
                return;
            }
        }
    }
}

void checkButtonState(int button, int line)
{

    if (digitalRead(ButtonPins[button + 5 * line]))
    {
        switch (ButtonStates[button])
        {
        case off:
            break;

        case bad:
            badButtonPressed(line);
            break;

        case good:
            goodButtonPressed(line);
            break;
        }
    }
}

void goodButtonPressed(int player)
{
    buttonPressed = true;
    switch (player)
    {
    case 0:
        playerScore_0 += 1;
        ledStrip0.fill(ledStrip0.Color(0, 255, 0));
        ledStrip1.clear();
        break;

    case 1:
        playerScore_1 += 1;
        ledStrip1.fill(ledStrip0.Color(0, 255, 0));
        ledStrip0.clear();
        break;
    }

    Serial.println("score player 0: " + String(playerScore_0));
    Serial.println("score player 1: " + String(playerScore_1));

    ledStrip0.show();
    ledStrip1.show();
}

void badButtonPressed(int player)
{
    buttonPressed = true;
    switch (player)
    {
    case 0:
        playerScore_0 -= 1;
        ledStrip0.fill(ledStrip0.Color(255, 0, 0));
        ledStrip1.clear();
        break;

    case 1:
        playerScore_1 -= 1;
        ledStrip1.fill(ledStrip1.Color(255, 0, 0));
        ledStrip0.clear();
        break;

    case 2: // als bijde speler niet op tijd op de knop drukken
        ledStrip0.fill(ledStrip0.Color(255, 0, 0));
        ledStrip1.fill(ledStrip1.Color(255, 0, 0));
    }
    Serial.println("score player 0: " + String(playerScore_0));
    Serial.println("score player 1: " + String(playerScore_1));

    ledStrip0.show();
    ledStrip1.show();
}

void randButtonStates(int NumBadButtons) // vul de array "buttonStates" met statussen
{
    for (int i = 0; i < buttonStatesSize; i++) // volledige array nul maken
    {
        ButtonStates[i] = 0;
    }

    int buttonToPress = random(0, 5); // kies een knop om in te drukken
    ButtonStates[buttonToPress] = good;

    int badButton;
    for (int i = 0; i < NumBadButtons; i++)
    {
        badButton = random(0, 5);
        while (badButton == buttonToPress) // slechte knop mag niet de goede vervangen
        {
            badButton = random(0, 5);
        }
        ButtonStates[badButton] = bad;
    }
}

void light()
{
    ledStrip0.clear();
    ledStrip1.clear();

    for (int i = 0; i < buttonStatesSize; i++)
    {
        switch (ButtonStates[i])
        {
        case off:
            break;

        case bad:
            ledStrip0.setPixelColor(i, ledStrip0.Color(225, 0, 0));
            ledStrip1.setPixelColor(i, ledStrip1.Color(225, 0, 0));
            break;

        case good:
            ledStrip0.setPixelColor(i, ledStrip0.Color(0, 225, 0));
            ledStrip1.setPixelColor(i, ledStrip1.Color(0, 225, 0));
            break;
        }
    }
    ledStrip0.show();
    ledStrip1.show();
}

void GameEnd()
{
    for (int i = 0; i < 15; i++)
    {
        if (playerScore_0 > playerScore_1)
        {

            ledStrip0.fill(ledStrip0.Color(0, 255, 0));
            ledStrip1.fill(ledStrip1.Color(255, 0, 0));
        }
        else if (playerScore_0 < playerScore_1)
        {

            ledStrip0.fill(ledStrip0.Color(255, 0, 0));
            ledStrip1.fill(ledStrip1.Color(0, 255, 0));
        }
        else // gelijkspel
        {

            ledStrip0.fill(ledStrip0.Color(0, 255, 0));
            ledStrip1.fill(ledStrip1.Color(0, 255, 0));
        }
        ledStrip0.show();
        ledStrip1.show();
        delay(700);
        ledStrip0.clear();
        ledStrip1.clear();
        ledStrip0.show();
        ledStrip1.show();
        delay(700);
    }
}

void blink()
{
    light();
    delay(700);
    ledStrip0.clear();
    ledStrip1.clear();
    ledStrip0.show();
    ledStrip1.show();
    delay(500);
}

// void test_Buttons()
// {
//     for (int j = 0; j < 2; j++)
//     {
//         for (int i = 0; i < 5; i++)
//         {
//             light(j, i, true);
//             while (!digitalRead(ButtonToPress.ButtonPin[i + 5 * j]))
//             {
//                 delay(10);
//             }
//             light(j, i, false);
//         }
//     }
// }