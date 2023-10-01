

// #include "PSComponent.h"
// #include "PSParameter.h"
// //#include "PSParamaterTargets.h"
// #include "Audio.h"

// class TestComponent : PSComponent
// {
// public:
//     TestComponent(String name = "test") : PSComponent(name)
//     {
//         AudioEffectEnvelope *e = &filter1env;
//         a = addParameter("a", 50, 0, 100);
//         d = addParameter("d", 50, 0, 100);
//         s = addParameter("s", 50, 0, 100);
//         r = addParameter("r", 0.5, 0.0f, 1.0f);

//         // this->set("a", 80);
//     }

//     void attack(float v)
//     {
//         a->setValue(v);
//     }

//     static void setParm(PSParameter *p, AudioEffectEnvelope *e)
//     {
//         switch (p->name.c_str())
//         {
//         case "a":
//             e->attack(p->getValue());
//             break;
//         case "d":
//             e->decay(p->getValue());
//             break;
//         case "s":
//             break;
//         case "r":
//             break;
//         }
//     }

// private:
//     PSParameter *a, *d, *s, *r;
//     PSParameterVector parameters;
// };



