#include "pch.h"
#include "func.h"

#include "CTaskMgr.h"
#include "CObj.h"
#include "CEffect.h"
#include "CAnimator.h"

void CreateObject(CObj* _Object, LAYER_TYPE _LayerType)
{
	tTask task = {};
	task.TaskType = TASK_TYPE::CREATE_OBJECT;
	task.Param_0 = (DWORD_PTR)_Object;
	task.Param_1 = (DWORD_PTR)_LayerType;

	CTaskMgr::Get()->AddTask(task);
}

void DeleteObject(CObj* _Object)
{
	tTask task = {};
	task.TaskType = TASK_TYPE::DELETE_OBJECT;
	task.Param_0 = (DWORD_PTR)_Object;
	CTaskMgr::Get()->AddTask(task);
}

void ChangeLevel(int _Type)
{
	tTask task = {};
	task.TaskType = TASK_TYPE::CHANGE_LEVEL;
	task.Param_0 = (DWORD_PTR)_Type;
	CTaskMgr::Get()->AddTask(task);
}

void RestartLevel(int _Type)
{
	tTask task = {};
	task.TaskType = TASK_TYPE::RESTART_LEVEL;
	task.Param_0 = (DWORD_PTR)_Type;
	CTaskMgr::Get()->AddTask(task);
}

#include "CDebugMgr.h"

void DrawDebugShape(DEBUG_SHAPE _Shape, PEN_TYPE _Pen, BRUSH_TYPE _Brush
			      , Vec2 _vPos, Vec2 _vScale, float _Duration)
{
	tDebugShapeInfo info = {};

	info.Shape = _Shape;	
	info.Pen = _Pen;
	info.BRUSH = _Brush;
	info.Position = _vPos;
	info.Scale = _vScale;	
	info.Duration = _Duration;

	CDebugMgr::Get()->AddDebugShapeInfo(info);
}

void DrawDebugLine(PEN_TYPE _Pen, Vec2 _Start, Vec2 _End, float Duration)
{
	tDebugShapeInfo info = {};

	info.Shape = DEBUG_SHAPE::LINE;
	info.Pen = _Pen;	
	info.Position = _Start;
	info.Scale = _End;
	info.Duration = Duration;

	CDebugMgr::Get()->AddDebugShapeInfo(info);
}


Vec2 RotateDir(Vec2 _vDir, float _Degree)
{
	// cos(0), sin(0)		
	// cos(0 + a), sin(0 + a) 
	// cos(0) * cos(a) - sin(0) * sin(a),  sin(0) * cos(a) + cos(0) * sin(a)

	// x, y
	// cos(0 + a), sin(0 + a) 
	// cos(a) * x  - sin(a) * y,  x * sin(a) + y * cos(a)
	float angle = (_Degree / 180.f) * PI;

	_vDir.Normalize();
	Vec2 vRotateDir = Vec2(cosf(angle) * _vDir.x - sinf(angle) * _vDir.y
		                 , sinf(angle) * _vDir.x + cosf(angle) * _vDir.y);

	return vRotateDir;
}

void SaveWString(const wstring& _Str, FILE* _File)
{
	BYTE len = (BYTE)_Str.length();
	fwrite(&len, sizeof(BYTE), 1, _File);
	fwrite(_Str.c_str(), sizeof(wchar_t), _Str.length(), _File);
}

void LoadWString(wstring& _Str, FILE* _File)
{
	BYTE len = 0;
	fread(&len, 1, 1, _File);

	wchar_t szBuff[255] = {};
	fread(szBuff, sizeof(wchar_t), len, _File);

	_Str = szBuff;
}


#include "CAssetMgr.h"
#include "CAsset.h"

void SaveAssetRef(CAsset* _Asset, FILE* _File)
{
	// Asset 이 nullptr 인지 아닌지 저장
	bool b = _Asset;
	fwrite(&b, 1, 1, _File);

	if (b)
	{
		SaveWString(_Asset->GetKey(), _File);
		SaveWString(_Asset->GetRelativePath(), _File);
	}
}

class CTexture* LoadTextureRef(FILE* _File)
{
	bool b = false;
	fread(&b, 1, 1, _File);

	if (b)
	{
		wstring strKey, strRelativePath;
		LoadWString(strKey, _File);
		LoadWString(strRelativePath, _File);

		return CAssetMgr::Get()->LoadTexture(strKey, strRelativePath);
	}
	else
	{
		return nullptr;
	}
}



#include "CCollider.h"

Vec2 GetCollDir(CObj* _Obj, CCollider* _Second)
{
	Vec2 vPos = _Obj->GetPos();
	Vec2 vDir = {};

	Vec2 vCollPos = _Second->GetFinalPos();
	Vec2 vCollScale = _Second->GetScale();

	if (vPos.x < vCollPos.x && vCollPos.y - vCollScale.y / 2.f < vPos.y)
	{
		vDir = Vec2(-1, 0);
	}
	else if (vCollPos.x < vPos.x && vCollPos.y - vCollScale.y / 2.f < vPos.y)
	{
		vDir = Vec2(1, 0);
	}
	else
	{
		vDir = Vec2(0, -1);
	}

	return vDir;
}
