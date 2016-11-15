#pragma once

#include "SDK.h"
#include "Vector.h"

class Aimbot
{
public:
	Aimbot();

	void CreateMove(CUserCmd *Command);
	
    void DropTarget(void);
    void FindTarget(void);
    void SetTarget(int Index);
    void SilentAimFix(CUserCmd *Cmd, Vector &ViewAngles);
    void AimAtTarget(CBaseEntity *BaseEntity, int Hitbox);
    void AimAtVector(Vector Aim);
    void AimAtVectorSilent(Vector Aim, CUserCmd *Cmd);
    void ClampAngle(Vector &Ang);

    float GetFOV(Vector Origin);
    float GetDistance(Vector Origin);

    Vector vecGetHitbox(CBaseEntity *BaseEntity, int Hitbox);
    Vector GetAimbotAngles();

    bool GetTeam(int Index);
    bool IsTargetSpot(int Index, int Hitbox);
    bool HasTarget(void);
    bool IsValidTarget(int Index);
    int IsValidEntity(int Index);
    bool IsVisible(Vector &Enemy, CBaseEntity *BaseEntity);
    bool IsVisiblePlayer(Vector &Start, CBaseEntity *BaseEntity);

    int GetTarget() { return Target; }
    int GetHitbox(int WeaponID, bool OnGround);
	Vector GetHitbox(CBaseEntity *, int);

private:
    int Target;

    Vector TargetOrg;
    Vector Min, Max;
    Vector AimAngles;
    Vector CurrentView;

    // Matrix
    matrix3x4 BoneToWorld[128];

    // pointer to the hitbox
    mstudiobbox_t *box;
};
