#include "MWDModel.h"
#include "MWDRenderer.h"
void MWDModel::Draw()
{
    for (int i = 0; i < meshes.size(); ++i) {
        meshes[i].DrawMesh(MWDRenderer::GetMainRenderer());
    }
}
