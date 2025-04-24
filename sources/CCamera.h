#pragma once

enum FILTER_EFFECT
{
	FADE_IN,
	FADE_OUT,
	NONE,
};

struct FILTER_INFO
{
	FILTER_EFFECT	Effect;
	float			Duration;
	float			AccTime;
};


class CCamera
{
	SINGLE(CCamera);
private:
	Vec2		m_LookAt;		// 카메라가 보고있는 실제위치
	Vec2		m_LookAtDiff;	// 해상도 중심과, m_LookAt 과의 차이값을 계산 (실제 위치->렌더포즈, 렌더포즈->실제위치)
	Vec2		m_ShakeOffset;
	float		m_CamSpeed;

	// Filter Effect
	CTexture*			m_BlackFilterTex;
	list<FILTER_INFO>	m_FilterInfo;

	// Camera Oscillation
	float		m_Frequency;
	float		m_Amplitude;
	float		m_Duration;
	float		m_AccTime;
	float		m_Dir;
	bool		m_CamShake;

public:
	// Oscillation
	void SetCamShake(float _Frequency, float _Amplitude, float _Duration)
	{
		m_Frequency = _Frequency;
		m_Amplitude = _Amplitude;
		m_Duration = _Duration;
		m_Dir = 1.f;
		m_AccTime = 0.f;
		m_CamShake = true;
	}

	void SetFilterEffect(FILTER_EFFECT _Effect, float _Duration)
	{
		FILTER_INFO info = {};	

		info.Effect = _Effect;
		info.Duration = _Duration;
		info.AccTime = 0.f;

		m_FilterInfo.push_back(info);
	}

	void SetLookAt(Vec2 _Look) { m_LookAt = _Look; }
	Vec2 GetLookAt() { return m_LookAt + m_ShakeOffset; }
	Vec2 GetRenderPos(Vec2 _RealPos) { return _RealPos - m_LookAtDiff; }
	Vec2 GetRealPos(Vec2 _vRenderPos) {return _vRenderPos + m_LookAtDiff; }
	float GetCamSpeed() { return m_CamSpeed; }

	void ChangeLookAt(Vec2 _Look, bool IsAdd);

public:
	void Init();
	void Tick();
	void Render();

private:
	void CameraEffect();
};

