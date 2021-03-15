#include "gst_sample.h"
#include "main.h"

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "StarfishDirectMediaPlayer.h"

static GstElement *pipeline;
static GstBus *bus;
static StarfishDirectMediaPlayer *playerctx;
static DirectMediaAudioConfig *audioConfig;
static DirectMediaVideoConfig *videoConfig;
static bool audioEosEmitted, videoEosEmitted;

static void cb_message(GstBus *bus, GstMessage *message, gpointer user_data);

static void playerEos();
static bool playerOpen();

int gst_sample_create()
{
    playerctx = StarfishDirectMediaPlayer_Create(WEBOS_APPID);
    audioEosEmitted = false;
    videoEosEmitted = false;
    audioConfig = NULL;
    videoConfig = NULL;
    return 0;
}

int gst_sample_destroy()
{
    StarfishDirectMediaPlayer_Destroy(playerctx);
    return 0;
}

int gst_sample_initialize()
{
    GstStateChangeReturn ret;

    pipeline = gst_parse_launch("curlhttpsrc location=http://commondatastorage.googleapis.com/gtv-videos-bucket/sample/BigBuckBunny.mp4 ! qtdemux name=demux \
    demux.audio_0 ! queue ! aacparse ! audio/mpeg,mpegversion=4,stream-format=adts ! appsink name=audsink \
    demux.video_0 ! queue ! h264parse config-interval=-1 ! video/x-h264,stream-format=byte-stream,alignment=nal ! appsink name=vidsink",
                                NULL);

    g_assert(pipeline);

    GstElement *audiosink, *videosink;
    audiosink = gst_bin_get_by_name(GST_BIN(pipeline), "audsink");
    g_assert(audiosink);

    videosink = gst_bin_get_by_name(GST_BIN(pipeline), "vidsink");
    g_assert(videosink);

    GstAppSinkCallbacks audioCallbacks = {
        .eos = audioEos,
        .new_preroll = audioNewPreroll,
        .new_sample = audioNewSample,
    };
    gst_app_sink_set_callbacks(GST_APP_SINK(audiosink), &audioCallbacks, NULL, NULL);

    GstAppSinkCallbacks videoCallbacks = {
        .eos = videoEos,
        .new_preroll = videoNewPreroll,
        .new_sample = videoNewSample,
    };
    gst_app_sink_set_callbacks(GST_APP_SINK(videosink), &videoCallbacks, NULL, NULL);

    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_signal_watch(bus);
    g_signal_connect(bus, "message", (GCallback)cb_message, pipeline);

    /* Start playing */
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    return 0;
}

int gst_sample_finalize()
{
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    return 0;
}

void audioEos(GstAppSink *appsink, gpointer user_data)
{
    free(audioConfig);
    audioConfig = NULL;
    audioEosEmitted = true;
    playerEos();
}

GstFlowReturn audioNewPreroll(GstAppSink *appsink, gpointer user_data)
{
    GstSample *preroll = gst_app_sink_pull_preroll(appsink);
    GstCaps *caps = gst_sample_get_caps(preroll);
    GstStructure *cap = gst_caps_get_structure(caps, 0);
    int channels = 0, rate = 0;
    gst_structure_get_int(cap, "channels", &channels);
    gst_structure_get_int(cap, "rate", &rate);

    gst_sample_unref(preroll);

    DirectMediaAudioConfig config = {DirectMediaAudioCodecAAC, channels, 16, rate};
    audioConfig = malloc(sizeof(DirectMediaAudioConfig));
    memcpy(audioConfig, &config, sizeof(DirectMediaAudioConfig));
    if (!playerOpen())
    {
        return GST_FLOW_ERROR;
    }
    return GST_FLOW_OK;
}

GstFlowReturn audioNewSample(GstAppSink *appsink, gpointer user_data)
{
    GstSample *sample = gst_app_sink_pull_sample(appsink);

    GstBuffer *buf = gst_sample_get_buffer(sample);

    GstMapInfo info;
    gst_buffer_map(buf, &info, GST_MAP_READ);

    StarfishDirectMediaPlayer_Feed(playerctx, info.data, info.size, buf->pts, DirectMediaFeedAudio);

    gst_buffer_unmap(buf, &info);
    gst_sample_unref(sample);
    return GST_FLOW_OK;
}

void videoEos(GstAppSink *appsink, gpointer user_data)
{
    free(videoConfig);
    videoConfig = NULL;
    videoEosEmitted = true;
    playerEos();
}

GstFlowReturn videoNewPreroll(GstAppSink *appsink, gpointer user_data)
{
    GstSample *preroll = gst_app_sink_pull_preroll(appsink);
    GstCaps *caps = gst_sample_get_caps(preroll);
    GstStructure *cap = gst_caps_get_structure(caps, 0);

    int width, height;
    g_assert(gst_structure_get_int(cap, "width", &width));
    g_assert(gst_structure_get_int(cap, "height", &height));

    gst_sample_unref(preroll);

    DirectMediaVideoConfig config = {DirectMediaVideoCodecH264, width, height};
    videoConfig = malloc(sizeof(DirectMediaVideoConfig));
    memcpy(videoConfig, &config, sizeof(DirectMediaVideoConfig));
    if (!playerOpen())
    {
        return GST_FLOW_ERROR;
    }
    return GST_FLOW_OK;
}

GstFlowReturn videoNewSample(GstAppSink *appsink, gpointer user_data)
{
    GstSample *sample = gst_app_sink_pull_sample(appsink);

    GstBuffer *buf = gst_sample_get_buffer(sample);

    GstMapInfo info;
    gst_buffer_map(buf, &info, GST_MAP_READ);

    StarfishDirectMediaPlayer_Feed(playerctx, info.data, info.size, buf->pts, DirectMediaFeedVideo);

    gst_buffer_unmap(buf, &info);
    gst_sample_unref(sample);
    return GST_FLOW_OK;
}

/* called when a new message is posted on the bus */
void cb_message(GstBus *bus, GstMessage *message, gpointer user_data)
{
    GstElement *pipeline = GST_ELEMENT(user_data);

    switch (GST_MESSAGE_TYPE(message))
    {
    case GST_MESSAGE_ERROR:
        g_print("we received an error!\n");
        request_exit();
        break;
    case GST_MESSAGE_EOS:
        g_print("we reached EOS\n");
        request_exit();
        break;
    default:
        break;
    }
}

void playerEos()
{
    if (audioEosEmitted && videoEosEmitted)
    {
        StarfishDirectMediaPlayer_Close(playerctx);
    }
}

bool playerOpen()
{
    if (audioConfig && videoConfig)
    {
        GstState state;
        gst_element_get_state(pipeline, &state, NULL, 0);
        printf("StarfishDirectMediaPlayer_Open， GstState: %d\n", state);
        if (state != GST_STATE_READY)
        {
            return true;
        }
        return StarfishDirectMediaPlayer_Open(playerctx, audioConfig, videoConfig);
    }
    return true;
}