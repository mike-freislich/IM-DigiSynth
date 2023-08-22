#ifndef PS_VOICE_H
#define PS_VOICE_H

#include "PSComponent.h"
#include "PSDCO.h"

class PSVoice : public PSComponent
{
public:
    PSDCO *dco1, *dco2;

    PSVoice(String name) : PSComponent(name)
    {
        dco1 = new PSDCO("DCO1", &AENV1, &filter1env, &osc1PENV);
        addChild(dco1);        
        dco2 = new PSDCO("DCO2", &AENV2, &filter2env, &osc2PENV);
        addChild(dco2);
    }

};

#endif