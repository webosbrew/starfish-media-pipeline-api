#include "StarfishDirectMediaPlayer_C.h"

static void print_bytes(const void *ptr, int size)
{
    const unsigned char *p = (const unsigned char *)ptr;
    int i;
    for (i = 0; i < size; i++)
    {
        fprintf(stdout, "%02hhX ", p[i]);
    }
    fprintf(stdout, "\n");
}

bool StarfishDirectMediaPlayer_Feed(StarfishDirectMediaPlayer *ctx, const void *data, size_t size, uint64_t pts, DirectMediaFeedType type)
{
    if (type == DirectMediaFeedVideo)
        print_bytes(data, size > 16 ? 16 : size);
    return ctx->Feed((void *)data, size, pts, type);
}

size_t StarfishDirectMediaPlayer_Flush(StarfishDirectMediaPlayer *ctx)
{
    return ctx->Flush();
}

bool StarfishDirectMediaPlayer_Open(StarfishDirectMediaPlayer *ctx, DirectMediaAudioConfig *audioConfig, DirectMediaVideoConfig *videoConfig, const char *windowId)
{
    return ctx->Open(audioConfig, videoConfig, windowId);
}

void StarfishDirectMediaPlayer_Close(StarfishDirectMediaPlayer *ctx)
{
    ctx->Close();
}

StarfishDirectMediaPlayer *StarfishDirectMediaPlayer_Create(const char *appId)
{
    StarfishDirectMediaPlayer *player = new StarfishDirectMediaPlayer(appId);
    return player;
}

void StarfishDirectMediaPlayer_Destroy(StarfishDirectMediaPlayer *ctx)
{
    delete ctx;
}