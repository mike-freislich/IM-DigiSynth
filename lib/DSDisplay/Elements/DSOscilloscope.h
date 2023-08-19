#ifndef DS_OSCILLOSCOPE_H
#define DS_OSCILLOSCOPE_H

#include "DSElement.h"
#include "SimpleTimer.h"

//*************************************************************************
// ElectroTechnique TSynth scope
// Change scale for TFT ER-TFTM018-3 Display 128x160 Pixel
// and implement Idle flag when no audio signal
//
// https://github.com/ElectroTechnique/TSynth-Teensy4.1
//
//*************************************************************************

class DSOscilloscope : public AudioStream, public DSElement
{
public:
    DSOscilloscope(ILI9341_t3n *lcd, String label) : AudioStream(1, inputQueueArray), DSElement(lcd, label, Rect())
    {
        box.x = 15;
        box.y = 65;
        box.width = 128;
        box.height = 60;
    }

    void inputConnection(AudioStream *input, uint8_t fromChannel = 0)
    {
        
        if (_input == nullptr)
            _input = new AudioConnection(*input, fromChannel, *this, 0);
    }

    void render()
    {

        if (getShouldRedraw())
        {
            if (_scene != nullptr && _scene->visible)
            {
                uint32_t now = millis();
                 if (now - lastDisplayRefresh < 1000 / 20)
                     return;
                lastDisplayRefresh = now;
                pixel_x = 0;
                prev_pixel_y = map(buffer[0], 32767, -32768, -110, 110) + 93;
                if (prev_pixel_y < 65)
                    prev_pixel_y = 65;
                if (prev_pixel_y > 121)
                    prev_pixel_y = 121;

                _lcd->fillRect(box.x, box.y, box.width, box.height, _lcd->color565(10,10,10));
                for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES - 1; i++)
                {
                    pixel_y = map(buffer[i], 32767, -32768, -110, 110) + 93;
                    if (pixel_y < 65)
                        pixel_y = 65;
                    if (pixel_y > 121)
                        pixel_y = 121;
                    _lcd->drawLine(pixel_x + 15, prev_pixel_y, pixel_x + 16, pixel_y, 0x07B0);
                    prev_pixel_y = pixel_y;
                    pixel_x++;
                }
            }
            DSElement::render();
        }
    }

    // Runs on Audio Thread
    void AddtoBuffer(int16_t *audio)
    {
        if (_scene != nullptr && _scene->visible)
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
    }

    // runs on Audio Thread
    void update(void)
    {
        if (_scene != nullptr && _scene->visible)
        {
            audio_block_t *block;
            block = receiveReadOnly(0);
            if (block)
            {
                AddtoBuffer(block->data);
                release(block);
                if (bufferBlock == 0)
                {
                    setShouldRedraw(true);
                }
            }
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
};

#endif
