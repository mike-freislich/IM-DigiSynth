#ifndef PS_TESTS_H
#define PS_TESTS_H

//#define RUNTESTS

#ifdef RUNTESTS

#include <Arduino.h>
#include <PSComponent.h>

void testComponents();

FLASHMEM void runTests()
{
    testComponents();
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

    //c.addParameter("p1", 0.1);
    //c.addParameter("p2", 0.2);
    lfo.addParameter("freq", 1000);
    lfo.addParameter("shape", 2);
    env_a.addParameter("atk", 30);
    env_a.addParameter("dcy", 130);
    env_a.addParameter("sus", 65);
    env_a.addParameter("rel", 32);
    env_f.addParameter("atk", 32);
    env_f.addParameter("dcy", 132);
    env_f.addParameter("sus", 67);
    env_f.addParameter("rel", 32);

    Serial.println(c.toString());

    Serial.println("END TEST : COMPONENTS -------------------------");

    String data = "*Synth{p1:0.10,p2:0.20*LFO{freq:1000.00,shape:2.00}*ENV_A{atk:30.00,dcy:130.00,sus:65.00,rel:32.00}*ENV_F{atk:32.00,dcy:132.00,sus:67.00,rel:32.00}}";
    c.fromString(data);
}

#endif

#endif