#include <Wire.h>

const int led = 12;
const int adress = 3;

struct
{
    int ButtonPin[4];
    int button;
    int line;
} Button;

void setup()
{
    Serial.begin(9600);
    pinMode(led, OUTPUT);

    Wire.begin(adress);
    Wire.onReceive(receiveEvent);
}

void loop()
{
}

void receiveEvent(int numBytes)
{
    switch (Wire.read()) //de eerste byte bepaald het doel van de boodschap
    {
    case 0:
        receiveButton();
        break;

    case 1:
        reset();
    }
}

void receiveButton()
{
    int line = Wire.read();
    int button = Wire.read();

    Serial.println("line: ");
    Serial.print(line);
    Serial.println("button: ");
    Serial.print(button);

    Button.line = line;
    Button.button = button;

    digitalWrite(led, HIGH);

    light(line, button, true);
}

void buttonPressed() // word opgeroepen als de juiste knop ingedrukt wordt
{
    light(Button.line, Button.button, false); // ingedrukte knop uitzetten
}

void light(int line, int button, bool ledON)
{
}
void reset()
{
    Serial.print("reset");
    digitalWrite(led, LOW);
}