#pragma once
#include <PCF8575.h>
#include <vector>
#include "ControlPins.h"
#include "timing.h"

#define PCF_ISR_PIN 2
#define PCF_ADDR0 0x20
#define PCF_ADDR1 0x21
#define PCF_ADDR2 0x22
#define PCF_ADDR3 0x23

typedef std::vector<PCF8575 *> DigitalBusVector;
volatile bool digitalIOChanged = false;
void DigitalIO_ISR() { digitalIOChanged = true; }

class DigitalIO
{
public:
    DigitalBusVector bus;
    DigitalIO() {}
    ~DigitalIO() {}

    void init()
    {
        Serial.println("initialising Digital IO");
        Wire.begin();
        if (!(
                // this->addBus(PCF_ADDR0) &&
                // this->addBus(PCF_ADDR1) &&
                this->addBus(PCF_ADDR2) &&
                this->addBus(PCF_ADDR3)))
        {
            Serial.println(F("Error: Digital IO Bus Expansion failed to inititalise "));
            Serial.printf("Usable busses %d and pins %d\n", this->busCount(), this->pinCount());
        }
        this->findAddress();
        this->setDebounceTime(30);
        this->setupIOPins();
    }

    void begin()
    {
        pinMode(PCF_ISR_PIN, INPUT_PULLUP);
        attachInterrupt(PCF_ISR_PIN, DigitalIO_ISR, CHANGE);
    }

    void update()
    {
        if (digitalIOChanged)
            onDidGPIOChange();
    }

    void setupIOPins()
    {

        for (auto b : bus) {
            b->write16(0);
            b->setButtonMask(0);
        }

        for (auto p : buttonPinsActive)
            pinMode(p, INPUT);

        for (auto l : ledPinsActive) {
            pinMode(l, OUTPUT);
        } 

        // uint16_t mask[this->busCount()];
        // uint8_t bus, channel;
        // for (uint8_t b = 0; b < this->busCount(); b++)
        //     mask[b] = word(B00000000, B11111111);
        // for (uint8_t p = 0; p < sizeof(buttonPinsActive); p++)
        // {
        //     this->getDigitalBusChannel(buttonPinsActive[p], bus, channel);
        //     printf("clearing bit %d in bus %d\n", channel, bus);
        //     bitClear(mask[bus], channel);
        // }
        // for (uint8_t b = 0; b < this->busCount(); b++) {
        //     printf("bus0-mask : %s , bus1-mask : %s\n", binary16(mask[0]).c_str(), binary16(mask[1]).c_str());
        // }
        // mask[0] = word(B00000000, B00000000);
        // mask[1] = word(B00000000, B11110000);
        // for (uint8_t b = 0; b < this->busCount(); b ++)
        // {
        //     this->bus[b]->setButtonMask(mask[b]);
        // }
    }

    void pinMode(uint8_t gpioPin, uint8_t pinMode)
    {
        uint8_t bus, channel;
        getDigitalBusChannel(gpioPin, bus, channel);
        PCF8575 *b = this->bus[bus];
        b->write(channel, pinMode);
        uint16_t mask = b->getButtonMask();       
        (pinMode) ? bitSet(mask, channel) : bitClear(mask, channel);
        b->setButtonMask(mask);    
        
        Serial.printf("setting output [pin %d] buttonMask [bus %d] = %s\n", channel, bus, binary16(b->getButtonMask()).c_str());
    }

    bool addBus(uint8_t addr, uint16_t buttonMask = word(B00000000, B11110000))
    {
        if (busCount() < 8)
        {
            PCF8575 *b = new PCF8575(addr);
            if (b->begin())
            {
                //b->write16(buttonMask);
                //b->setButtonMask(buttonMask);
            }
            else
            {
                Serial.printf("Error: Unable to initialise PCF board %d at address (%x)\n", busCount() + 1, addr);
                delete[] b;
                return false;
            }
            bus.push_back(b);
            return true;
        }
        return false;
    }

    /// @brief reads the state of a pin directly from hardware
    /// @param gpioId the id of the pin to query
    /// @return the state
    uint8_t digitalReadIO(uint8_t gpioId)
    {
        if (this->bus.size() > 0)
        {
            uint8_t bus = 0, channel = 0;
            if (getDigitalBusChannel(gpioId, bus, channel))
                return this->bus[bus]->read(channel);
        }
        return 0;
    }

