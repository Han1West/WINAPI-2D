#pragma once


struct Vec2
{
	float x;
	float y;


	Vec2& operator = (POINT _pt)
	{
		x = (float)_pt.x;
		y = (float)_pt.y;
		return *this;
	}

	float Length()
	{
		return sqrt(x * x + y * y);
	}


	float Distance(Vec2 _vOther)
	{
		return sqrt(pow(_vOther.x - x, 2) + pow(_vOther.y - y, 2));
	}

	void Normalize()
	{		
		float fLen = Length();
		assert(fLen);
		x /= fLen;
		y /= fLen;
	}

	bool operator == (Vec2 _Other)
	{
		if (x == _Other.x && y == _Other.y)
			return true;
		return false;
	}

	bool operator != (Vec2 _Other)
	{
		if (x == _Other.x && y == _Other.y)
			return false;
		return true;
	}

	Vec2 operator -()
	{
		return Vec2(-x, -y);
	}


	Vec2 operator + (Vec2 _Other)
	{
		return Vec2(x + _Other.x, y + _Other.y);
	}

	void operator += (Vec2 _Other)
	{
		x += _Other.x;
		y += _Other.y;
	}

	Vec2 operator + (float _f)
	{
		return Vec2(x + _f, y + _f);	
	}

	Vec2 operator - (Vec2 _Other)
	{
		return Vec2(x - _Other.x, y - _Other.y);
	}

	void operator -= (Vec2 _Other)
	{
		x -= _Other.x;
		y -= _Other.y;
	}

	Vec2 operator - (float _f)
	{
		return Vec2(x - _f, y - _f);
	}



	Vec2 operator * (Vec2 _Other)
	{
		return Vec2(x * _Other.x, y * _Other.y);
	}

	void operator *= (Vec2 _Other)
	{
		x *= _Other.x;
		y *= _Other.y;
	}

	Vec2 operator * (float _f)
	{
		return Vec2(x * _f, y * _f);		
	}

	void operator *= (float _f)
	{
		x *= _f;
		y *= _f;
	}


	Vec2 operator / (Vec2 _Other)
	{
		return Vec2(x / _Other.x, y / _Other.y);
	}

	void operator /= (Vec2 _Other)
	{
		assert(_Other.x && _Other.y);

		x /= _Other.x;
		y /= _Other.y;
	}

	Vec2 operator / (float _f)
	{
		assert(_f);
		return Vec2(x / _f, y / _f);	
	}

	bool operator <= (Vec2 _Other)
	{
		if (x <= _Other.x && y <= _Other.y)
			return true;
		else
			return false;
	}


public:
	Vec2()
		: x(0.f)
		, y(0.f)
	{}

	Vec2(float _x, float _y)
		: x(_x)
		, y(_y)
	{}
};


struct tTask
{
	TASK_TYPE	TaskType;
	DWORD_PTR	Param_0;
	DWORD_PTR	Param_1;
};

struct tDebugShapeInfo
{
	DEBUG_SHAPE Shape;
	Vec2		Position;
	Vec2		Scale;
	PEN_TYPE	Pen;
	BRUSH_TYPE	BRUSH;
	float		Duration;	// DebugShape 유지 시간
	float		Time;		// 현재 진행 시간
};


struct tLog
{
	wstring		strLog; // 출력할 메세지	
	LOG_LEVEL	Level;	// 로그 수준
	float		Time;	// 현재 시간
};

struct tMonsterInfo
{
	float   HP;
	float	MaxHP;

	float	Speed;

	float	DetectRange;	
	float	AttackRange;
};