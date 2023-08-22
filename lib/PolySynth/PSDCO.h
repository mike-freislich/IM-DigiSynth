#ifndef PS_DCO_H
#define PS_DCO_H

#include "patching.h"
#include "PSEnvelope.h"

class PSDCO : public PSComponent
{
public:
    PSEnvelope *vca_env, *vcf_env, *mod_env;
    
    PSDCO(String name, AudioEffectEnvelope *a_env, AudioEffectEnvelope *f_env, AudioEffectEnvelope *p_env)
        :PSComponent(name)
    {
        vca_env = new PSEnvelope("A-ENV", a_env);        
        vcf_env = new PSEnvelope("F-ENV", f_env);
        mod_env = new PSEnvelope("P-ENV", p_env);

        this->addChild(vca_env);
        this->addChild(vcf_env);
        this->addChild(mod_env);

        //vca_env->update(3, 130, 50, 15);
        //vcf_env->update(3, 130, 50, 15);
        //mod_env->update(3, 130, 50, 15);
    }
    ~PSDCO() {}

private:
    
};

#endif