/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#include <vector>

#include "MapTree.h"
#include "VMapManager2.h"
#include "WorldModel.h"
#include "ModelInstance.h"

using namespace std;

namespace VMAP
{
    // Need direct access to encapsulated VMAP data, so we add functions for MMAP generator
    // maybe add MapBuilder as friend to all of the below classes would be better?

    // declared in src/shared/vmap/MapTree.h
    void StaticMapTree::getModelInstances(ModelInstance* &models, uint32 &count)
    {
        models = iTreeValues;
        count = iNTreeValues;
    }

    // declared in src/shared/vmap/VMapManager2.h
    void VMapManager2::getInstanceMapTree(InstanceTreeMap &instanceMapTree)
    {
        instanceMapTree = iInstanceMapTrees;
    }

    // declared in src/shared/vmap/WorldModel.h
    void WorldModel::getGroupModels(vector<GroupModel> &groupModels)
    {
        groupModels = this->groupModels;
    }

    // declared in src/shared/vmap/WorldModel.h
    void GroupModel::getMeshData(vector<Vector3> &vertices, vector<MeshTriangle> &triangles, WmoLiquid* &liquid)
    {
        vertices = this->vertices;
        triangles = this->triangles;
        liquid = iLiquid;
    }

    // declared in src/shared/vmap/ModelInstance.h
    WorldModel* const ModelInstance::getWorldModel()
    {
        return iModel;
    }

    // declared in src/shared/vmap/WorldModel.h
    void WmoLiquid::getPosInfo(uint32 &tilesX, uint32 &tilesY, Vector3 &corner) const
    {
        tilesX = iTilesX;
        tilesY = iTilesY;
        corner = iCorner;
    }
}