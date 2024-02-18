#ifndef PS_SYNTH_H
#define PS_SYNTH_H

#include <PSComponent.h>
#include <PSVoice.h>
#include <PSLFO.h>
#include <patching.h>


class PSSynth : public PSComponent
{
public:
    PSVoice *voice1;
    PSLFO *filterLFO;
    PSLFO *pitchLFO;

    PSSynth() : PSComponent("Synth")
    {
        // components
        voice1 = new PSVoice("Voice1", this);
        
        //filterLFO = new PSLFO("Filter LFO", this); 
        //pitchLFO = new PSLFO("Pitch LFO", this);
        
        // parameters

        

    } 
};

#endif