    /// @brief for output pins, sets the state of the pin to on or off
    /// @param gpioId the id of the pin to change state
    /// @param on the state to set
    void digitalWriteIO(uint8_t gpioId, bool on)
    {
        uint8_t bus = 0, channel = 0;
        if (getDigitalBusChannel(gpioId, bus, channel))
        {
            if (bus < busCount())
            {
                noInterrupts();
                this->bus[bus]->write(channel, (on) ? LOW : HIGH);
                interrupts();
            }
        }
    }

    /// @brief reads the last stored state of the GPIO pin
    /// @param gpioId the id of the pin to read from
    /// @return whether the pin is set or not
    bool pinState(uint8_t gpioId)
    {
        uint8_t bus = 0, channel = 0;
        if (getDigitalBusChannel(gpioId, bus, channel))
        {
            bool bitval = false;
            if (bus < busCount())
            {
                // Serial.printf("UPDATING BUTTON FROM PINSTATE : bus=%d, channel=%d\n", bus, channel);
                // Serial.printf("busdata[%d] = %d\n", bus, busData[bus]);
                bitval = bitRead(busData[bus], channel);
                // Serial.printf("BITVAL = %d\n", bitval);
            }
            return bitval;
        }
        return false;
    }

    void setDebounceTime(uint32_t debounceMs) { debounceTimer.duration(debounceMs); }

    void findAddress()
    {
        int count = 0;
        Serial.println("searching for IO bus addresses");
        // loop through possible addresses
        for (int i = 1; i < 120; i++)
        {            
            Wire.beginTransmission(i);
            // returns 0 if device found
            if (Wire.endTransmission() == 0)
            {
                Serial.print("Address Found: ");
                Serial.print(i);
                Serial.print(", (0x");
                Serial.print(i, HEX);
                Serial.print("), ");
                Serial.print(" (0b");
                Serial.print(i, BIN);
                Serial.println(")");
                count++;
            }
            delay(5);
        } // end of for loop
        Serial.printf("Found %d device(s).\n", count);
    }

    uint16_t getBusData(uint8_t busIndex)
    {
        return busData[busIndex % 8];
    }

    uint8_t busCount() { return this->bus.size(); }
    uint8_t pinCount() { return busCount() * 16; }

    String binary16(uint16_t iIn)
    {
        String result = "";
        for (uint16_t mask = 0b1000000000000000; mask; mask >>= 1)
        {
            if (mask & iIn)
                result += '1';
            else
                result += '0';
        }
        return result;
    }

private:
    uint16_t busData[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    SimpleTimer debounceTimer;

    bool getDigitalBusChannel(uint8_t gpioId, uint8_t &bus, uint8_t &channel)
    {
        bus = gpioId / 16;
        channel = gpioId % 16;
        return (bus < busCount());
    }

    void onDidGPIOChange()
    {
        digitalIOChanged = false;
        if (debounceTimer.update())
        {
            for (uint8_t i = 0; i < busCount(); i++)
                busData[i] = bus[i]->read16();

            Serial.printf("GPIO changed : [0] %s - [1] %s\n", binary16(busData[0]).c_str(), binary16(busData[1]).c_str());
        }
    }

} digitalIO;

// ..............
//
// uint8_t gpioLed[] = {4, 5, 6, 7, 20, 21, 22, 23};
// uint8_t gpioButton[] = {1, 2, 3, 4, 16, 17, 18, 19};
// uint32_t ledFlashLastTime = 0;
// uint32_t ledFlashDuration = 100;
//
// void digitalIOSetup()
// {
//     bool success = (digitalIO.addBus(PCF_ADDR0, word(B00000000, B11110000)) &&
//                     digitalIO.addBus(PCF_ADDR1, word(B00000000, B11110000)) &&
//                     digitalIO.addBus(PCF_ADDR2, word(B00000000, B11110000)) &&
//                     digitalIO.addBus(PCF_ADDR3, word(B00000000, B11110000)));
//
//     if (!success)
//     {
//         Serial.println(F("Error: Digital IO Bus Expansion failed to inititalise "));
//         Serial.printf("Usable busses %d and pins %d\n", digitalIO.getBusCount(), digitalIO.getPinCount());
//         digitalIO.findAddress();
//     }
//     digitalIO.setDebounceTime(10);
//     digitalIO.begin();
// }
//
// SimpleTimer flashTimer(10);
// void digitalIOLoop()
// {
//     if (flashTimer.update())
//         digitalIO.digitalWriteIO(gpioLed[random(sizeof(gpioLed))], (bool)random(2));
//     digitalIO.update();
//     digitalIO.pinState(0);
// }
