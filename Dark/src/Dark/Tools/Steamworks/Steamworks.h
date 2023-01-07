#pragma once

namespace Dark {

  class Steamworks
  {
  public:
    Steamworks() = default;
    ~Steamworks() = default;

    void InitSteamAPI();
    void ShutdownSteamAPI();
  private:
  };

} // namespace Dark