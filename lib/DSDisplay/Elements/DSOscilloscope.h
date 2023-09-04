#ifndef DS_OSCILLOSCOPE_H
#define DS_OSCILLOSCOPE_H

#include "DSElement.h"

#pragma region WIP
// #include "SimpleTimer.h"

//*************************************************************************
// ElectroTechnique TSynth scope
// Change scale for TFT ER-TFTM018-3 Display 128x160 Pixel
// and implement Idle flag when no audio signal
//
// https://github.com/ElectroTechnique/TSynth-Teensy4.1
//
//*************************************************************************

// #define NO_OF_BLOCKS 1

// class DSOscilloscope : public AudioStream, public DSElement
// {
// public:
//     DSOscilloscope(ILI9341_t3n *lcd, String label) : AudioStream(1, inputQueueArray), DSElement(lcd, label, Rect())
//     {
//         box.x = 15;
//         box.y = 65;
//         box.width = 128;
//         box.height = 60;
//     }
//     virtual void update(void);
//     void ScreenSetup(ILI9341_t3n *);
//     void Display(void);
//     void AddtoBuffer(int16_t *);
//     void inputConnection(AudioStream *input, uint8_t fromChannel);

// private:
//     audio_block_t *inputQueueArray[1];
//     ILI9341_t3n *display;
//     int16_t buffer[AUDIO_BLOCK_SAMPLES * NO_OF_BLOCKS];
//     int16_t old_val[AUDIO_BLOCK_SAMPLES + 1];
//     uint32_t count = 0;
//     AudioConnection *_input = nullptr;
// };
// #endif

// void DSOscilloscope::inputConnection(AudioStream *input, uint8_t fromChannel = 0)
// {
//     if (_input == nullptr)
//         _input = new AudioConnection(*input, fromChannel, *this, 0);
// }

// void DSOscilloscope::ScreenSetup(ILI9341_t3n *screen)
// {
//     __disable_irq();
//     display = screen;
//     __enable_irq();
// }

// void DSOscilloscope::Display()
// {
//     __disable_irq();

//     uint16_t pixel_x = 0;
//     uint16_t i = 0;
//     old_val[0] = 96;
//     do
//     {
//         int16_t wave_data = buffer[i];
//         int16_t pixel_y = map(wave_data, 32767, -32768, -100, 100) + 96;
//         if (pixel_y < 65)
//         {
//             pixel_y = 65;
//             display->drawLine(pixel_x + 19, old_val[pixel_x], pixel_x + 19, pixel_y, ILI9341_BLACK); // New pixel
//         }
//         else if (pixel_y > 125)
//         {
//             pixel_y = 125;
//             display->drawLine(pixel_x + 19, old_val[pixel_x], pixel_x + 19, pixel_y, ILI9341_BLACK); // New pixel
//         }
//         else
//         {
//             display->drawLine(pixel_x + 19, old_val[pixel_x], pixel_x + 19, pixel_y, ILI9341_BLUE); // New pixel
//         }
//         old_val[pixel_x + 1] = pixel_y;
//         pixel_x++;
//         i = i + NO_OF_BLOCKS;
//     } while (i < ((AUDIO_BLOCK_SAMPLES * NO_OF_BLOCKS) - 8)); // -8 trigger samples
//     __enable_irq();
// }

// void DSOscilloscope::AddtoBuffer(int16_t *audio)
// {
//     // const int16_t *begin = audio;
//     const int16_t *end = audio + AUDIO_BLOCK_SAMPLES;
//     int sample1 = 0;
//     int sample2 = 0;
//     uint8_t trigger_flag = false;
//     __disable_irq();

//     // cross trigger
//     for (uint8_t i = 0; i < 8; i++)
//     {
//         sample1 = *audio;
//         audio++;
//         sample2 = *audio;
//         if (sample1 > 0 && sample2 < 0)
//         {
//             trigger_flag = true;
//             break;
//         }
//     }

