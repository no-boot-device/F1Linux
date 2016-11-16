#include "Aimbot.h"

void Aimbot::CreateMove(CUserCmd *Command)
{
    try {
        CBaseEntity *LocalEntity = GetBaseEntity(me);
        FindTarget();
        bool hasTarget = HasTarget();
        if(hasTarget) // TODO Stop this from shooting when out of ammo.
        {
            // force attack
            Command->buttons |= IN_ATTACK;
            if(/*!gUtil.bOnShot(LocalEntity,
			 LocalEntity->GetActiveWeapon(), pUserCmd)*/ true) {
                gInts.Engine->SetViewAngles(
                    Command->viewangles); // We're going to return false from
                // CreateMove, so the game isn't going to
                // set the view angles, so we need to do
                // it manually.
                Vector angles = GetAimbotAngles();
                SilentAimFix(Command, angles);
            }
        }
    }
    catch(...) {
        Log::Error("Failed Aimbot::CreateMove");
    }
}
float Aimbot::GetDistance(Vector Origin)
{
    if(Origin.Length() == 0)
        return 0;

    CBaseEntity *LocalEntity = GetBaseEntity(me);

    if(LocalEntity == NULL)
        return 1.0f;

    Vector Distance = Origin - LocalEntity->GetAbsOrigin();

    float Length = sqrt(Distance.Length());

    if(Length < 1.0f)
        return 1.0f;

    return Length;
}
float Aimbot::GetFOV(Vector Origin)
{
    try {
        CBaseEntity *BaseEntity = GetBaseEntity(me);
        Vector AngleAim         = BaseEntity->GetAbsAngles();
        Vector Aim;
        AngleVector(AngleAim, &Aim);

        Vector EyeDiff = Origin - BaseEntity->GetEyePosition();
        EyeDiff.NormalizeInPlace();

        float DotProduct = Aim.Dot(EyeDiff);
        float FoV        = RAD2DEG(acos(DotProduct));

        if(FoV > 0) {
            return FoV;
        }
        else {
            return 0;
        }
    }
    catch(...) {
        Log::Error("Failed GetFOV");
    }
}
Aimbot::Aimbot(void) { Target = -1; }
void Aimbot::DropTarget(void) { Target = -1; }
void Aimbot::SetTarget(int iIndex) { Target = iIndex; }
bool Aimbot::HasTarget(void) { return Target != -1; }
bool Aimbot::GetTeam(int iIndex)
{
    // Add team shit here later for when they add MP.
    return true;
}
bool Aimbot::IsVisible(Vector &Enemy, CBaseEntity *BaseEntity)
{
    try {
        trace_t Trace;
        Ray_t Ray;
        CBaseFilter filter;

        Ray.Init(GetBaseEntity(me)->GetEyePosition(), Enemy);
        gInts.Trace->TraceRay(Ray, MASK_AIMBOT | CONTENTS_HITBOX, &filter,
                              &Trace);

        if(Trace.m_pEnt != NULL) {
            return (Trace.m_pEnt == BaseEntity);
        }

        return true;
    }
    catch(...) {
        Log::Error("Failed IsVisible");
    }
    return false;
}
mstudiobbox_t *GetHitbox(int Hitbox, StudioModel Header)
{
    int HitboxSetIndex = *(int *)((uintptr_t)Header + 0xB0);
    mstudiohitboxset_t *Set =
        (mstudiohitboxset_t *)(((uintptr_t)Header) + HitboxSetIndex);

    return (mstudiobbox_t *)Set->pHitbox(Hitbox);
}
void VectorTransform(Vector &in1, const matrix3x4 &in2, Vector &out)
{
    out[0] = (in1[0] * in2[0][0] + in1[1] * in2[0][1] + in1[2] * in2[0][2]) +
             in2[0][3];
    out[1] = (in1[0] * in2[1][0] + in1[1] * in2[1][1] + in1[2] * in2[1][2]) +
             in2[1][3];
    out[2] = (in1[0] * in2[2][0] + in1[1] * in2[2][1] + in1[2] * in2[2][2]) +
             in2[2][3];
}
Vector Aimbot::GetHitbox(CBaseEntity *BaseEntity, int Hitbox)
{
    Vector TargetHitbox;
    try {
        if(BaseEntity == NULL || BaseEntity->GetIndex() == 0)
            return TargetHitbox;

        StudioModel StudioHdr =
            gInts.ModelInfo->GetStudiomodel(BaseEntity->GetModel());

        if(!BaseEntity->SetupBones(BoneToWorld, 128, 0x100, 0))
            return TargetHitbox;

        box = ::GetHitbox(Hitbox, StudioHdr);

        VectorTransform(box->bbmin, BoneToWorld[box->bone], Min);
        VectorTransform(box->bbmax, BoneToWorld[box->bone], Max);

        TargetHitbox = (Min + Max) / 2;
    }
    catch(...) {
        Log::Error("Failed to get hitbox %i for %s (%s)", Hitbox,
                   BaseEntity->GetClientClass()->chName,
                   gInts.ModelInfo->GetModelName(BaseEntity->GetModel()));
    }
    return TargetHitbox;
}
bool Aimbot::IsTargetSpot(int Index, int Hitbox)
{
    try {
        CBaseEntity *BaseEntity = GetBaseEntity(Index);

        if(BaseEntity == NULL)
            return false;

        Vector TargetHitbox = GetHitbox(BaseEntity, Hitbox);

        if(IsVisible(TargetHitbox, BaseEntity)) {
            return true;
        }
    }
    catch(...) {
        Log::Error("Failed IsTargetSpot");
    }
    return false;
}
int Aimbot::IsValidEntity(int Index)
{
    try {
        if(Index == gInts.Engine->GetLocalPlayer())
            return -1;

        CBaseEntity *BaseEntity = GetBaseEntity(Index);

        if(BaseEntity == NULL)
            return -1;

        if(BaseEntity->IsAlive() == false)
            return -1;

        if(BaseEntity->GetClientClass()->iClassID == classId::CTFPlayer)
            return 0; // return target hitbox
    }
    catch(...) {
        Log::Error("Failed IsValidEntity");
    }
    return -1;
}
bool Aimbot::IsValidTarget(int Index)
{
    try {
        int Return = IsValidEntity(Index);

        if(Return == -1)
            return false;

        if(IsTargetSpot(Index, Return) == false)
            return false;
    }
    catch(...) {
        Log::Error("Failed IsValidtarget");
    }
    return true;
}
void Aimbot::FindTarget()
{
    try {
        CBaseEntity *BaseEntity = GetBaseEntity(me);

        DropTarget();

        if(BaseEntity == NULL)
            return;

        if(BaseEntity->IsDormant() == true)
            return;

        if(BaseEntity->IsAlive() == false)
            return;

        if(BaseEntity->GetActiveWeapon() == NULL)
            return;

        for(int Index = 1; Index <= gInts.EntList->GetHighestEntityIndex();
            Index++) {
            int validTarget = IsValidTarget(Index);
            if(validTarget) {
                Target = Index;
                AimAtTarget(GetBaseEntity(GetTarget()), IsValidEntity(Index));
            }
        }
    }
    catch(...) {
        Log::Error("Failed FindTarget");
    }
}
void Aimbot::AimAtTarget(CBaseEntity *BaseEntity, int Hitbox)
{
    try {
        if(GetBaseEntity(me) == NULL || BaseEntity == NULL)
            return;

        Vector AimSpot;

        AimSpot = GetHitbox(BaseEntity, Hitbox);

        Vector AimDelta = AimSpot - GetBaseEntity(me)->GetEyePosition();

        VectorAngles(AimDelta, AimAngles);

        ClampAngle(AimAngles);
    }
    catch(...) {
        Log::Error("Failed AimAttarget");
    }
}
void Aimbot::SilentAimFix(CUserCmd *Cmd, Vector &ViewAngles)
{
    Vector Silent(Cmd->forwardmove, Cmd->sidemove, Cmd->upmove);
    float Speed = sqrt(Silent.x * Silent.x + Silent.y * Silent.y);
    Vector Move;
    VectorAngles(Silent, Move);
    float Yaw        = DEG2RAD(ViewAngles.y - Cmd->viewangles.y + Move.y);
    Cmd->forwardmove = cos(Yaw) * Speed;
    Cmd->sidemove    = sin(Yaw) * Speed;
    Cmd->viewangles  = ViewAngles;
}
void Aimbot::ClampAngle(Vector &Ang)
{
    while(Ang[0] > 89)
        Ang[0] -= 180;

    while(Ang[0] < -89)
        Ang[0] += 180;

    while(Ang[1] > 180)
        Ang[1] -= 360;

    while(Ang[1] < -180)
        Ang[1] += 360;

    Ang.z = 0;
}
Vector Aimbot::GetAimbotAngles() { return AimAngles; }
