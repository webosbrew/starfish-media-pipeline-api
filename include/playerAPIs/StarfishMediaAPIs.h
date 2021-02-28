#pragma once

#include <glib.h>

class StarfishMediaAPIs
{
public:
    StarfishMediaAPIs(const char *appId = nullptr);
    ~StarfishMediaAPIs();

    void Feed(const char *payload);
    uint flush();
    uint flush(const char *payload);
    void getCurrentPlaytime();
    const char *getMediaID();
    void getVolume();
    bool Load(const char *payload, void(callback)(gint type, gint64 numValue, const gchar *strValue, void *data), void *data);
    void notifyBackground();
    void notifyForeground();
    void Pause();
    void Play();
    void pushEOS();
    void Seek(const char *millis);
    void setTimeToDecode(const char *);
    void setVolume(const char *);
    void Unload();
    void setExternalContext(GMainContext *);
    void unsetExternalContext();
};