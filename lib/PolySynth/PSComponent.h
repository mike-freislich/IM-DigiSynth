#ifndef PS_COMPONENT_H
#define PS_COMPONENT_H

#include <Arduino.h>
#include "PSParameter.h"
#include "patching.h"
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
            if (param->name == parameterName)
            {
                param->setValue(v, force);                
                return true;
            }
        }
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
    void fromString(const String &data, int startIndex = 0)
    {
#pragma region find first thisComponent in the data
        // Serial.printf("----> INSIDE COMPONENT '%s' <---\n", name.c_str());
        String componentName;
        const char *cd = data.c_str();
        int p = startIndex;
        while (componentName != this->name)
        {
            componentName = "";
            if (findNextIndexOf(cd, '*', p) != -1)
                if (findNextIndexOf(cd, '{', p, &componentName) == -1)
                    return; // bail if there's no block start after the component name
        }
        int endIndex = findBlockEnd(cd, p);
        // Serial.println(data.substring(startIndex, endIndex));
        // Serial.printf("Component Block start %d, end %d\n", p, endIndex);
        if (endIndex < 0)
        {
            // Serial.println(F("Error loading data.. runaway component block"));
            return;
        }
        // Serial.printf(">>>>>> %s <<<<<<\n", componentName.c_str());
        p++;
        while (isWhitespace(cd[p]) && p < endIndex)
            p++;

#pragma endregion

#pragma region getParameters for thisComponent

        // Get Parameter Block
        int nextComponent = p;
        int endParameterBlock = p;
        findNextIndexOf(cd, '*', nextComponent);
        findNextIndexOf(cd, '}', endParameterBlock);
        if (endParameterBlock == endIndex)
            return;
        // Serial.printf("nextComponent %d, endParameterBlock %d\n", nextComponent, endParameterBlock);=
        String pblock = data.substring(p, min(nextComponent, endParameterBlock));
        // Serial.printf("allocated pblock with %d characters\n", pblock.length());
        if (pblock.length())
        {
            p = min(nextComponent, endParameterBlock);
            // Serial.printf("PARAMETER BLOCK : %s\n", pblock.c_str());

            // get Parameters
            SplitStrings paramStrings = splitString(pblock, ',');
            for (auto pstring : paramStrings)
            {
                // Serial.printf("analyzing PARM: %s\n", pstring.c_str());
                SplitStrings nameValuePair = splitString(pstring, ':');
                if (nameValuePair.size() == 2)
                {
                    set(nameValuePair[0], nameValuePair[1].toFloat(), true);                    
                    // Serial.printf("setting %s->%s to %.2f\n", this->name.c_str(), nameValuePair[0].c_str(), nameValuePair[1].toFloat());
                }
            }
        }
#pragma endregion
        
        // find all component blocks and pass them to each valid component
        if (children.size() > 0)
        {
            // Serial.println("Finding child components");
            while (cd[p] != '\0' && p < endIndex)
            {
                if (findNextIndexOf(cd, '*', p) != -1)
                {
                    int compStartIndex = p;
                    String childComponentName = "";
                    if (findNextIndexOf(cd, '{', p, &childComponentName) != -1 && p < endIndex)
                    {
                        // Serial.printf("searching for child '%s'\n", childComponentName.c_str());
                        for (uint8_t c = 0; c < children.size(); c++)
                        {
                            PSComponent *child = children[c];
                            if (child->name == childComponentName)
                            {
                                child->fromString(data, compStartIndex);
                                break;
                            }
                        }
                    }
                }
            }
        }        
        this->updateFromControl();
    }
    

    PSParameter *addParameter(String name, float value, float rangeMin, float rangeMax, PSComponent *c, ParameterTargetC t, PSDisplayMode displayMode = asINTEGER)
    {
        PSParameter *newp = (new PSParameter(c, t))->init(name, value, rangeMin, rangeMax, displayMode);
        params.push_back(newp);
        return newp; 
    }

    PSParameter *addParameter(String name, float value, float rangeMin, float rangeMax, AudioStream *a, ParameterTarget t, PSDisplayMode displayMode = asINTEGER)
    {
        PSParameter *newp = (new PSParameter(a,t))->init(name, value, rangeMin, rangeMax, displayMode);
        params.push_back(newp);
        return newp;
    }

    PSParameter *addParameter(PSParameter * newp)
    {
        params.push_back(newp);
        return newp;
    }

    PSParameter *addParameter(String name, float value, float rangeMin, float rangeMax)
    {
        return addParameter(name, value, rangeMin, rangeMax, (AudioStream *)nullptr, nullptr);
    }

    FLASHMEM PSComponent *addChild(PSComponent *child)
    {
        children.push_back(child);
        child->parent = this;
        return child;
    }

    FLASHMEM PSComponent *childComponent(String name)
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
    
    virtual bool updateFromControl()
    {   
        bool result = false;
        for (auto parameter : params)
        {
            if (parameter != nullptr)
            {
                if (parameter->didChange())
                {
                    parameter->callTarget();
                    result = true;
                }
            }
        }

        if (controllersDidChange())
        {
            updateAudioStreamComponent();
            result = true;
        }        
        return result;
    }
    virtual void updateAudioStreamComponent() {}

protected:
    AudioConnectionVector _connections;

    FLASHMEM bool controllersDidChange()
    {
        for (auto p : params)
            if (p->didChange(true))
                return true;
        return false;
    }

    // searches a string e.g. = "{xxxx {} xxx { {}}  }"
    // to find the index of closing brace for the opening brace at the start
    FLASHMEM int findBlockEnd(const char *data, int startIndex)
    {
        int i = startIndex;
        if (findNextIndexOf(data, '{', i) < 0)
            return -1;
        int openCount = 1;
        char c = data[++i];
        while (c != '\0')
        {
            if (c == '{')
                openCount++;
            else if (c == '}')
                openCount--;
            if (openCount == 0)
                return ++i;
            c = data[++i];
        }
        return -1;
    }

    // string manipulation : ...
    // int &p = pass the start position within buffer
    //          p is incremented until findChar is discovered
    //
    // String *copyOut (optional)
    //          read from buffer into copyOut from position p, until the next findChar
    //
    // Returns: index of findChar in buffer (also represented in "p")
    //          -1 if findChar is not found (p will represent the null terminator)
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
};

typedef std::vector<PSComponent *> PSComponentVector;

#endif
