#ifndef DSDISPLAY_H
#define DSDISPLAY_H

#include <ILI9341_t3n.h>
#include <XPT2046_Touchscreen.h>
#include <MsTimer2.h>
#include <vector>
#include <DSScene.h>
#include <DSScenes.h>
#include "DSFonts.h"
#include "controls.h"

#define TOUCH_CS 8
#define TFT_CS 10
#define TFT_DC 9
#define TIRQ_PIN 2
#define TOUCH_REFRESH_RATE 60
#define SCREEN_REFRESH_RATE 60

// // This is calibration data for the raw touch data to the screen coordinates
// #define TS_MINX 350
// #define TS_MINY 350
// #define TS_MAXX 3800
// #define TS_MAXY 3800

class DSDisplay : public DSBounds
{
public:
    DSDisplay(void (*displayTouchCallback)(), PolySynth *synth, Controls *controls) : DSBounds()
    {        
        _controls = controls;
        this->onDisplayUpdateTouch = displayTouchCallback;
        this->synth = synth;                
    }
    ~DSDisplay() {}

    void begin()
    {
        tft.begin(90000000u, 6000000);
        if (!ts.begin())
        {
            Serial.println(F("Unable to start touchscreen."));
            while (1)
                delay(1);
        }
        Serial.println(F("Touchscreen started."));        
        tft.setRotation(3);
        tft.fillScreen(ILI9341_BLACK);        
        this->setBounds(2,0,318,240);
    }

    void setupScenes()
    {
        PSVoicePart *dco1 = synth->voice1.part1;
        PSVoicePart *dco2 = synth->voice1.part2;
        PSLFO *filterLFO1 = synth->filterLFO2;

//      PSLFO *filterLFO = new PSLFO()
        
        tft.setFont(FONT_UI_LABEL);
        addScene(new DSTestScene(&tft));
        addScene(new DSSceneVoice(&tft));
        addScene(new DSParamEdit(&tft, &synth->voice1));
        addScene(new DSParamEdit(&tft, dco1));
        addScene(new DSParamEdit(&tft, filterLFO1));
        addScene(new DSSceneEnvelope(&tft, F("OSC1 - A-ENV"), dco1->vca_env, _controls));
        addScene(new DSSceneEnvelope(&tft, F("OSC1 - F-ENV"), dco1->vcf_env, _controls));
        addScene(new DSSceneEnvelope(&tft, F("OSC1 - P-ENV"), dco1->mod_env, _controls));
        addScene(new DSSceneEnvelope(&tft, F("OSC2 - A-ENV"), dco2->vca_env, _controls));
        addScene(new DSSceneEnvelope(&tft, F("OSC2 - F-ENV"), dco2->vcf_env, _controls));
        addScene(new DSSceneEnvelope(&tft, F("OSC2 - P-ENV"), dco2->mod_env, _controls));

        scenes[sceneIndex]->show();

        Encoder *sceneEnc = &_controls->encoders[0];
        sceneEnc->setRange(0, (int)scenes.size() - 1);
        sceneIndex = 0;
        tft.setTextSize(1);
        tft.setFont(FONT_UI_HEADING);
    }

    void updateTouch()
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
        Encoder *sceneEnc = &_controls->encoders[0];
        int newIndex = (int)max((int)sceneEnc->getValue(), 0) % scenes.size();

        if (newIndex != sceneIndex)
            nextScene(newIndex);
        scenes[sceneIndex]->render();
    }

    void clearScreen(uint16_t color = ILI9341_BLUE) { scenes[sceneIndex]->clear(); }

    ILI9341_t3n &lcd() { return tft; }

    void nextScene(int newIndex = -1)
    {
        scenes[sceneIndex]->hide();

        if (newIndex < 0)
            sceneIndex = (sceneIndex + 1) % scenes.size();
        else
            sceneIndex = newIndex;
        scenes[sceneIndex]->show();
        scenes[sceneIndex]->clear();
    }

    uint8_t getSceneID() { return sceneIndex; }

    DSScene *getCurrentScene() { return scenes[sceneIndex]; }

private:    
    ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC);
    XPT2046_Touchscreen ts = XPT2046_Touchscreen(TOUCH_CS, TIRQ_PIN);
    PolySynth *synth;
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