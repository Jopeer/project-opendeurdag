#include <Wire.h>

const int knop = 2;
const int resetKnop = 3;

unsigned long currentMillis;
unsigned long MaxplayTime = 30000; // de max tijd om een knop in te drukken [ms]

int Slave_A = 3; // adress van de slave

void setup()
{
    Serial.begin(9600);
    Wire.begin();

    pinMode(knop, INPUT_PULLUP);
    pinMode(resetKnop, INPUT_PULLUP);
}

void loop()
{
    if (!digitalRead(knop))
    {
        randButton();
        waitForButton();
    }

    if (!digitalRead(resetKnop))
    {
        sendReset(Slave_A);
    }

    

    // if (currentMillis >= playTime)
    // {
    //     sendReset(Slave_A);
    // }
    // currentMillis = millis();
}

void randButton()
{
    int arduino = random(0, 1);
    int line = random(0, 1);
    int button = random(0, 5);
    sendButton(arduino, line, button);
}

void sendButton(int arduino, int line, int button)
{
    Wire.beginTransmission(Slave_A);
    Wire.write(0); // stuur het doel van de boodschap
    switch (arduino)
    {
    case 0:
        light(line, button);

        break;

    case 1:

        Wire.write(line);
        Wire.write(button);

        break;
    }
    Wire.endTransmission();
}

void waitForButton()
{
    Wire.requestFrom(Slave_A, 1);

    while (!Wire.available() && currentMillis <= MaxplayTime)
    {
        currentMillis = millis();
    }
    // if (currentMillis >= MaxplayTime)
    // {
    //     sendReset(Slave_A);
    // }
    
    
    Serial.print("pressed");
}

void light(int line, int button)
{
}

void sendReset(int slaveAdress)
{
    Wire.beginTransmission(slaveAdress);
    Wire.write(1); // case 1: reset() (zie slave)
    Wire.endTransmission();
}