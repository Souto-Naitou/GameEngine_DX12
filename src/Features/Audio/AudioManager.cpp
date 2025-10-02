#include "AudioManager.h"

#include <Core/ConfigManager/ConfigManager.h>

#include <cassert>
#include <utility>

void AudioManager::Initialize()
{
    hr_ = XAudio2Create(&pXAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
    hr_ = pXAudio2_->CreateMasteringVoice(&pMasteringVoice_);

    pFilePathSearcher_ = std::make_unique<PathResolver>();
    pFilePathSearcher_->Initialize();

    // 設定ファイルからパスを取得して登録
    auto& cfgData = ConfigManager::GetInstance()->GetConfigData();
    for (auto& path : cfgData.audio_paths)
    {
        this->AddSearchPath(path);
    }
}

void AudioManager::Update()
{
    {
        auto itr = sourceVoices_.begin();
        while (itr != sourceVoices_.end())
        {
            IXAudio2SourceVoice* sv = *itr;
            XAUDIO2_VOICE_STATE state;
            sv->GetState(&state);
            if (state.BuffersQueued == 0)
            {
                sv->DestroyVoice();
                itr = sourceVoices_.erase(itr);
            }
            else
            {
                ++itr;
            }
        }
    }

}

void AudioManager::Finalize()
{
    pXAudio2_.Reset();
}

void AudioManager::AddSearchPath(const std::string& _path)
{
    pFilePathSearcher_->AddSearchPath(_path);
}

Audio* AudioManager::GetNewAudio(const std::string& category, const std::string& filename)
{
    auto& soundData = LoadWave(pFilePathSearcher_->GetFilePath(filename).c_str());

    /// Audio生成
    auto audio = std::make_unique<Audio>(pXAudio2_.Get(), &soundData);
    audio->Initialize();

    Audio* pAudio = audio.get();

    audioMap_[category].emplace_back(std::move(audio));

    return pAudio;
}

SoundData& AudioManager::LoadWave(const char* _filename)
{
    std::ifstream file(_filename, std::ios::binary);

    assert(file.is_open());

    /// RIFFチャンク読み込み
    RiffHeader riff = {};
    file.read(reinterpret_cast<char*>(&riff), sizeof(riff));
    /// riffチェック
    if (strncmp(riff.chunkHeader.id, "RIFF", 4) != 0)
    {
        assert(false);
    }
    /// Waveチェック
    if (strncmp(riff.format, "WAVE", 4) != 0)
    {
        assert(false);
    }


    /// Formatチャンク読み込み
    FormatChunk format = {};
    ChunkHeader chunkHeader = {};
    // ヘッダー読み込み
    while (file.read((char*)&chunkHeader, sizeof(chunkHeader))) {
        // チャンクIDが "fmt" か確認
        if (strncmp(chunkHeader.id, "fmt ", 4) == 0) {
            // Formatチャンクのサイズを確認、データを読み込む
            assert(chunkHeader.size <= sizeof(format.wfex));

            format.chunkHeader = chunkHeader; // チャンクヘッダーをコピー
            file.read((char*)&format.wfex, chunkHeader.size); // fmtのデータを読み込み

            break;
        }
        else {
            // 次のチャンクに移動
            file.seekg(chunkHeader.size, std::ios_base::cur);
        }
    }

    /// 拡張フォーマットが含まれている場合、その部分を処理
    if (format.chunkHeader.size > sizeof(format.wfex)) {
        // 残りのデータをスキップ
        file.seekg(format.chunkHeader.size - sizeof(format.wfex), std::ios::cur);
    }


    /// Dataチャンク読み込み
    ChunkHeader data = {};
    ChunkHeaderRead(file, data, "data");

    /// 波形データ読み込み
    std::unique_ptr<char[]> pBuffer = std::make_unique<char[]>(data.size);
    file.read(pBuffer.get(), data.size);


    /// ファイルクローズ
    file.close();


    soundDataMap_[_filename] = {};

    auto& soundData = soundDataMap_[_filename];

    soundData.wfex = format.wfex;
    soundData.pBuffer = std::make_unique<BYTE[]>(data.size);
    soundData.bufferSize = data.size;

    std::memcpy(soundData.pBuffer.get(), pBuffer.get(), data.size);

    return soundData;
}

void AudioManager::ChunkHeaderRead(std::ifstream& _file, ChunkHeader& _chunkHeader, const char* _target)
{
    while (true)
    {
        _file.read(reinterpret_cast<char*>(&_chunkHeader), sizeof(ChunkHeader));
        if (strncmp(_chunkHeader.id, _target, 4) != 0)
        {
            _file.seekg(_chunkHeader.size, std::ios::cur); // 現在のチャンクをスキップ
        }
        else
        {
            break;
        }
    }
}
