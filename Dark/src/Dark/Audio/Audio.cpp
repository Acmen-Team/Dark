#include "dkpch.h"

#include "Audio.h"

#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"
#include "minimp3_ex.h"

static mp3dec_t mp3d;

namespace Dark {

  void Audio::InitDevice()
  {
    deviceName = alcGetString(nullptr, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);

    if (deviceName != nullptr)
    {

      device = alcOpenDevice(nullptr);

      if (device != nullptr)
      {
        context = alcCreateContext(device, nullptr);
        alcMakeContextCurrent(context);
      }

      alGetError();

      ALenum error = alGetError();
      if (error != AL_NO_ERROR)
      {
        DK_CORE_ERROR("alGenBuffers : {0}", error);
      }

      mp3dec_init(&mp3d);
    }
  }

  void Audio::SetSound(const std::string& path)
  {
    /*typedef struct
{
	int frame_bytes;
	int channels;
	int hz;
	int layer;
	int bitrate_kbps;
} mp3dec_frame_info_t;*/

    mp3dec_file_info_t info;
    int loadResult = mp3dec_load(&mp3d, path.c_str(), &info, NULL, NULL);
    uint32_t size  = info.samples * sizeof(mp3d_sample_t);

    auto sampleRate = info.hz;
    auto channels   = info.channels;
    ALenum alFormat;
    switch (channels)
    {
    case 1:
      alFormat = AL_FORMAT_MONO16;
    case 2:
      alFormat = AL_FORMAT_STEREO16;
    }

    float lengthSeconds = size / (info.avg_bitrate_kbps * 1024.0f);

    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, alFormat, info.buffer, size, sampleRate);

    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);

    if (alGetError() != AL_NO_ERROR)
      DK_CORE_ERROR("Failed to setup sound source");
  }

  void Audio::PlaySound()
  {
    m_IsPlay = true;
    alSourcePlay(source);
  }

  void Audio::StopSound()
  {
    m_IsPlay = false;
    alSourceStop(source);
  }

} // namespace Dark