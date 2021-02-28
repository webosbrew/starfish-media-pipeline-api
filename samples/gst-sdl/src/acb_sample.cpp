#include <Acb.h>
#include <StarfishMediaAPIs.h>

#include <memory>

class Acb;
class StarfishMediaAPIs;

class AcbSample
{
public:
    AcbSample(/* args */);
    ~AcbSample();
    void AcbHandler(long acb_id, long task_id, long event_type, long app_state, long play_state,
                    const char *reply);

private:
    std::unique_ptr<Acb> acb_client_;
    std::unique_ptr<StarfishMediaAPIs> starfish_media_apis_;
    static void MPCallback(const gint type, const gint64 numValue, const gchar *strValue, void *data);
};

AcbSample::AcbSample(/* args */)
{
    starfish_media_apis_.reset(new StarfishMediaAPIs());
    acb_client_.reset(new Acb);

    auto handler = std::bind(&AcbSample::AcbHandler, this, acb_client_->getAcbId(),
                             std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                             std::placeholders::_4, std::placeholders::_5);
    acb_client_->initialize(ACB::PlayerType::AUDIO, "", handler);
    acb_client_->setMediaId("");
    acb_client_->setState(ACB::AppState::INITIALIZED, ACB::PlayState::UNLOADED);
    acb_client_->setDisplayWindow(0, 0, 0, 0, false);
    acb_client_->setCustomDisplayWindow(0, 0, 0, 0, 0, 0, 0, 0, false);
    acb_client_->setMediaAudioData("");
    acb_client_->setMediaVideoData("");
    acb_client_->setSubWindowInfo("");
    acb_client_->changePlayerType(ACB::PlayerType::DIRECT_AUDIO);
    acb_client_->connectDass();
    acb_client_->disconnectDass();
    acb_client_->setVsmInfo(ACB::StateSinkType::MAIN, ACB::DassAction::AUTO, ACB::VsmPurpose::NONE);
    acb_client_->startMute(true, true);
    acb_client_->stopMute(true, true);
    acb_client_->setSinkType(ACB::StateSinkType::MAIN);
    acb_client_->finalize();

    starfish_media_apis_->notifyBackground();
    starfish_media_apis_->notifyForeground();
    starfish_media_apis_->setVolume("");
    starfish_media_apis_->Play();
    starfish_media_apis_->Pause();
    starfish_media_apis_->pushEOS();
    starfish_media_apis_->Load("", AcbSample::MPCallback, nullptr);
    starfish_media_apis_->Seek("");
    starfish_media_apis_->setExternalContext(g_main_context_default());
}

AcbSample::~AcbSample()
{
}

void AcbSample::AcbHandler(long acb_id, long task_id, long event_type, long app_state, long play_state,
                           const char *reply)
{
}

void AcbSample::MPCallback(const gint type,
                           const gint64 numValue,
                           const gchar *strValue,
                           void *data)
{
}

extern "C" int gst_sample_initialize()
{
    return 0;
}

extern "C" int gst_sample_finalize()
{
    return 0;
}