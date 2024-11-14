#ifndef ALISA_ENTITY_SPRITE_H_
#define ALISA_ENTITY_SPRITE_H_

#include <string>
#include <vector>
#include <array>

#include "entity.h"

#include "alisa/utils/math/vector_2f.h"
#include "alisa/utils/math/vector_3f.h"
#include "alisa/utils/math/color_4f.h"

#include "alisa/resource/texture.h"

#include "alisa/graphics/vertex.h"

namespace alisa {
namespace entity {

class Sprite: public Entity {
public:
    Sprite(const std::string& name);
    Sprite(const std::string& name, const std::string& path, int frames_count = 1, int frames_per_row = 1);

    virtual ~Sprite();

public:
    virtual void update(double dt);
    
public:
    void setSize(float width, float height);

    virtual int getWidth() const;
    virtual int getHeight() const;

public:
    virtual resource::Texture* getTexture() const;

public:
    virtual unsigned int getVAO() const;

public:
    virtual void setScale(float scale);
    virtual void setScale(const math::Vector3f& scale);
    virtual void setScale(float x, float y, float z);
 
    virtual void setScaleX(float x);
    virtual void setScaleY(float y);

// play frames logic
public:
    void setCurrentFrame(int current_frame);
    void setFramesPerSec(int count);
    
    void setPlayFrames(bool play);
    void setFramesLoop(bool loop);

protected:
    void playFrames(double dt);
    void updateFrame();

protected:
    void initVertexArray();
    void calculateFrameCoords();

protected:
    bool initiated_;
    resource::Texture* texture_;

    unsigned int VAO_;
    unsigned int VBO_;

    math::Vector2f size_;

    int new_current_frame_;  
    int current_frame_;  
    int frames_count_;    
    int frames_per_row_;  

    graphics::Vertex vertices_[4];
    std::vector<std::array<math::Vector2f, 4>> frame_coords_; 

    // animation 
    bool play_frames_loop_;
    bool play_frames_;
    double current_time_;
    double max_animation_time_;
};
}
}

#endif