#include "StarfishDirectMediaPlayer.h"

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#include <stdbool.h>
#define EXTERNC
#endif

typedef void *StarfishDirectMediaPlayerHandle;

EXTERNC StarfishDirectMediaPlayerHandle StarfishDirectMediaPlayer_Create(const char *appId);
EXTERNC bool StarfishDirectMediaPlayer_Open(StarfishDirectMediaPlayerHandle, DirectMediaAudioConfig *audioConfig, DirectMediaVideoConfig *videoConfig, const char *windowId);
EXTERNC bool StarfishDirectMediaPlayer_Feed(StarfishDirectMediaPlayerHandle, const void *data, size_t size, uint64_t pts, DirectMediaFeedType type);
EXTERNC size_t StarfishDirectMediaPlayer_Flush(StarfishDirectMediaPlayerHandle);
EXTERNC void StarfishDirectMediaPlayer_Close(StarfishDirectMediaPlayerHandle);
EXTERNC void StarfishDirectMediaPlayer_Destroy(StarfishDirectMediaPlayerHandle);