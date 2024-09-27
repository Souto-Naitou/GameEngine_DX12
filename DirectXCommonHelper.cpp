#include "DirectXCommonHelper.h"
#include "Logger.h"
#include <format>
#include <cassert>

void CreateDevice(Microsoft::WRL::ComPtr<ID3D12Device>& _device, Microsoft::WRL::ComPtr<IDXGIAdapter4>& _adapter)
{
	/// D3D12Device�̐���

	// �@�\���x���ƃ��O�o�͗p�̕�����
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_2, D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] = { "12.2", "12.1", "12.0" };
	// �������ɐ����ł��邩�����Ă���
	for (size_t i = 0; i < _countof(featureLevels); ++i)
	{
		// �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		HRESULT hr = D3D12CreateDevice(_adapter.Get(), featureLevels[i], IID_PPV_ARGS(&_device));
		// �w�肵���@�\���x���Ńf�o�C�X�������ł��������m�F
		if (SUCCEEDED(hr))
		{
			// �����ł����̂Ń��O�o�͂��s���ă��[�v�𔲂���
			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}
	// �f�o�C�X�̐��������܂������Ȃ������̂ŋN���ł��Ȃ�
	assert(_device && "�f�o�C�X�̐����Ɏ��s");
	Log("Complete create D3D12Device!!!\n"); // �����������̃��O���o��
}

void PauseError(Microsoft::WRL::ComPtr<ID3D12Device>& _device, Microsoft::WRL::ComPtr<ID3D12InfoQueue>& _infoQ)
{
#ifdef _DEBUG
	if (SUCCEEDED(_device->QueryInterface(IID_PPV_ARGS(&_infoQ))))
	{
		// ��΂��G���[���Ɏ~�܂�@
		_infoQ->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		// �G���[���Ɏ~�܂� <- ����Y�ꂪ����������A�R�����g�A�E�g
		_infoQ->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//�x�����Ɏ~�܂�
		_infoQ->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		// �J��
		_infoQ->Release();

		// �}�����郁�b�Z�[�W��ID
		D3D12_MESSAGE_ID denyIds[] = {
			// Windows11�ł�DXGI�f�o�b�O���C���[��DX12�f�o�b�O���C���[�̑��ݍ�p�o�O�ɂ��G���[���b�Z�[�W
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};

		// �}�����郌�x��
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		// �w�肵�����b�Z�[�W�̕\���𐧌�����
		_infoQ->PushStorageFilter(&filter);
	}
#endif // _DEBUG
}
