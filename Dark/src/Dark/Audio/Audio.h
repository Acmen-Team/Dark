#pragma once

namespace Dark {

  class Audio
  {
  public:
    //Audio();
    //~Audio();

    void InitDevice();
    void SetSound(const std::string& path);
    void PlaySound();
    void StopSound();

    bool GetIsPlay() { return m_IsPlay; }
  private:
    bool m_IsPlay{false};

    const ALchar* deviceName = nullptr;
    ALCdevice* device        = nullptr;
    ALCcontext* context      = nullptr;
    ALuint source;
  };

} // namespace Dark