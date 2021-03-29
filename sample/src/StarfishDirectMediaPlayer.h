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
    bool Open(DirectMediaAudioConfig *audioConfig, DirectMediaVideoConfig *videoConfig, const char *windowId);
    bool Feed(void *data, size_t size, uint64_t pts, DirectMediaFeedType type);
    size_t Flush();
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

    std::string MakeLoadPayload(uint64_t time, const char *windowId);
    void AcbHandler(long acb_id, long task_id, long event_type, long app_state, long play_state,
                    const char *reply);
};
#else
typedef struct StarfishDirectMediaPlayer StarfishDirectMediaPlayer;
#endif