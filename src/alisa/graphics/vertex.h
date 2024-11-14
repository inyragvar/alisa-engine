#ifndef ALISA_GRAPHICS_VERTEX_H_
#define ALISA_GRAPHICS_VERTEX_H_

#include "alisa/utils/math/vector_2f.h"
#include "alisa/utils/math/vector_3f.h"

namespace alisa {
namespace graphics {

const unsigned int MAX_BONE_INFLUENCE = 4; 

struct Vertex {
    utils::math::Vector3f position;
    utils::math::Vector2f texture;
    utils::math::Vector3f normal;
    utils::math::Vector3f tangent;
    utils::math::Vector3f bitangent;

    int bone_ids[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];    

    Vertex() {}

    Vertex(const utils::math::Vector3f& position, const utils::math::Vector2f& texture);
};

Vertex CreateMidpoint(const Vertex& v1, const Vertex& v2);

}
}

#endif