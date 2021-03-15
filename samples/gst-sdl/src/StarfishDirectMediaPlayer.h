#pragma once

#include "DirectMediaPlayer.h"

#ifdef __cplusplus

#if USE_ACB
#include <Acb.h>
#endif
#include <StarfishMediaAPIs.h>

#include <memory>

#if USE_ACB
class Acb;
#endif
class StarfishMediaAPIs;

class StarfishDirectMediaPlayer : public DirectMediaPlayer
{
public:
    StarfishDirectMediaPlayer(const std::string &appId);
    ~StarfishDirectMediaPlayer();
    bool Open(DirectMediaAudioConfig *audioConfig, DirectMediaVideoConfig *videoConfig);
    bool Feed(void *data, size_t size, uint64_t pts, DirectMediaFeedType type);
    bool SetVolume(int level);
    void Close();

private:
    std::string mAppId;
#if USE_ACB
    std::unique_ptr<Acb> mAcbClient;
#endif
    std::unique_ptr<StarfishMediaAPIs> mStarfishMediaAPIs;

    DirectMediaAudioConfig *mAudioConfig;
    DirectMediaVideoConfig *mVideoConfig;

    std::string MakeLoadPayload(uint64_t time);
    void AcbHandler(long acb_id, long task_id, long event_type, long app_state, long play_state,
                    const char *reply);
    static void LoadCallback(gint type, gint64 numValue, const gchar *strValue, void *data);
    const char *mWindowId;
};
#else
typedef struct StarfishDirectMediaPlayer StarfishDirectMediaPlayer;
#endif

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC StarfishDirectMediaPlayer *StarfishDirectMediaPlayer_Create(const char *appId);
EXTERNC bool StarfishDirectMediaPlayer_Open(StarfishDirectMediaPlayer *ctx, DirectMediaAudioConfig *audioConfig, DirectMediaVideoConfig *videoConfig);
EXTERNC bool StarfishDirectMediaPlayer_Feed(StarfishDirectMediaPlayer *ctx, void *data, size_t size, uint64_t pts, DirectMediaFeedType type);
EXTERNC void StarfishDirectMediaPlayer_Close(StarfishDirectMediaPlayer *ctx);
EXTERNC void StarfishDirectMediaPlayer_Destroy(StarfishDirectMediaPlayer *ctx);