#pragma once
#include <unordered_map>
#include <string>
#include "PSKeys.h"
#include "PSObject.h"

class PSObjectCollection
{
protected:
public:
    std::unordered_map<PSK, PSObject *> items;

    template <typename T>
    T *addItem(T *obj)
    {
        if (obj)
        {
            const PSK &key = obj->key;
            items[key] = obj;
        }
        return obj;
    }

    template <typename T>
    T *getItem(const PSK &key)
    {
        auto item = items.find(key);
        if (item != items.end())
            return (T *)item->second;
        return nullptr;
    }

    virtual ~PSObjectCollection()
    {
        for (auto entry : items)
        {
            if (entry.second)
                delete entry.second;
        }
    }

    virtual bool exists(const PSK &key) { return getItem<PSObject>(key); }
    // virtual PSObject *byKey(const PSK &key) { return getItem<PSObject>(key); }

    virtual bool update()
    {
        bool result = false;
        for (auto item : items)
            result &= item.second->update();

        return result;
    }
};

// class PSKeys : public PSObjectCollection
// {
// public:
//     void test()
//     {
//     }
// } psk;

// void testKeys2()
// {
//     //psk.addItem(new PSObject());
// }