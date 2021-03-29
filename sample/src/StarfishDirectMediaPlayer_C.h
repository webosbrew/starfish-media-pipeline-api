#include "StarfishDirectMediaPlayer.h"

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#include <stdbool.h>
#define EXTERNC
#endif

EXTERNC StarfishDirectMediaPlayer *StarfishDirectMediaPlayer_Create(const char *appId);
EXTERNC bool StarfishDirectMediaPlayer_Open(StarfishDirectMediaPlayer *ctx, DirectMediaAudioConfig *audioConfig, DirectMediaVideoConfig *videoConfig, const char *windowId);
EXTERNC bool StarfishDirectMediaPlayer_Feed(StarfishDirectMediaPlayer *ctx, const void *data, size_t size, uint64_t pts, DirectMediaFeedType type);
EXTERNC size_t StarfishDirectMediaPlayer_Flush(StarfishDirectMediaPlayer *ctx);
EXTERNC void StarfishDirectMediaPlayer_Close(StarfishDirectMediaPlayer *ctx);
EXTERNC void StarfishDirectMediaPlayer_Destroy(StarfishDirectMediaPlayer *ctx);