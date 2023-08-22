#ifndef DSDISPLAY_H
#define DSDISPLAY_H

#include <ILI9341_t3n.h>
#include <XPT2046_Touchscreen.h>
#include <MsTimer2.h>
#include <DSScene.h>
#include <DSSceneVoice.h>
#include <DSSceneSplash.h>
#include <DSSceneEnvelope.h>
#include "DSFonts.h"
#include "controls.h"

#define TOUCH_CS 8
#define TFT_CS 10
#define TFT_DC 9
#define TIRQ_PIN 2
#define TOUCH_REFRESH_RATE 60
#define SCREEN_REFRESH_RATE 60

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 350
#define TS_MINY 350
#define TS_MAXX 3800
#define TS_MAXY 3800

class DSDisplay
{
public:
    DSDisplay(void (*displayTouchCallback)(), PolySynth *synth, Controls *controls)
    {
        _controls = controls;
        sceneIndex = 0;
        tft.setTextSize(1);
        this->onDisplayUpdateTouch = displayTouchCallback;

        PSDCO *dco1 = synth->voice1.dco1;
        PSDCO *dco2 = synth->voice1.dco2;
        addScene(new DSSceneVoice(&tft));
        addScene(new DSSceneEnvelope(&tft, F("OSC1 - A-ENV"), dco1->vca_env, controls));
        addScene(new DSSceneEnvelope(&tft, F("OSC1 - F-ENV"), dco1->vcf_env, controls));
        addScene(new DSSceneEnvelope(&tft, F("OSC1 - P-ENV"), dco1->mod_env, controls));
        addScene(new DSSceneEnvelope(&tft, F("OSC2 - A-ENV"), dco2->vca_env, controls));
        addScene(new DSSceneEnvelope(&tft, F("OSC2 - F-ENV"), dco2->vcf_env, controls));
        addScene(new DSSceneEnvelope(&tft, F("OSC2 - P-ENV"), dco2->mod_env, controls));

        scenes[sceneIndex]->show();
        tft.setFont(FONT_UI_LABEL);
    }
    ~DSDisplay() {}

    void begin()
    {
        tft.begin(90000000u, 6000000);
        if (!ts.begin())
        {
            Serial.println(F("Unable to start touchscreen."));
            while (1)
                ;
        }
        Serial.println(F("Touchscreen started."));

        this->clearScreen(ILI9341_BLACK);
        tft.setRotation(3);

        MsTimer2::set(1000 / TOUCH_REFRESH_RATE, this->onDisplayUpdateTouch);
        MsTimer2::start();
    }

    void update()
    {
        /*
        if (ts->tirqTouched())
        {
            if (!ts->bufferEmpty())
            {
                TS_Point p = ts->getPoint();
                p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft->width());
                p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft->height());
                scenes[sceneIndex]->update(p);
            }
        }
        */
    }

    void render()
    {
        if (_controls->buttons[0].pressed())
        {
            uint32_t now = millis();
            if (now - lastSceneChange > 500)
            {
                nextScene();
                lastSceneChange = now;
            }
        };

        if (sceneIndex != 0)
        {
            ((DSSceneEnvelope *)scenes[sceneIndex])->updateControls();
        }

        scenes[sceneIndex]->render();
    }

    void clearScreen(uint16_t color = ILI9341_BLUE)
    {
        scenes[sceneIndex]->clear();
    }

    ILI9341_t3n &lcd()
    {
        return tft;
    }

    void nextScene()
    {
        scenes[sceneIndex]->hide();
        sceneIndex = (sceneIndex + 1) % scenes.size();
        scenes[sceneIndex]->show();
        scenes[sceneIndex]->clear();
        // render();
    }

    uint8_t getSceneID()
    {
        return sceneIndex;
    }

    DSScene *getCurrentScene()
    {
        return scenes[sceneIndex];
    }

private:
    ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC);
    XPT2046_Touchscreen ts = XPT2046_Touchscreen(TOUCH_CS, TIRQ_PIN);
    void (*onDisplayUpdateTouch)();
    Controls *_controls = nullptr;

    int sceneIndex;
    DSSceneVector scenes;
    uint32_t lastSceneChange;

    template <typename T>
    T *addScene(T *scene)
    {
        scenes.push_back(scene);
        return scene;
    }
};

#endif