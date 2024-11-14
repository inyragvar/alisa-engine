#ifndef ALISA_ENTITY_ENTITY_H_
#define ALISA_ENTITY_ENTITY_H_

#include <unordered_map>
#include <string>
#include <memory>

#include "alisa/utils/math/vector_2f.h"
#include "alisa/utils/math/vector_3f.h"
#include "alisa/utils/math/color_4f.h"
#include "alisa/utils/math/matrix_4f.h"
#include "alisa/resource/texture.h"

#include "alisa/utils/string_id.h"

namespace alisa {
namespace entity {

class Entity {
public:
    Entity(const std::string& name);
    virtual ~Entity();

public:
    std::string getName() const;
    utils::StringID getID() const;

public:
    void setPosition(const utils::math::Vector3f& position);
    void setPosition(float x, float y, float z);

    void setX(float x);
    void setY(float y);
    void setZ(float z);

    math::Vector3f getPosition() const;

    float getX() const;
    float getY() const;
    float getZ() const;

public:
    virtual void setScale(float scale);
    virtual void setScale(const math::Vector3f& scale);
    virtual void setScale(float x, float y, float z);
 
    virtual void setScaleX(float x);
    virtual void setScaleY(float y);
    virtual void setScaleZ(float z);

    math::Vector3f getScale() const;

    float getScaleX() const;
    float getScaleY() const;
    float getScaleZ() const;

public:
    void setAngle(const math::Vector3f& angle);

    void setAngleX(float x);
    void setAngleY(float y);
    void setAngleZ(float z);

    math::Vector3f getAngle() const;

    float getAngleX() const;
    float getAngleY() const;
    float getAngleZ() const;

public:
    math::Matrix4f& getTransformation();

public:
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;

public:
    virtual resource::Texture* getTexture() const = 0;
    bool withTexture() const;

public: 
    void setColor(float r, float g, float b, float a);
    void setColor(const math::Vector4f& color);
    void setColor(const math::Color4f& color);

    virtual math::Color4f getColor() const;

public:
    virtual unsigned int getVAO() const;

public:
    void setShow(bool is_show = true);
    bool isShow() const;

public:
    void setParent(Entity* parent);
    Entity* getParent() const;

    void addChild(Entity* child); 
    void removeChild(Entity* child);

    Entity* getChild(utils::StringID id);
    Entity* getChildByName(const std::string& name);

public:
    virtual void update(double dt);

protected:
    void setChanged();
    void setUnchanged();

protected:
    utils::StringID id_;
    std::string name_;

    math::Vector3f position_;
    math::Vector3f scale_;
    math::Vector3f angle_;
    math::Color4f color_;

    math::Matrix4f transformation_;

    bool is_changed_ = true;
    bool with_texture_ = true;
    bool is_show_ = true;

    Entity* parent_;
    std::unordered_map<utils::StringID, Entity*> childs_;
};

}
}

#endif