//     if (trigger_flag == true)
//     {
//         do
//         {
//             buffer[count++] = *audio;
//             audio++;
//         } while (audio < end);
//         if (count > (AUDIO_BLOCK_SAMPLES * NO_OF_BLOCKS) - 1)
//         {
//             count = 0;
//         }
//     }
//     Display();
//     __enable_irq();
// }

// void DSOscilloscope::update(void)
// {
//     if (!display)
//         return;
//     audio_block_t *block;
//     block = receiveReadOnly(0);
//     if (block)
//     {
//         AddtoBuffer(block->data);
//         release(block);
//     }
// }

#pragma endregion

class DSOscilloscope : public AudioStream, public DSElement
{
public:
    DSOscilloscope(ILI9341_t3n *lcd, String name) : AudioStream(1, inputQueueArray), DSElement(lcd, name)
    {
        setBounds(0, 150, 255, 60);        
    }

    int16_t getBufferValue(uint16_t index, Rect &box)
    {
        int16_t value = map(buffer[index], 32767, -32768, -110, 110) + 93;
        value = clamp(value, (int16_t)65, (int16_t)124);
        return value + box.y - box.height-5;
    }

    void display()
    {
        Rect box = dockedBounds();
        //lcd->drawRect(box.x - 1, box.y - 1, box.width + 2, box.height + 2, color.border);

        prev_pixel_y = getBufferValue(0, box);
        pixel_x = 0;

        lcd->fillRect(box.x, box.y, box.width, box.height, lcd->color565(10, 10, 20));
        for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES - 1; i++)
        {
            pixel_y = getBufferValue(i, box);
            lcd->drawLine(pixel_x + box.x, prev_pixel_y, pixel_x + box.x + 2, pixel_y, 0x07B0);
            prev_pixel_y = pixel_y;
            pixel_x += 2;
            // if (pixel_x > AUDIO_BLOCK_SAMPLES - 1)
            //     Serial.println("gonna crash!!");
            // old_val[pixel_x + 1] = pixel_y;
        }
    }

    void inputConnection(AudioStream *input, uint8_t fromChannel = 0)
    {

        if (_input == nullptr)
            _input = new AudioConnection(*input, fromChannel, *this, 0);
    }

    void render() override
    {
        if (visible && didChange)
        {
            uint32_t now = millis();
            if (now - lastDisplayRefresh > 50)
            {
                lastDisplayRefresh = now;
                display();
            }
        }
        DSElement::render();
    }

    // Runs on Audio Thread
    void AddtoBuffer(int16_t *audio)
    {
        audio++;
        if (bufferBlock == 0)
        {
            if (*(audio - 1) > -16 && *(audio + 3) < 16)
            {
                bufferBlock = 1;
                bufcount = 0;
            }
        }
        else
        {
            for (uint16_t i = 0; i < 32; i++)
            {
                buffer[bufcount++] = *audio;
                audio += 4;
            }
            bufferBlock++;
            if (bufferBlock >= 5)
                bufferBlock = 0;
        }
    }

    // runs on Audio Thread
    void update(void)
    {
        if (!visible)
            return;

        audio_block_t *block;
        block = receiveReadOnly(0);
        if (block)
        {
            AddtoBuffer(block->data);
            release(block);
            if (bufferBlock == 0)
                didChange = true;
        }
    }

private:
    audio_block_t *inputQueueArray[1];
    int16_t buffer[AUDIO_BLOCK_SAMPLES];
    AudioConnection *_input = nullptr;
    boolean EnvIdelFlag = false;
    boolean scopeIdel = false;

    uint8_t bufferBlock = 0;
    uint8_t bufcount = 0;
    uint8_t pixel_x = 0;
    int16_t pixel_y = 0;
    int16_t prev_pixel_y = 0;
    boolean MainShift = false;
    uint32_t lastDisplayRefresh;
    int16_t old_val[AUDIO_BLOCK_SAMPLES + 1];
};

#endif
