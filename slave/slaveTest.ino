// #include <Wire.h>

// const int led = 12;
// const int adress = 3;

// bool ledOn;

// void setup()
// {
//     // Serial.begin(9600);
//     pinMode(led, OUTPUT);

//     Wire.begin(adress);
//     Wire.onReceive(receiveEvent);
// }

// void loop()
// {

//     if (ledOn)
//     {
//         digitalWrite(led, HIGH);
//     }
//     if (!ledOn)
//     {
//         digitalWrite(led, LOW);
//     }
// }

// void receiveEvent(int numBytes)
// {
//     while (Wire.available())
//     {
//         ledOn = Wire.read();
//         // Serial.print("receiveEvent");
//     }
// }