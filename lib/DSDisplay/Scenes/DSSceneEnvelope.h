#ifndef DS_SCENENEVELOPE_H
#define DS_SCENENEVELOPE_H

#include <Audio.h>
#include <DSScenes.h>
#include <DSButton.h>
#include <DSColor.h>

#include "DSEnvelopeGraph.h"
#include "controls.h"
#include "PolySynth.h"
#include "PSVoice.h"
#include "PSParameter.h"
#include "DSParameterDial.h"

class DSSceneEnvelope : public DSScene
{
public:
    DSSceneEnvelope(ILI9341_t3n *lcd, String label, PSEnvelope *env, Controls *controls) : DSScene(lcd)
    {
        _thisElement = (DSElement *)this;
        _envelope = env;
        _controls = controls;
        color.background = 0;
        setSceneName(label);

        addDial(PSP_ENV_ATTACK, 0);
        addDial(PSP_ENV_HOLD, -1);
        addDial(PSP_ENV_DECAY, 1);
        addDial(PSP_ENV_SUSTAIN, 2);
        addDial(PSP_ENV_RELEASE, 3);
        layoutParams();

        graph = (DSEnvelopeGraph *)addElement(new DSEnvelopeGraph(lcd, "graph", _envelope->params));
    }

    ~DSSceneEnvelope()
    {
        DSScene::~DSScene();
    }

    void addDial(PSEnvelopeParam paramType, int potNum)
    {
        PSParameter *p = _envelope->params[paramType];
        if (potNum != -1)
            p->attachController(&_controls->pots[potNum]);

        DSParameterDial *dial = new DSParameterDial(_lcd, p, _thisElement);
        dial->attachToSpace(this);

        addElement(dial);
    }

    void render() override
    {
        updateControls();
        DSScene::render();
    }

    void updateControls()
    {
        if (visible)
        {
            if (_envelope->updateFromControl())
                graph->setDidChange();
        }
    }

    void show() override
    {
        DSScene::show();
        for (auto p : _envelope->params)
            p->activate();
    }
    void hide() override
    {
        DSScene::hide();
        for (auto p : _envelope->params)
            p->deactivate();
    }

protected:
    void layoutParams()
    {
        //int parmCount = _envelope->params.size();
        const int usableWidth = 280;
        const int xmargin = (320 - usableWidth) / 2;
        int n = 5;                 // Number of objects
        const int w = 50;          // Width of each object
        const int x = usableWidth; // Horizontal distance
        int spacing = (x - w) / (n - 1);

        int i = 0;
        for (auto e : elements)
        {
            if (e->getName() == "PARM")
            {                
                int xpos = xmargin + i * spacing;                
                e->setBoundsPosition(xpos, 0);
                e->dock(noneH, bottom, 0, 10);
                i++;
            }
        }
    }

private:
    DSEnvelopeGraph *graph;
    DSElement *_thisElement;
    PSEnvelope *_envelope;
    Controls *_controls;
};

#endif