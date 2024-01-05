#ifndef PS_PATCHMANAGER_H
#define PS_PATCHMANAGER_H

#include "PSParameter.h"

class PSPatchManager
{
public:
    PSPatchManager() {
        
    }

    FLASHMEM bool savePatch(const char *filename, String data)
    {
        SD.begin(BUILTIN_SDCARD);

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
        String msg;
        SD.begin(BUILTIN_SDCARD);    
        msg = SD.mediaPresent() ? "success" : "failed to detect SD card";

        if (SD.exists(filename))
        {
            File patchFile = SD.open(filename);
            if (patchFile)
            {
                String data = patchFile.readString();
                patchFile.close();
                Serial.println("Patch Loaded");
                //printf("Loaded patch data: '%s'\n", data.c_str());
                return data;
            } else {
                msg = "Unable to open file";
            }
        } else {
            msg = "File does not exist";
        }
        printf("Failed to load patchfile '%s' due to error: %s\n", filename, msg.c_str());
        return "";
    }
};

PSPatchManager patchManager;

#endif