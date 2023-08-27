#ifndef PS_COMPONENT_H
#define PS_COMPONENT_H

#include <Arduino.h>
#include "PSParameter.h"
#include <vector>

class PSComponent;

using namespace std;
typedef vector<PSComponent *> PSComponentVector;
typedef vector<String> SplitStrings;

FLASHMEM bool isNumber(char c) { return c >= 48 && c <= 57; }                                                    // numeral
FLASHMEM bool isText(char c) { return (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c == 95) || (c == 45); } // Alpha or underscore
FLASHMEM bool isNameChar(char c) { return isNumber(c) || isText(c); }                                            // Valid characters for name

FLASHMEM SplitStrings splitString(const String &input, char delimiter)
{
    SplitStrings lines;
    int start = 0;
    int end = input.indexOf(delimiter);

    while (end >= 0)
    {
        lines.push_back(input.substring(start, end));
        start = end + 1;
        end = input.indexOf(delimiter, start);
    }
    lines.push_back(input.substring(start));
    return lines;
}

class PSComponent
{
public:
    String name;
    PSComponent *parent;
    PSComponentVector children;
    PSParameterVector params;

    PSComponent(String name, PSComponent *parent = nullptr)
    {
        this->name = name;
        this->parent = parent;
    }

    // find parameter by name and set the value
    FLASHMEM bool set(String parameterName, float v, bool force = false)
    {
        for (auto param : params)
        {
            //printf("searching for P-%s\n", parameterName.c_str());
            if (param->name == parameterName)
            {                
                param->setValue(v, force);
                //Serial.println("found!");
                return true;
            }
        }
        //Serial.println("not found!");
        return false;
    }

    // find parameter by index and set the value
    FLASHMEM bool set(uint8_t parameterIndex, float v, bool force = false)
    {
        if (parameterIndex > (params.size() - 1))
            return false;
        params[parameterIndex]->setValue(v, force);
        return true;
    }

    // string manipulation : ...
    FLASHMEM int findNextIndexOf(const char *buffer, char findChar, int &p, String *copyOut = nullptr, bool allcharsLegal = false)
    {
        while (buffer[p] != findChar)
        {
            char c = buffer[p];

            if (c == '\0')
                return -1;

            if (copyOut != nullptr)
            {
                if (allcharsLegal || isNameChar(c))
                    *copyOut += c;
            }
            p++;
        }
        return p;
    }

    // returns a string containing the config data
    String toString()
    {
        String s = "*" + name + "{";
        for (auto param : params)
            s += param->name + ":" + param->getValue() + ",";
        if (s.charAt(s.length() - 1) == ',')
            s.remove(s.length() - 1);
        for (auto child : children)
            s += child->toString();
        s += "}";
        return s;
    }

    // accepts a config data string and sets the parameters of the component
    void fromString(String data)
    {
        String componentName;
        const char *cd = data.c_str();
        int p = 0;
        while (componentName != name)
        {
            componentName = "";
            if (findNextIndexOf(cd, '*', p) != -1)
            {
                if (findNextIndexOf(cd, '{', p, &componentName) == -1)
                    return;
            }
        }
        p++;
        Serial.printf(">>>>>> %s <<<<<<\n", componentName.c_str());

        while (isWhitespace(cd[p]) && p < (int)data.length())
            p++;

        // Get Parameter Block
        int nextComponent = p;
        int endParameterBlock = p;
        findNextIndexOf(cd, '*', nextComponent);
        findNextIndexOf(cd, '}', endParameterBlock);
        //Serial.printf("nextComponent %d, endParameterBlock %d\n", nextComponent, endParameterBlock);

        String pblock = data.substring(p, min(nextComponent, endParameterBlock));
        //Serial.printf("allocated pblock with %d characters\n", pblock.length());

        if (pblock.length())
        {
            p = min(nextComponent, endParameterBlock);
            Serial.printf("PARAMETER BLOCK : %s\n", pblock.c_str());

            // get Parameters
            SplitStrings paramStrings = splitString(pblock, ',');
            for (auto pstring : paramStrings)
            {
                //Serial.printf("analyzing PARM: %s\n", pstring.c_str());
                SplitStrings nameValuePair = splitString(pstring, ':');
                if (nameValuePair.size() == 2)
                {
                    set(nameValuePair[0], nameValuePair[1].toFloat(), true);
                    //Serial.printf("setting %s->%s to %.2f\n", this->name.c_str(), nameValuePair[0].c_str(), nameValuePair[1].toFloat());
                }
            }
        }

        // TODO this is finding all child components each time. Perhaps split data string that is handed to children?
        // find all component blocks and pass them to each valid component
        if (children.size() > 0)
        {
            //Serial.println("Finding child components");
            while (cd[p] != '\0')
            {
                if (findNextIndexOf(cd, '*', p) != -1)
                {
                    String childComponentName = "";
                    if (findNextIndexOf(cd, '{', p, &childComponentName) != -1)
                    {
                        //Serial.printf("searching for child '%s'\n", childComponentName.c_str());
                        for (auto child : children)
                        {
                            if (child->name == childComponentName)
                            {
                                child->fromString(data);
                            }
                        }
                    }
                }
            }
        }
        this->update();
    }

    PSParameter *addParameter(String name, float value, float rangeMin, float rangeMax)
    {
        PSParameter *newp = new PSParameter(name, value, rangeMin, rangeMax);
        params.push_back(newp);
        return newp;
    }

    PSComponent *addChild(PSComponent *child)
    {
        children.push_back(child);
        child->parent = this;
        return child;
    }

    PSComponent *childComponent(String name)
    {
        for (auto child : children)
        {
            if (child->name == name)
                return child;
        }
        return nullptr;
    }

    virtual void attachControllers(Controls *c) {}
    virtual void detachControllers() {}
    virtual bool update() {return false; }
    virtual void updateAudioStreamComponent() {}

protected:
    AudioConnectionVector _connections;
};

typedef std::vector<PSComponent *> PSComponentVector;

#endif
