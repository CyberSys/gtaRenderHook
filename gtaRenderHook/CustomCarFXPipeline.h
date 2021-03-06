#pragma once
#include "DeferredPipeline.h"

class CCustomCarFXPipeline :public CDeferredPipeline
{
public:
    CCustomCarFXPipeline();
    ~CCustomCarFXPipeline();
    static void Patch();
    void ResetAlphaList();
    void RenderAlphaList();
    void Render( RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags );
private:
    static std::list<AlphaMesh*> m_aAlphaMeshList;
};


extern CCustomCarFXPipeline* g_pCustomCarFXPipe;