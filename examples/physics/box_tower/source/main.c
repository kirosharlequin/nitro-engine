// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2008-2011, 2019, 2022, 2024
//
// This file is part of Nitro Engine

#include <NEMain.h>

#include "cube_bin.h"

typedef struct {
    NE_Camera *Camara;
    NE_Model *Model[6];
    NE_Physics *Physics[6];
} SceneData;

void Draw3DScene(void *arg)
{
    SceneData *Scene = arg;

    NE_CameraUse(Scene->Camara);

    NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, 0);
    for (int i = 0; i < 5; i++)
        NE_ModelDraw(Scene->Model[i]);

    NE_PolyFormat(31, 0, NE_LIGHT_1, NE_CULL_BACK, 0);
    NE_ModelDraw(Scene->Model[5]);
}

int main(int argc, char *argv[])
{
    SceneData Scene = { 0 };

    irqEnable(IRQ_HBLANK);
    irqSet(IRQ_VBLANK, NE_VBLFunc);
    irqSet(IRQ_HBLANK, NE_HBLFunc);

    NE_Init3D();

    Scene.Camara = NE_CameraCreate();
    NE_CameraSet(Scene.Camara,
                 -9, 7, 5,
                  0, 6, 0,
                  0, 1, 0);

    // Create objects
    for (int i = 0; i < 6; i++)
    {
        Scene.Model[i] = NE_ModelCreate(NE_Static);
        Scene.Physics[i] = NE_PhysicsCreate(NE_BoundingBox);

        NE_ModelLoadStaticMesh(Scene.Model[i], cube_bin);

        NE_PhysicsSetModel(Scene.Physics[i], Scene.Model[i]);

        NE_PhysicsSetSize(Scene.Physics[i], 1, 1, 1);
    }

    NE_PhysicsEnable(Scene.Physics[5], false);

    // Object coordinates
    NE_ModelSetCoord(Scene.Model[0], 0, 2, 0);
    NE_ModelSetCoord(Scene.Model[1], 0, 4, 0);
    NE_ModelSetCoord(Scene.Model[2], 0, 6, 0);
    NE_ModelSetCoord(Scene.Model[3], 0, 8, 0);
    NE_ModelSetCoord(Scene.Model[4], 0, 10, 0);
    NE_ModelSetCoord(Scene.Model[5], 0, 0, 0);

    // Set gravity
    NE_PhysicsSetGravity(Scene.Physics[0], 0.001);
    NE_PhysicsSetGravity(Scene.Physics[1], 0.001);
    NE_PhysicsSetGravity(Scene.Physics[2], 0.001);
    NE_PhysicsSetGravity(Scene.Physics[3], 0.001);
    NE_PhysicsSetGravity(Scene.Physics[4], 0.001);

    // Tell the engine what to do if there is a collision
    NE_PhysicsOnCollision(Scene.Physics[0], NE_ColBounce);
    NE_PhysicsOnCollision(Scene.Physics[1], NE_ColBounce);
    NE_PhysicsOnCollision(Scene.Physics[2], NE_ColBounce);
    NE_PhysicsOnCollision(Scene.Physics[3], NE_ColBounce);
    NE_PhysicsOnCollision(Scene.Physics[4], NE_ColBounce);

    // Set percent of energy kept after a bounce
    // Default is 50, 100 = no energy lost
    NE_PhysicsSetBounceEnergy(Scene.Physics[0], 100);
    NE_PhysicsSetBounceEnergy(Scene.Physics[1], 100);
    NE_PhysicsSetBounceEnergy(Scene.Physics[2], 100);
    NE_PhysicsSetBounceEnergy(Scene.Physics[3], 100);
    NE_PhysicsSetBounceEnergy(Scene.Physics[4], 100);

    // Lights
    NE_LightSet(0, NE_Green, -1, -1, 0);
    NE_LightSet(1, NE_Blue, -1, -1, 0);

    // Background
    NE_ClearColorSet(NE_Red, 31, 63);

    while (1)
    {
        NE_WaitForVBL(NE_UPDATE_PHYSICS);

        NE_ProcessArg(Draw3DScene, &Scene);
    }

    return 0;
}
