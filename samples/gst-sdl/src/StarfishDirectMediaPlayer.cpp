#include "StarfishDirectMediaPlayer.h"

#include <algorithm>
#include <string>
#include <functional>
#include <iostream>

#include <pbnjson.hpp>

namespace pj = pbnjson;

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

bool StarfishDirectMediaPlayer::Open(DirectMediaAudioConfig *audioConfig, DirectMediaVideoConfig *videoConfig)
{
    printf("Audio: codec = %d, channels = %d, bitsPerChannel = %d, samplesPerSecond = %d\n", audioConfig->codec,
           audioConfig->channels, audioConfig->bitsPerChannel, audioConfig->samplesPerSecond);

    mAudioConfig = audioConfig;
    mVideoConfig = videoConfig;

    mStarfishMediaAPIs->setExternalContext(g_main_context_default());
    mStarfishMediaAPIs->notifyForeground();

    std::string payload = MakeLoadPayload(0);
    std::cout << payload << std::endl;
    if (!mStarfishMediaAPIs->Load(payload.c_str(), &StarfishDirectMediaPlayer::LoadCallback, this))
    {
        std::cout << "Load() failed!" << std::endl;
        return false;
    }
    return true;
}

bool StarfishDirectMediaPlayer::Feed(void *data, size_t size, uint64_t pts, DirectMediaFeedType type)
{
    return false;
}

void StarfishDirectMediaPlayer::Close()
{
    mAudioConfig = nullptr;
    mVideoConfig = nullptr;
}

std::string StarfishDirectMediaPlayer::MakeLoadPayload(uint64_t time)
{
    pj::JValue payload = pj::Object();
    pj::JValue args = pj::Array();
    pj::JValue arg = pj::Object();
    pj::JValue option = pj::Object();
    pj::JValue contents = pj::Object();
    pj::JValue externalStreamingInfo = pj::Object();
    pj::JValue codec = pj::Object();

    arg.put("mediaTransportType", "BUFFERSTREAM");
    option.put("appId", mAppId);
    pj::JValue adaptiveStreaming = pj::Object();
    adaptiveStreaming.put("maxWidth", 1920);
    adaptiveStreaming.put("maxHeight", 1080);
    adaptiveStreaming.put("maxFrameRate", 60);
    option.put("adaptiveStreaming", adaptiveStreaming);
    option.put("needAudio", true);

    codec.put("video", DirectMediaVideoCodecNames[mVideoConfig->codec]);
    codec.put("audio", DirectMediaAudioCodecNames[mAudioConfig->codec]);

    if (mAudioConfig->codec == DirectMediaAudioCodecOpus)
    {
        pj::JValue opusInfo = pj::Object();
        opusInfo.put("channels", std::to_string(mAudioConfig->channels));
        opusInfo.put("sampleRate", static_cast<double>(mAudioConfig->samplesPerSecond / 1000.f));
        contents.put("opusInfo", opusInfo);
    }

    contents.put("codec", codec);
    contents.put("format", "RAW");

    pj::JValue esInfo = pj::Object();
    esInfo.put("ptsToDecode", static_cast<int64_t>(time));
    esInfo.put("seperatedPTS", true);
    contents.put("esInfo", esInfo);

    externalStreamingInfo.put("contents", contents);
    option.put("externalStreamingInfo", externalStreamingInfo);
    arg.put("option", option);
    args.append(arg);
    payload.put("args", args);

    return pbnjson::JGenerator::serialize(payload, pbnjson::JSchemaFragment("{}"));
}

void StarfishDirectMediaPlayer::AcbHandler(long acb_id, long task_id, long event_type, long app_state, long play_state,
                                           const char *reply)
{
    printf("AcbHandler event_type: %d, app_state: %d, play_state: %d, reply: %s\n", event_type, app_state, play_state, reply);
}

void StarfishDirectMediaPlayer::LoadCallback(gint type, gint64 numValue, const gchar *strValue, void *data)
{
    printf("LoadCallback type: %d, numValue: %d, strValue: %s\n", type, numValue, strValue);
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

bool StarfishDirectMediaPlayer_Open(StarfishDirectMediaPlayer *ctx, DirectMediaAudioConfig *audioConfig, DirectMediaVideoConfig *videoConfig)
{
    return ctx->Open(audioConfig, videoConfig);
}

void StarfishDirectMediaPlayer_Close(StarfishDirectMediaPlayer *ctx)
{
    ctx->Close();
}
