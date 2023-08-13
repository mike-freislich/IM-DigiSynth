#ifndef DS_SCENENEVELOPE_H
#define DS_SCENENEVELOPE_H

#include <Audio.h>
#include <DSScene.h>
#include <DSButton.h>
#include <DSColor.h>
#include "Parameters.h"
#include "DSParameterElement.h"
#include "DSEnvelopeGraph.h"
#include "controls.h"
#include "PolySynth.h"

class DSSceneEnvelope : public DSScene
{
public:
    DSEnvelopeGraph *graph;
    DSParameterElement *attack, *hold, *decay, *sustain, *release;

    DSSceneEnvelope(ILI9341_t3n *lcd, String label, MyEnvelope *env) : DSScene(lcd)
    {
        _env = env;
        _thisElement = (DSElement *)this;
        color.background = 0;
        setSceneName(label);

        // create parameters
        _parms = new EnvParameters(label);

        // create elements
        attack = (DSParameterElement *)addElement(new DSParameterElement(_lcd, _parms->getParm(PARM_ATTACK), _thisElement));
        hold = (DSParameterElement *)addElement(new DSParameterElement(_lcd, _parms->getParm(PARM_HOLD), _thisElement));
        decay = (DSParameterElement *)addElement(new DSParameterElement(_lcd, _parms->getParm(PARM_DECAY), _thisElement));
        sustain = (DSParameterElement *)addElement(new DSParameterElement(_lcd, _parms->getParm(PARM_SUSTAIN), _thisElement));
        release = (DSParameterElement *)addElement(new DSParameterElement(_lcd, _parms->getParm(PARM_RELEASE), _thisElement));
        layoutParams();

        graph = (DSEnvelopeGraph *)addElement(new DSEnvelopeGraph(lcd, "graph", (EnvParameters *)_parms));
    }

    ~DSSceneEnvelope()
    {
        DSScene::~DSScene();
        delete[] _parms;
    }

    void render()
    {
        DSScene::render();
    }

    void updateControls()
    {
        // get envelope parameters from the scene
        attack->setValue(map(controls.pots[0].getLogValue(), POT_RANGE_MIN, POT_RANGE_MAX, 0, attack->getMax()));
        decay->setValue(map(controls.pots[1].getLogValue(), POT_RANGE_MIN, POT_RANGE_MAX, 0, decay->getMax()));
        sustain->setValue(map(controls.pots[2].getValue(), POT_RANGE_MIN, POT_RANGE_MAX, 0, sustain->getMax()));
        release->setValue(map(controls.pots[3].getLogValue(), POT_RANGE_MIN, POT_RANGE_MAX, 0, release->getMax()));
        _env->setParameters(attack->getValue(), 0, decay->getValue(), sustain->getValue() / 100, release->getValue());
        graph->setShouldRedraw(true);
    }

protected:
    void layoutParams()
    {
        int parmCount = _parms->getLength();
        uint8_t xpad, ypad = 3;
        int usableWidth = 280;

        for (int i = 0; i < parmCount; i++)
        {
            DSElement *pe = elements[i + 1];
            Rect bb = pe->getBoundingBox();
            xpad = (usableWidth - (bb.width * parmCount)) / (parmCount);
            pe->MoveTo(i * (bb.width + xpad) + (320 - usableWidth) / 2, this->height() - bb.height - ypad);
        }
    }

private:
    DSElement *_thisElement;
    MyEnvelope *_env;
};

#endif