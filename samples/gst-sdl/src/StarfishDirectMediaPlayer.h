#pragma once

#include "DirectMediaPlayer.h"

#ifdef __cplusplus

#include <Acb.h>
#include <StarfishMediaAPIs.h>

#include <memory>

class Acb;
class StarfishMediaAPIs;

class StarfishDirectMediaPlayer : public DirectMediaPlayer
{
public:
    StarfishDirectMediaPlayer(const std::string &appId);
    ~StarfishDirectMediaPlayer();
    bool Open(DirectMediaAudioConfig audioConfig, DirectMediaVideoConfig videoConfig);
    bool Feed(void *data, size_t size, uint64_t pts, DirectMediaFeedType type);
    void Close();

private:
    std::string mAppId;
    std::unique_ptr<Acb> mAcbClient;
    std::unique_ptr<StarfishMediaAPIs> mStarfishMediaAPIs;

    void AcbHandler(long acb_id, long task_id, long event_type, long app_state, long play_state,
                    const char *reply);
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
EXTERNC bool StarfishDirectMediaPlayer_Open(StarfishDirectMediaPlayer *ctx, DirectMediaAudioConfig audioConfig, DirectMediaVideoConfig videoConfig);
EXTERNC bool StarfishDirectMediaPlayer_Feed(StarfishDirectMediaPlayer *ctx, void *data, size_t size, uint64_t pts, DirectMediaFeedType type);
EXTERNC void StarfishDirectMediaPlayer_Close(StarfishDirectMediaPlayer *ctx);
EXTERNC void StarfishDirectMediaPlayer_Destroy(StarfishDirectMediaPlayer *ctx);