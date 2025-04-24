#pragma once

#include "CComponent.h"

enum class RIGIDBODY_MODE
{
    TOPVIEW,
    PLATFOMER,
};

class CRigidBody :
    public CComponent
{
private:
    RIGIDBODY_MODE      m_Mode;
    Vec2                m_Force;            // 누적 힘 (크기 + 방향)
    Vec2                m_Velocity;         // 속도    (크기 + 방향)
    Vec2                m_InitVelo;         
    Vec2                m_GravidyVelocity;
    float               m_Mass;             // 질량
    float               m_Friction;         // 마찰계수
    float               m_MaxSpeed;         // 최대 속력 제한
    float               m_GravityAccel;     // 중력가속도 크기
    float               m_GravityMaxSpeed;  // 중력으로 인해서 발생하는 속도의 최대 제한치
    float               m_JumpSpeed;
    bool                m_IsMove;           // 현재 오브젝트가 움직이는 중인지 아닌지 체크
    bool                m_IsGound;          // 지상위에 서있는지 판정
    bool                m_UseGravity;

public:
    void SetForce(Vec2 _Force) { m_Force = _Force; }
    void AddForce(Vec2 _Force) { m_Force += _Force; }
    void SetInitVelo(Vec2 _Velocity) { m_InitVelo = _Velocity; }
    void SetMass(float _Mass) { m_Mass = _Mass; }
    void SetMaxSpeed(float _Max) { m_MaxSpeed = _Max; }
    void SetVelocity(Vec2 _Velocity) { m_Velocity = _Velocity; }
    void AddVelociy(Vec2 _Velocity) { m_Velocity += _Velocity; }
    void SetFriction(float _Friction) { m_Friction = _Friction; }
    void SetMode(RIGIDBODY_MODE _Mode) { m_Mode = _Mode; }
    void SetGravityAccelScale(float _Accel) { m_GravityAccel = _Accel; }
    void SetMaxGravitySpeed(float _Max) { m_GravityMaxSpeed = _Max; }
    void UseGravity(bool _Use) { m_UseGravity = _Use; }
    void SetJumpSpeed(float _Speed) { m_JumpSpeed = _Speed; }

    void SetGround(bool _Ground);

    void Jump();
    void Squash();

    Vec2 GetForce() { return m_Force; }
    float GetMass() { return m_Mass; }
    float GetMaxSpeed() { return m_MaxSpeed; }
    Vec2 GetVelocity() { return m_Velocity; }
    Vec2 GetGravityVeloicity() { return m_GravidyVelocity; }
    RIGIDBODY_MODE GetMode() { return m_Mode; } 
    bool IsMove() { return m_IsMove; }
    bool IsGround() { return m_IsGound; }

public:
    virtual void FinalTick() override;

public:
    CLONE(CRigidBody);
    CRigidBody();
    CRigidBody(const CRigidBody& _Other);
    ~CRigidBody();
};

 