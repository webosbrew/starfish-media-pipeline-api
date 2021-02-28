#pragma once
#include <stddef.h>
#include <stdint.h>

struct DirectMediaAudioConfig
{
};

struct DirectMediaVideoConfig
{
    unsigned int width, height;
};

enum DirectMediaFeedType
{
    DirectMediaFeedAudio,
    DirectMediaFeedVideo
};

#ifdef __cplusplus
class DirectMediaPlayer
{
public:
    void Open(DirectMediaAudioConfig audioConfig, DirectMediaVideoConfig videoConfig);
    bool Feed(void *data, size_t size, uint64_t pts, DirectMediaFeedType type);
    void Close();
};
#else
typedef struct DirectMediaAudioConfig DirectMediaAudioConfig;
typedef struct DirectMediaVideoConfig DirectMediaVideoConfig;
typedef enum DirectMediaFeedType DirectMediaFeedType;
#endif
