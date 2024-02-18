#pragma once

#define MUXA_COM A0
#define MUXB_COM A1
#define MUXC_COM A2
#define MUXD_COM A3
#define MXP_S0 31
#define MXP_S1 30
#define MXP_S2 29
#define MXP_S3 28
#define SAMPLEAVG 32
#define BITRESOLUTION 10

const uint8_t PROGMEM muxBus[] = {MUXA_COM, MUXB_COM, MUXC_COM, MUXD_COM};
const uint8_t PROGMEM potPin[] = {0, 1, 2, 3};
uint16_t last[] = {0, 0, 0, 0};

class AnalogMux
{
public:
    AnalogMux()
    {
        pinMode(MXP_S0, OUTPUT);
        pinMode(MXP_S1, OUTPUT);
        pinMode(MXP_S2, OUTPUT);
        pinMode(MXP_S3, OUTPUT);
        pinMode(MUXA_COM, INPUT);
        pinMode(MUXB_COM, INPUT);
        analogReadResolution(BITRESOLUTION);
        analogReadAveraging(SAMPLEAVG);
    }

    uint16_t analogReadIO(uint8_t ioPin)
    {
        uint8_t bus = 0, channel = 0;
        ioToBusChannel(ioPin, bus, channel);
        return muxRead(bus, channel);
    }

private:
    void SetChannel(uint8_t channel)
    {
        digitalWrite(MXP_S0, bitRead(channel, 0));
        digitalWrite(MXP_S1, bitRead(channel, 1));
        digitalWrite(MXP_S2, bitRead(channel, 2));
        digitalWrite(MXP_S3, bitRead(channel, 3));
    }

    uint16_t muxRead(uint8_t busID, uint8_t ioChannel)
    {                
        SetChannel(ioChannel);
        return analogRead(muxBus[busID]);
    }

    void ioToBusChannel(uint8_t ioId, uint8_t &bus, uint8_t &channel)
    {
        bus = ioId / 16;
        channel = ioId % 16;
    }
} analogMux;


// void analogMuxSetup()
// {
// }
//
// void analogMuxLoop()
// {
//     bool changed = false;
//
//     for (uint8_t i = 0; i < sizeof(potPin); i++)
//     {
//         uint16_t potval = analogMux.analogReadIO(potPin[i]);
//         if (potval < 10)
//             potval = 10;
//         if (potval > 1014)
//             potval = 1014;
//         potval = map(potval, 10, 1014, 0, 1000);
//
//         if (potval < last[i] - 1 || potval > last[i] + 1)
//         {
//             last[i] = potval;
//             changed = true;
//         }
//     }
//     if (changed)
//     {
//         for (uint8_t i = 0; i < sizeof(potPin); i++)
//         {
//             Serial.print("pot ");
//             Serial.print(i);
//             Serial.print(" ");
//             Serial.print(last[i]);
//             Serial.print(", ");
//         }
//         Serial.println();
//     }
// }