#pragma once
#include <stddef.h>
#include <stdint.h>

enum DirectMediaAudioCodec
{
    DirectMediaAudioCodecUnknown = 0,
    DirectMediaAudioCodecAAC,
    DirectMediaAudioCodecAC3,
    DirectMediaAudioCodecEAC3,
    DirectMediaAudioCodecOpus,
    DirectMediaAudioCodecCount,
};

static const char *DirectMediaAudioCodecNames[DirectMediaAudioCodecCount] = {
    "",
    "AAC",
    "AC3",
    "AC3 PLUS",
    "OPUS",
};

enum DirectMediaVideoCodec
{
    DirectMediaVideoCodecUnknown = 0,
    DirectMediaVideoCodecH264,
    DirectMediaVideoCodecHEVC,
    DirectMediaVideoCodecCount,
};

static const char *DirectMediaVideoCodecNames[DirectMediaVideoCodecCount] = {
    "",
    "H264",
    "HEVC",
};

enum DirectMediaFeedType
{
    DirectMediaFeedVideo = 0x1,
    DirectMediaFeedAudio = 0x2,
};

#ifndef __cplusplus
typedef enum DirectMediaAudioCodec DirectMediaAudioCodec;
typedef enum DirectMediaVideoCodec DirectMediaVideoCodec;
typedef enum DirectMediaFeedType DirectMediaFeedType;
#endif

struct DirectMediaAudioConfig
{
    DirectMediaAudioCodec codec;
    int32_t channels;
    int32_t bitsPerChannel;
    int32_t samplesPerSecond;
};

struct DirectMediaVideoConfig
{
    DirectMediaVideoCodec codec;
    unsigned int width, height;
};

#ifndef __cplusplus
typedef struct DirectMediaAudioConfig DirectMediaAudioConfig;
typedef struct DirectMediaVideoConfig DirectMediaVideoConfig;
#endif

#ifdef __cplusplus
class DirectMediaPlayer
{
public:
    void Open(DirectMediaAudioConfig *audioConfig, DirectMediaVideoConfig *videoConfig);
    bool Feed(void *data, size_t size, uint64_t pts, DirectMediaFeedType type);
    void Close();
};
#endif
