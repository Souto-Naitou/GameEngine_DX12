#pragma once

#include <xaudio2.h>
#include <fstream>
#include <wrl.h>

#include <list>
#include <memory>

#include <Common/HRESULT_ASSERT.h>

#pragma comment(lib, "xaudio2.lib")

struct ChunkHeader
{
    char id[4];
    DWORD size;
};

struct RiffHeader
{
    ChunkHeader chunkHeader;
    char format[4];
};

struct FormatChunk
{
    ChunkHeader chunkHeader;
    WAVEFORMATEX wfex;
};

struct SoundData
{
    WAVEFORMATEX wfex;
    std::unique_ptr<BYTE[]> pBuffer;
    DWORD bufferSize;
};

class Audio
{
public:
    void Initialize();
    void Finalize();

    void Unload(SoundData* soundData);
    void Play(bool isLoop = false);
    void SetVolume(float volume);
    float GetVolume() const;


public:
    void SetSoundData(SoundData* _soundData) { soundData_ = _soundData; }
    void SetXAudio2(IXAudio2* _pXAudio2) { pXAudio2_ = _pXAudio2; }


private:
    HRESULT_ASSERT hr_ = {};
    SoundData* soundData_ = nullptr;
    IXAudio2* pXAudio2_ = nullptr;
    IXAudio2SourceVoice* pCurrentSourceVoice_ = nullptr;
};
