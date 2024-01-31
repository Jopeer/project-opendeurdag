// #include <Wire.h>

// const int knop = 2;

// int Slave_A = 3; // adress van de slave

// void setup()
// {
//     Serial.begin(9600);

//     Wire.begin();
//     pinMode(knop, INPUT_PULLUP);
// }

// void loop()
// {
//     if (!digitalRead(knop))
//     {
//         // Serial.print("knop");

//         Wire.beginTransmission(Slave_A);
//         Wire.write(true);
//         Wire.endTransmission();
//     }
//     else
//     {
//         Wire.beginTransmission(Slave_A);
//         Wire.write(false);
//         Wire.endTransmission();
//     }
// }