#include <Acb.h>

#include <memory>

class Acb;

class AcbSample
{
public:
    AcbSample(/* args */);
    ~AcbSample();
    void AcbHandler(long acb_id, long task_id, long event_type, long app_state, long play_state,
                    const char *reply);

private:
    std::unique_ptr<Acb> acb_client_;
};

AcbSample::AcbSample(/* args */)
{
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
}

AcbSample::~AcbSample()
{
}

void AcbSample::AcbHandler(long acb_id, long task_id, long event_type, long app_state, long play_state,
                           const char *reply)
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