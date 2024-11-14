#include "sprite.h"

#include "alisa/graphics/gl/function.h"
#include "alisa/graphics/graphics.h"


#include "alisa/logger/logger.h"

#include "alisa/resource/resource_manager.h"

namespace alisa {
namespace entity {

using namespace alisa::utils;

Sprite::Sprite(const std::string& name):
    Entity(name), texture_(nullptr), current_frame_(0), new_current_frame_(0), frames_count_(0), frames_per_row_(0), initiated_(false),
    play_frames_loop_(false), play_frames_(false), current_time_(0)  {

    size_ = math::Vector2f(1.0f, 1.0f);
}

Sprite::Sprite(const std::string& name, const std::string& path, int frames_count, int frames_per_row):
    Entity(name), current_frame_(0), new_current_frame_(0), frames_count_(frames_count), frames_per_row_(frames_per_row), initiated_(false),  
    play_frames_loop_(false), play_frames_(false), current_time_(0) {
    
    size_ = math::Vector2f(1.0f, 1.0f);
    texture_ = resource::ResourceManager::get().getTexture(path);

    calculateFrameCoords();
}

Sprite::~Sprite() {
    if (with_texture_) {
        std::string image_path = texture_->getPath();
        resource::ResourceManager::get().releaseTexture(image_path);
    }
    
    graphics::gl::DeleteBuffers(1, &VAO_);
    logger::Logger::info("releasing VAO: %d;", VAO_);
}

void Sprite::update(double dt) {
    if (!initiated_) {
        initVertexArray();
        initiated_ = true;
    }

    Entity::update(dt);
    
    if (play_frames_) {
        playFrames(dt);
    }

    if (new_current_frame_ != current_frame_) {
        current_frame_ = new_current_frame_;
        updateFrame();
    }
}

void Sprite::setSize(float width, float height) {
    size_.x = width;
    size_.y = height;
}

int Sprite::getWidth() const {
    return size_.x;
}

int Sprite::getHeight() const {
    return size_.y;
}

resource::Texture* Sprite::getTexture() const {
    return texture_;
}

unsigned int Sprite::getVAO() const {
    return VAO_;
}

void Sprite::setScale(float scale) {
    Entity::setScale(math::Vector3f(scale * size_.x, scale * size_.y, 1.0f));
}

void Sprite::setScale(const math::Vector3f& scale) {
    Entity::setScale(math::Vector3f(scale.x * size_.x, scale.y * size_.y, 1.0f));
}

void Sprite::setScale(float x, float y, float z) {
    Entity::setScale(x * size_.x, y * size_.y, 1.0f);
}

void Sprite::setScaleX(float x) {
    Entity::setScaleX(x * size_.x);
}

void Sprite::setScaleY(float y) {
    Entity::setScaleY(y * size_.y);
}


void Sprite::initVertexArray() {
    // Generate and bind a Vertex Array Object (VAO)
    graphics::gl::GenVertexArrays(1, &VAO_);
    // Generate and bind a Vertex Buffer Object (VBO)
    glGenBuffers(1, &VBO_);

    graphics::gl::BindVertexArray(VAO_);
    // Define 4 vertices for the rectangle (two triangles forming a quad)

    const auto& coords = frame_coords_[current_frame_];

    // Specify the vertices in the correct order for GL_TRIANGLE_STRIP
    vertices_[0] = graphics::Vertex(math::Vector3f(-1.0, -1.0, 0.0f), coords[0]);  // Bottom left
    vertices_[1] = graphics::Vertex(math::Vector3f(-1.0,  1.0, 0.0f), coords[1]);  // Top left
    vertices_[2] = graphics::Vertex(math::Vector3f( 1.0, -1.0, 0.0f), coords[2]);  // Bottom right
    vertices_[3] = graphics::Vertex(math::Vector3f( 1.0,  1.0, 0.0f), coords[3]);  // Top rights
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);

    // Upload vertex data to the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);

    // Enable vertex attributes and set up the pointers for position, color, and texture coordinates
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(graphics::Vertex), (const GLvoid*)offsetof(graphics::Vertex, position));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(graphics::Vertex), (const GLvoid*)offsetof(graphics::Vertex, texture));
   
    // Unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);

      // Unbind the VAO
    graphics::gl::BindVertexArray(0);

    // Optionally disable the vertex attributes (not strictly necessary, but good for cleanup) - must be after VAO unbind!!!!!
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(2);
}

void Sprite::calculateFrameCoords() {
    float frame_width = 1.0f / frames_per_row_;
    float frame_height = 1.0f / (frames_count_ / frames_per_row_);

    for (int i = 0; i < frames_count_; ++i) {
        int row = i / frames_per_row_;
        int col = i % frames_per_row_;

        math::Vector2f t00 = math::Vector2f(col * frame_width, row * frame_height);            // Bottom left
        math::Vector2f t01 = math::Vector2f(col * frame_width, (row + 1) * frame_height);      // Top left
        math::Vector2f t10 = math::Vector2f((col + 1) * frame_width, row * frame_height);      // Bottom right
        math::Vector2f t11 = math::Vector2f((col + 1) * frame_width, (row + 1) * frame_height);// Top right

        frame_coords_.push_back({t00, t01, t10, t11});
    }
}

void Sprite::setCurrentFrame(int current_frame) {
    new_current_frame_ = current_frame % frames_count_;
}

void Sprite::setFramesPerSec(int count) {
    max_animation_time_ = frames_count_ / static_cast<float>(count);
}

void Sprite::setPlayFrames(bool play) {
    play_frames_ = play;
}

void Sprite::setFramesLoop(bool loop) {
    play_frames_loop_ = loop;
}

void Sprite::playFrames(double dt) {
    current_time_ += dt;

    if (current_time_ > max_animation_time_) {
        if (!play_frames_loop_) {
            current_time_ = 0;
            play_frames_ = false;
            return;
        }
        current_time_ = current_time_ - max_animation_time_;
    }

    int current_frame = frames_count_ * (current_time_ / max_animation_time_);
    int new_frame = current_frame_;
    if (current_frame > frames_count_) {
        new_frame = current_frame % frames_count_;
    } else {
        new_frame = current_frame;
    }

    new_current_frame_ = new_frame;
}

void Sprite::updateFrame() {
    // FIXME: need check why current_frame has <0 value or reload
    if (current_frame_ < 0 || current_frame_ > frames_count_) {
        return;
    }
    // Update the texture coordinates in the vertex array
    const auto& coords = frame_coords_[current_frame_];

    vertices_[0].texture = coords[0];  // Bottom left
    vertices_[1].texture = coords[1];  // Top left
    vertices_[2].texture = coords[2];  // Bottom right
    vertices_[3].texture = coords[3];  // Top right

    // Upload the updated vertex data to the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_), vertices_);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}
}
