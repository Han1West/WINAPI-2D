#pragma once

#define SINGLE(Type)	public:\
							static Type* Get()\
							{\
								static Type mgr;\
								return &mgr;\
							}\
						public:\
							void operator = (const Type& _Other) = delete;\
						private:\
							Type();\
							Type(const Type& _Other) = delete;\
							~Type();

#define DT CTimeMgr::Get()->GetDeltaTime()

#define KEY_CHECK(key, State)  (CKeyMgr::Get()->GetKeyState(key) == State)

#define KEY_TAP(key)		KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_PRESSED(key)	KEY_CHECK(key, KEY_STATE::PRESSED)
#define KEY_RELEASED(key)	KEY_CHECK(key, KEY_STATE::RELEASED)
#define KEY_NONE(key)		KEY_CHECK(key, KEY_STATE::NONE)

#define PI 3.1415926535f

#define SELECT_PEN(DC, PEN)		CSelectObject SelectPen(DC, PEN);
#define SELECT_BRUSH(DC, BRUSH) CSelectObject SelectBrush(DC, BRUSH);

#define BackDC CEngine::Get()->GetBackDC()


#define CLONE(Type)	 Type* Clone() { return new Type(*this); }

#define CLONE_DISABLE(Type)     Type* Clone() { return nullptr; } \
								Type(const Type& _Other) = delete;

					
						