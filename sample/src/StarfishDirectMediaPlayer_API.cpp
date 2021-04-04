#include "StarfishDirectMediaPlayer_C.h"

struct StarfishMediaPlayerWrapper
{
    StarfishMediaPlayerWrapper(const char *appid)
    {
        player = new StarfishDirectMediaPlayer(appid);
    };
    ~StarfishMediaPlayerWrapper()
    {
        delete player;
    };

    StarfishDirectMediaPlayer *player;
};

StarfishDirectMediaPlayerHandle StarfishDirectMediaPlayer_Create(const char *appId)
{
    StarfishMediaPlayerWrapper *wrapper = new StarfishMediaPlayerWrapper(appId);
    return (StarfishDirectMediaPlayerHandle)wrapper;
}

void StarfishDirectMediaPlayer_Destroy(StarfishDirectMediaPlayerHandle hnd)
{
    StarfishMediaPlayerWrapper *wrapper = (StarfishMediaPlayerWrapper *)hnd;
    delete wrapper;
}

bool StarfishDirectMediaPlayer_Feed(StarfishDirectMediaPlayerHandle hnd, const void *data, size_t size, uint64_t pts, DirectMediaFeedType type)
{
    StarfishMediaPlayerWrapper *wrapper = (StarfishMediaPlayerWrapper *)hnd;
    return wrapper->player->Feed((void *)data, size, pts, type);
}

size_t StarfishDirectMediaPlayer_Flush(StarfishDirectMediaPlayerHandle hnd)
{
    StarfishMediaPlayerWrapper *wrapper = (StarfishMediaPlayerWrapper *)hnd;
    return wrapper->player->Flush();
}

bool StarfishDirectMediaPlayer_Open(StarfishDirectMediaPlayerHandle hnd, DirectMediaAudioConfig *audioConfig, DirectMediaVideoConfig *videoConfig, const char *windowId)
{
    StarfishMediaPlayerWrapper *wrapper = (StarfishMediaPlayerWrapper *)hnd;
    return wrapper->player->Open(audioConfig, videoConfig, windowId);
}

void StarfishDirectMediaPlayer_Close(StarfishDirectMediaPlayerHandle hnd)
{
    StarfishMediaPlayerWrapper *wrapper = (StarfishMediaPlayerWrapper *)hnd;
    wrapper->player->Close();
}