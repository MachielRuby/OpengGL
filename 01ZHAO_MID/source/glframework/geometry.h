#pragma once

#include "core.h"

class Geometry
{
    public:
    Geometry();
    ~Geometry();

    static Geometry*createBox(float size);
    static Geometry*createSphere(float radius);
	static Geometry* createPlane(float width, float height);


    GLuint getVao() const { return mVao; }
    GLuint getIndicesCount() const { return mIndicesCount; }
    private:
    GLuint mVao{0};
    GLuint mPosVbo{0};
    GLuint mUvVbo{0};
    GLuint mNormalVbo{0};
    GLuint mEbo{0};

    uint32_t mIndicesCount{0};

};

