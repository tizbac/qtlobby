#ifndef GLWATERPLANE_H
#define GLWATERPLANE_H

#include "GLHeightMap.h"
#include "RawHeightMap.h"

class GLWaterPlane : public GLHeightMap {
public:
    GLWaterPlane();
    void build(RawHeightMap heightmap, float y, int downscale = 2);
};

#endif // GLWATERPLANE_H
