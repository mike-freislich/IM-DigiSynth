#ifndef DSDISPLAY_H
#define DSDISPLAY_H

#include <ILI9341_t3n.h>
#include <MsTimer2.h>
#include <vector>
#include <DSScene.h>
#include <DSScenes.h>
#include "DSFonts.h"
#include "controls.h"

#define TFT_DC 9
#define TFT_CS 10
//#define TFT_MOSI 11
//#define TFT_MISO 12
//#define TFT_SCK 13

#define SCREEN_REFRESH_RATE 60
//#include <XPT2046_Touchscreen.h>
//#define TOUCH_CS 8
//#define TIRQ_PIN 2
//#define TOUCH_REFRESH_RATE 60

class DSDisplay : public DSBounds
{
public:
    DSDisplay(PolySynth *synth, Controls *controls) : DSBounds()
    {        
        _controls = controls;        
        this->synth = synth;                
    }
    ~DSDisplay() {}

    void begin()
    {        
        tft.begin(90000000u, 1000000);     
        //tft.begin
        tft.setRotation(3);
        tft.fillScreen(ILI9341_BLACK);        
        this->setBounds(2,0,318,240);
    }

    void setupScenes()
    {       
        tft.setFont(FONT_UI_LABEL);
        //addScene(new DSTestScene(&tft));
        addScene(new DSSceneVoice(&tft));
        addScene(new DSParamEdit(&tft, synth));
        addScene(new DSParamEdit(&tft, &synth->voice1));
        addScene(new DSParamEdit(&tft, synth->voice1.part1));
        addScene(new DSParamEdit(&tft, synth->LFO1));
        addScene(new DSParamEdit(&tft, synth->LFO3));
        addScene(new DSParamEdit(&tft, synth->voice1.part2));        
        addScene(new DSParamEdit(&tft, synth->LFO2));
        addScene(new DSParamEdit(&tft, synth->LFO4));
        
        addScene(new DSSceneEnvelope(&tft, F("OSC1 - A-ENV"), synth->voice1.part1->vca_env, _controls));
        addScene(new DSSceneEnvelope(&tft, F("OSC1 - F-ENV"), synth->voice1.part1->vcf_env, _controls));
        addScene(new DSSceneEnvelope(&tft, F("OSC1 - P-ENV"), synth->voice1.part1->mod_env, _controls));
        addScene(new DSSceneEnvelope(&tft, F("OSC2 - A-ENV"), synth->voice1.part2->vca_env, _controls));
        addScene(new DSSceneEnvelope(&tft, F("OSC2 - F-ENV"), synth->voice1.part2->vcf_env, _controls));
        addScene(new DSSceneEnvelope(&tft, F("OSC2 - P-ENV"), synth->voice1.part2->mod_env, _controls));

        scenes[sceneIndex]->show();

        Encoder *sceneEnc = &_controls->encoders[0];
        sceneEnc->setRange(0, (int)scenes.size() - 1);
        sceneIndex = 0;
        tft.setTextSize(1);
        tft.setFont(FONT_UI_HEADING);
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
    ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC, -1);
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