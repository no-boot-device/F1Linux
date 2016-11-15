#pragma once

#include <cstring>
#include "Vector.h"
#include <stdint.h>

class CBaseEntity;

class IHandleEntity
{
public:
    virtual ~IHandleEntity() {}
    virtual void SetRefEHandle(int RefEHandle) = 0;
    virtual int GetRefEHandle(void)               = 0;
    CBaseEntity *GetEntity(void);
};
class ITraceFilter
{
public:
    virtual bool ShouldHitEntity(IHandleEntity *BaseEntity, int mask) = 0;
    virtual int GetTraceType() const = 0;
};
class CBaseFilter : public ITraceFilter
{
public:
    CBaseFilter() : Self(nullptr), Ignore(nullptr) {}
    CBaseFilter(CBaseEntity *Ignore) : Ignore(Ignore) {}

    virtual bool ShouldHitEntity(IHandleEntity *Entity, int contentsMask);
    virtual int GetTraceType() const;
    // Setup the filter
    inline void SetIgnoreSelf(CBaseEntity *Self) { this->Self = Self; }
    inline void SetIgnoreEntity(CBaseEntity *Ignore) { this->Ignore = Ignore; }

protected:
    // void Debug( CBaseEntity* pEnt );
protected:
    CBaseEntity *Self;
    CBaseEntity *Ignore;
};

struct Ray_t
{
    Vector m_Start;
    float w1;
    Vector m_Delta;
    float w2;
    Vector m_StartOffset;
    float w3;
    Vector m_Extents;
    float w4;
    bool m_IsRay;
    bool m_IsSwept;
    void Init(Vector const &start, Vector const &end)
    {
        // VectorSubtract( end, start, m_Delta );
        m_Delta   = end - start;
        m_IsSwept = (m_Delta.LengthSqr() != 0);
        VectorClear(m_Extents);
        m_IsRay = true;
        VectorClear(m_StartOffset);
        VectorCopy(start, m_Start);
    }
    void Init(Vector const &start, Vector const &end, Vector const &mins,
              Vector const &maxs)
    {
        // VectorSubtract( end, start, m_Delta );
        m_Delta   = end - start;
        m_IsSwept = (m_Delta.LengthSqr() != 0);
        // VectorSubtract( maxs, mins, m_Extents );
        m_Extents = maxs - mins;
        m_Extents *= 0.5f;
        m_IsRay = (m_Extents.LengthSqr() < 1e-6);
        // VectorAdd( mins, maxs, m_StartOffset );
        m_StartOffset = mins + maxs;
        m_StartOffset *= 0.5f;
        // VectorAdd( start, m_StartOffset, m_Start );
        m_Start = start + m_StartOffset;
        m_StartOffset *= -1.f;
    }
};
struct csurface_t
{
    const char *name;
    short surfaceProps;
    unsigned short flags;
};
struct cplane_t
{
    Vector normal;
    float dist;
    uint8_t type;
    uint8_t signbits;
    uint8_t pad[2];
};
class CBaseTrace
{
public:
    Vector start;
    Vector end;
    cplane_t plane;
    float fraction;
    int contents;
    unsigned short dispFlags;
    bool allsolid;
    bool startsolid;
};
class trace_t : public CBaseTrace
{
public:
    float fractionleftsolid;
    csurface_t surface;
    int hitGroup;
    short physicsBone;
    CBaseEntity *m_pEnt;
    int hitbox;
};
