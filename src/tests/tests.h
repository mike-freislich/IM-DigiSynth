#ifndef PS_TESTS_H
#define PS_TESTS_H

//#define RUNTESTS

#ifdef RUNTESTS

#include <Arduino.h>
#include <SD.h>
#include <PSComponent.h>
#include <InputButton.h>

void testComponents();
void testSDCard();
bool testAudioStream();
bool testAudioComponent();
void testIO();
void testLED_loop();
void testmap();

FLASHMEM void runTests()
{
    Serial.println("*************** RUNNING TESTS *************** ");
    // testComponents();
    // testSDCard();
    // testAudioStream();
    // testAudioComponent();
    // testIO();

    testmap();
}

class ACEnvelope : public PSComponent
{
public:
    ACEnvelope(String name = "ACEnvelope") : PSComponent(name) {}
};

FLASHMEM bool testAudioComponent()
{
    ACEnvelope ace;
    PSParameter *attack = ace.addParameter("Attack", 50, 0, 1000, &filter1env, [](AudioStream *audioStream, float value)
                                           { ((AudioEffectEnvelope *)audioStream)->attack(value); });

    attack->setValue(112);
    attack->callTarget();

    delete attack;
    return true;
}

class Foo // Parameter
{
public:
    Foo(AudioStream *s, float v, ParameterTarget t)
    {
        setAudioStream(s);
        setTarget(t);
        setValue(v);
    };

    void setAudioStream(AudioStream *stream) { audioStream = stream; }
    void setTarget(ParameterTarget t) { target = t; }
    void setValue(float v) { value = v; }

    void callTarget()
    {
        if (target != nullptr)
            target(audioStream, value);
    }

private:
    ParameterTarget target = nullptr;
    AudioStream *audioStream = nullptr;
    float value;
};

FLASHMEM bool testAudioStream()
{
    Foo foo(&filter1env, 99.1, [](AudioStream *audioStream, float value)
            { ((AudioEffectEnvelope *)audioStream)->attack(value); });

    foo.callTarget();

    return true;
}

FLASHMEM bool savePatch(const char *filename, String data)
{

    if (SD.exists(filename))
    {
        Serial.println("overwriting file.");
        SD.remove(filename);
    }

    File patchFile = SD.open(filename, FILE_WRITE);
    if (patchFile)
    {
        patchFile.print(data);
        patchFile.close();
        Serial.println("saved patch file");
        return true;
    }
    Serial.println("Error: unable to save patchfile");
    return false;
}

FLASHMEM String loadPatch(const char *filename)
{
    if (SD.exists(filename))
    {
        File patchFile = SD.open(filename);
        if (patchFile)
        {
            String data = patchFile.readString();
            patchFile.close();
            return data;
        }
    }
    return "";
}

FLASHMEM void testSDCard()
{
    Serial.println("BEGIN TEST : SD CARD -------------------------");
    SD.begin(BUILTIN_SDCARD);
    if (!SD.mediaPresent())
        Serial.println("Error: no media");

    savePatch("testPatch.psp", "*Voice1{*DCO1{*A-ENV{Del:0.00,Atk:3.00,Hld:0.00,Dcy:0.00,Sus:50.00,Rel:3.00,Amt:1.00}*F-ENV{Del:0.00,Atk:3.00,Hld:0.00,Dcy:0.00,Sus:50.00,Rel:3.00,Amt:1.00}*P-ENV{Del:0.00,Atk:3.00,Hld:0.00,Dcy:0.00,Sus:50.00,Rel:3.00,Amt:1.00}}*DCO2{*A-ENV{Del:0.00,Atk:3.00,Hld:0.00,Dcy:0.00,Sus:50.00,Rel:3.00,Amt:1.00}*F-ENV{Del:0.00,Atk:3.00,Hld:0.00,Dcy:0.00,Sus:50.00,Rel:3.00,Amt:1.00}*P-ENV{Del:0.00,Atk:3.00,Hld:0.00,Dcy:0.00,Sus:50.00,Rel:3.00,Amt:1.00}}}");

    String data = loadPatch("testPatch.psp");
    printf("PatchFile data = '%s'\n", data.c_str());

    printf("totalSize %llu\n", SD.totalSize());
    printf("usedSize %llu\n", SD.usedSize());
}

FLASHMEM void testComponents()
{
    delay(1000);
    Serial.println("BEGIN TEST : COMPONENTS -------------------------");

    PSComponent c("Synth");
    PSComponent lfo("LFO");
    PSComponent env_a("ENV_A");
    PSComponent env_f("ENV_F");

    c.addChild(&lfo);
    c.addChild(&env_a);
    c.addChild(&env_f);

    // c.addParameter("p1", 0.1);
    // c.addParameter("p2", 0.2);
    //  lfo.addParameter("freq", 1000);
    //  lfo.addParameter("shape", 2);
    //  env_a.addParameter("atk", 30);
    //  env_a.addParameter("dcy", 130);
    //  env_a.addParameter("sus", 65);
    //  env_a.addParameter("rel", 32);
    //  env_f.addParameter("atk", 32);
    //  env_f.addParameter("dcy", 132);
    //  env_f.addParameter("sus", 67);
    //  env_f.addParameter("rel", 32);

    Serial.println(c.toString());

    Serial.println("END TEST : COMPONENTS -------------------------");

    String data = "*Synth{p1:0.10,p2:0.20*LFO{freq:1000.00,shape:2.00}*ENV_A{atk:30.00,dcy:130.00,sus:65.00,rel:32.00}*ENV_F{atk:32.00,dcy:132.00,sus:67.00,rel:32.00}}";
    c.fromString(data);
}

SimpleTimer ftimer;
int lightIndex = 0;
uint8_t testLights[] = {4, 20, 21, 22, 23};
FLASHMEM void testIO()
{
    Serial.println("*************** testing IO ***************");
    digitalIO.setup();
    digitalIO.setDebounceTime(30);
    digitalIO.begin();
    ftimer.setDuration(500);

    while (1)
    {
        int currentLight = testLights[lightIndex];
        digitalIO.digitalWriteIO(currentLight, true);
        while (!ftimer.update())
        {
            delay(10);
            digitalIO.update();
        }
        digitalIO.digitalWriteIO(currentLight, false);

        lightIndex = (lightIndex + 1) % (int)sizeof(testLights);
    }
}

#endif

#endif

#include "PSObjectCollection.h"

void testmap()
{
    PSObjectCollection collection;
    collection.addItem(new PSObject(PSK::CTRL_BTN_Shift, "shift button"));

    //collection.getItem()

}