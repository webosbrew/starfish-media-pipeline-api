#pragma once

#include <string>
#include <glib-2.0/glib.h>

class StarfishMediaAPIs
{
public:
    StarfishMediaAPIs(const char *appId = nullptr);
    ~StarfishMediaAPIs();

    std::string Feed(const char *payload);
    uint flush();
    uint flush(const char *payload);
    void getCurrentPlaytime();
    const char *getMediaID();
    const char *getVolume();
    bool Load(const char *payload, void(callback)(gint type, gint64 numValue, const gchar *strValue));
    bool Load(const char *payload, void(callback)(gint type, gint64 numValue, const gchar *strValue, void *data), void *data);
    bool notifyBackground();
    bool notifyForeground();
    bool Pause();
    bool Play();
    bool pushEOS();
    bool Seek(const char *millis);
    bool setTimeToDecode(const char *);
    bool setVolume(const char *);
    bool Unload();
    void setExternalContext(GMainContext *);
    bool unsetExternalContext();
};

enum PF_EVENT_T
{
    PF_EVENT_TYPE_FRAMEREADY = 0x0,
    PF_EVENT_TYPE_STR_STREAMING_INFO_PERI = 0x1,
    PF_EVENT_TYPE_INT_BUFFER_RANGE_INFO = 0x2,
    PF_EVENT_TYPE_INT_DURATION = 0x3,
    PF_EVENT_TYPE_STR_VIDEO_INFO = 0x4,
    PF_EVENT_TYPE_STR_VIDEO_TRACK_INFO = 0x5,
    PF_EVENT_TYPE_STR_AUDIO_INFO = 0x7,
    PF_EVENT_TYPE_STR_AUDIO_TRACK_INFO = 0x8,
    PF_EVENT_TYPE_STR_SUBT_TRACK_INFO = 0x9,
    PF_EVENT_TYPE_STR_BUFF_EVENT = 0xa,
    PF_EVENT_TYPE_STR_SOURCE_INFO = 0xb,
    PF_EVENT_TYPE_INT_ERROR = 0x12,
    PF_EVENT_TYPE_ANOTHER_ERROR = 0x13,
    PF_EVENT_TYPE_STR_STATE_UPDATE__PRELOADCOMPLETED = 0x15,
    PF_EVENT_TYPE_STR_STATE_UPDATE__LOADCOMPLETED = 0x16,
    PF_EVENT_TYPE_STR_STATE_UPDATE__UNLOADCOMPLETED = 0x17,
    PF_EVENT_TYPE_STR_STATE_UPDATE__TRACKSELECTED = 0x18,
    PF_EVENT_TYPE_STR_STATE_UPDATE__SEEKDONE = 0x19,
    PF_EVENT_TYPE_STR_STATE_UPDATE__PLAYING = 0x1a,
    PF_EVENT_TYPE_STR_STATE_UPDATE__PAUSED = 0x1b,
    PF_EVENT_TYPE_STR_STATE_UPDATE__ENDOFSTREAM = 0x1c,
    PF_EVENT_TYPE_STR_CUSTOM = 0x1d,
    PF_EVENT_TYPE_INT_NEED_DATA = 0x26,
    PF_EVENT_TYPE_INT_ENOUGH_DATA = 0x27,
    PF_EVENT_TYPE_INT_SVP_VDEC_READY = 0x2b,
    PF_EVENT_TYPE_INT_BUFFERLOW = 0x2c,
    PF_EVENT_TYPE_INT_DROPPED_FRAME = 0x2e,
    PF_EVENT_TYPE_STR_BUFFERFULL = 0x2d,
    PF_EVENT_TYPE_STR_BUFFERLOW = 0x2e,
    PF_EVENT_TYPE_STR_DROPPED_FRAME = 0x30,
};