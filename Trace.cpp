#include "Trace.h"
#include "SDK.h"

CBaseEntity *IHandleEntity::GetEntity(void)
{
    return gInts.EntList->GetClientEntityFromHandle(GetRefEHandle());
}

inline bool CBaseFilter::ShouldHitEntity(IHandleEntity *Handle,
                                         int contentsMask)
{
    CBaseEntity *Ent = Handle->GetEntity();

	if(Ent == nullptr)
		return false;

    // Debug traces...
    // Debug( pEnt );
    // Skip certain entity classes
    // TF2: Always filter respawn room visualizer entity! (should not interfere)
    ClientClass *EntCC = Ent->GetClientClass();
	
    if(!strcmp(EntCC->chName, "CFuncRespawnRoomVisualizer") ||
    !strcmp(EntCC->chName, "CTFMedigunShield") || !strcmp(EntCC->chName,
    "CFuncAreaPortalWindow") /*|| ( ccIgnore && pEntCC->iClassID == ccIgnore
    )*/)
    {
    	return false;
    }
    // Skip 'fake' entities used to throw us off
    // Left as an exercise to the reader ;)
	
    // Ignore specific entities
    if(Ent == Self || Ent == Ignore)
    {
        return false;
    }
    return true;
}
inline int CBaseFilter::GetTraceType() const
{
    return 0;
}

