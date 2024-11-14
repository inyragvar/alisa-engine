#include "vertex.h"

namespace alisa {
namespace graphics {

using namespace alisa::utils;

Vertex::Vertex(const math::Vector3f& position, const math::Vector2f& texture) {
    this->position = position;
    this->texture = texture;
}


Vertex CreateMidpoint(const Vertex& v1, const Vertex& v2) {
    Vertex midpoint;
    
    midpoint.position = (v1.position + v2.position) * 0.5f;
    midpoint.texture = (v1.texture + v2.texture) * 0.5f;
    midpoint.normal = (v1.normal + v2.normal) * 0.5f; // Normalize the averaged normal
    midpoint.tangent = (v1.tangent + v2.tangent) * 0.5f; // Normalize the averaged tangent
    midpoint.bitangent = (v1.bitangent + v2.bitangent) * 0.5f; // Normalize the averaged tangent

    midpoint.normal.normalize();
    midpoint.tangent.normalize();
    midpoint.bitangent.normalize();
    
    return midpoint;
}

}
}