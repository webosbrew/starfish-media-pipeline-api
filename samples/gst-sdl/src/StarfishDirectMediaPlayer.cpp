#include "StarfishDirectMediaPlayer.h"

#include <functional>
#include <iostream>

StarfishDirectMediaPlayer::StarfishDirectMediaPlayer(const std::string &appId) : mAppId(appId)
{
    mStarfishMediaAPIs.reset(new StarfishMediaAPIs());
    mAcbClient.reset(new Acb());
    if (!mAcbClient)
    {
        return;
    }

    auto handler = std::bind(&StarfishDirectMediaPlayer::AcbHandler, this, mAcbClient->getAcbId(),
                             std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                             std::placeholders::_4, std::placeholders::_5);
    if (!mAcbClient->initialize(ACB::PlayerType::MSE, appId, handler))
    {
        printf("StarfishDirectMediaPlayer.acb initialize failed\n");
        return;
    }
    printf("StarfishDirectMediaPlayer.acb success\n");
}

StarfishDirectMediaPlayer::~StarfishDirectMediaPlayer()
{
    mAcbClient->setState(ACB::AppState::FOREGROUND, ACB::PlayState::UNLOADED);
    mAcbClient->finalize();

    mStarfishMediaAPIs.reset(NULL);
}

bool StarfishDirectMediaPlayer::Open(DirectMediaAudioConfig audioConfig, DirectMediaVideoConfig videoConfig)
{
    return false;
}

bool StarfishDirectMediaPlayer::Feed(void *data, size_t size, uint64_t pts, DirectMediaFeedType type)
{
    return false;
}

void StarfishDirectMediaPlayer::Close()
{
}

void StarfishDirectMediaPlayer::AcbHandler(long acb_id, long task_id, long event_type, long app_state, long play_state,
                                           const char *reply)
{
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

bool StarfishDirectMediaPlayer_Feed(StarfishDirectMediaPlayer *ctx, void *data, size_t size, uint64_t pts, DirectMediaFeedType type)
{
    return ctx->Feed(data, size, pts, type);
}

bool StarfishDirectMediaPlayer_Open(StarfishDirectMediaPlayer *ctx, DirectMediaAudioConfig audioConfig, DirectMediaVideoConfig videoConfig)
{
    return ctx->Open(audioConfig, videoConfig);
}

void StarfishDirectMediaPlayer_Close(StarfishDirectMediaPlayer *ctx)
{
    ctx->Close();
}
