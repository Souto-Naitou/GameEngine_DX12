#include "Audio.h"

#include "AudioManager.h"

#include <fstream>
#include <cassert>
#include <memory>

void Audio::Initialize()
{
}

void Audio::Finalize()
{

}

void Audio::Unload(SoundData* soundData)
{
    soundData->pBuffer.reset();
    soundData->bufferSize = 0;
    soundData->wfex = {};

    return;
}

void Audio::Play(bool isLoop)
{
    /// SourceVoice作成
    hr_ = pXAudio2_->CreateSourceVoice(&pCurrentSourceVoice_, &soundData_->wfex);

    /// バッファ設定
    XAUDIO2_BUFFER buffer = {};
    buffer.pAudioData = soundData_->pBuffer.get();
    buffer.AudioBytes = soundData_->bufferSize;
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.LoopCount = isLoop ? XAUDIO2_LOOP_INFINITE : 0;

    /// 再生
    hr_ = pCurrentSourceVoice_->SubmitSourceBuffer(&buffer);
    hr_ = pCurrentSourceVoice_->Start(0);

    AudioManager::GetInstance()->AddSourceVoice(pCurrentSourceVoice_);
}

void Audio::SetVolume(float volume)
{
    assert(pCurrentSourceVoice_ != nullptr && "SourceVoice is not initialized.");
    hr_ = pCurrentSourceVoice_->SetVolume(volume);
}

float Audio::GetVolume() const
{
    assert(pCurrentSourceVoice_ != nullptr && "SourceVoice is not initialized.");
    float volume = 0.0f;
    pCurrentSourceVoice_->GetVolume(&volume);
    return volume;
}
