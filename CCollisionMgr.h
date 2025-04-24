#pragma once

union COLLIDER_ID
{
	DWORD_PTR ID;

	struct
	{
		UINT LeftID;
		UINT RightID;
	};
};



class CCollider;

class CCollisionMgr
{
	SINGLE(CCollisionMgr);
private:
	UINT					m_Matrix[(UINT)LAYER_TYPE::END];
	vector<CCollider*>		m_vecCollider[(int)LAYER_TYPE::END];
	map<DWORD_PTR, bool>	m_ColInfo;

	Vec2 m_CollDir;

public:
	void CollisionCheckClear() { memset(m_Matrix, 0, sizeof(UINT) * (UINT)LAYER_TYPE::END); }
	void CollisionCheck(UINT _Layer1, UINT _Layer2, bool _Check);
	void RegisterCollider(CCollider* _Collider, LAYER_TYPE _LayerType)
	{
		m_vecCollider[(int)_LayerType].push_back(_Collider);
	}

private:
	void CollisionLayer(int Left, int Right);
	bool Collision(CCollider* _Left, CCollider* _Right);


public:
	void Init();
	void Tick();
};

