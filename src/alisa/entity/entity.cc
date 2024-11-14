#include "entity.h"

namespace alisa {
namespace entity {

using namespace alisa::utils;

Entity::Entity(const std::string& name):
    name_(name), parent_(nullptr) {

    id_ = GetStringID(name);
    
    position_ = math::ZeroVector3f;
    scale_ = math::UnitVector3f;
    angle_ = math::ZeroVector3f;
    color_ = math::WhiteColor4f;

    transformation_.identity();
}

Entity::~Entity() {
    if (parent_ != nullptr) {
        parent_->removeChild(this);
    }
}

std::string Entity::getName() const {
    return name_;
}

StringID Entity::getID() const {
    return id_;
}

void Entity::setPosition(const math::Vector3f& position) {
    position_ = position;
    setChanged();
}

void Entity::setPosition(float x, float y, float z) {
    position_.x = x;
    position_.y = y;
    position_.z = z;

    setChanged();
}

void Entity::setX(float x) {
    position_.x = x;

    setChanged();
}

void Entity::setY(float y) {
    position_.y = y;

    setChanged();
}

void Entity::setZ(float z) {
    position_.z = z;

    setChanged();
}

math::Vector3f Entity::getPosition() const {
    return position_;
}

float Entity::getX() const {
    return position_.x;
}

float Entity::getY() const {
    return position_.y;
}

float Entity::getZ() const {
    return position_.z;
}

void Entity::setScale(float scale) {
    scale_.x = scale;
    scale_.y = scale;
    scale_.z = scale;
    
    setChanged();
}

void Entity::setScale(const math::Vector3f& scale) {
    scale_ = scale;
    
    setChanged();
}

void Entity::setScale(float x, float y, float z) {
    scale_.x = x;
    scale_.y = y;
    scale_.z = z;

    setChanged();
}

void Entity::setScaleX(float x) {
    scale_.x = x;

    setChanged();
}

void Entity::setScaleY(float y) {
    scale_.y = y;

    setChanged();
}

void Entity::setScaleZ(float z) {
    scale_.z = z;

    setChanged();
}

math::Vector3f Entity::getScale() const {
    return scale_;
}

float Entity::getScaleX() const {
    return scale_.x;
}

float Entity::getScaleY() const {
    return scale_.y;
}

void Entity::setAngle(const math::Vector3f& angle) {
    angle_ = angle;

    setChanged();
}

void Entity::setAngleX(float x) {
    angle_.x = x;

    setChanged();
}

void Entity::setAngleY(float y) {
    angle_.y = y;

    setChanged();
}

void Entity::setAngleZ(float z) {
    angle_.z = z;

    setChanged();
}

// FIXME: add get global info
math::Vector3f Entity::getAngle() const {
    return angle_;
}

void Entity::update(double dt) {   
    if (!is_changed_) {
        return;
    }

    math::Matrix4f transformation(1.0f);

    transformation.translate(getPosition());
    transformation.scale(getScale());
    transformation.rotate(getAngle());

    math::Matrix4f parent_transform(1.0f);

    if (parent_ != nullptr) {
        parent_->update(dt);
        parent_transform = parent_->getTransformation();
    } 
    transformation_ = parent_transform * transformation;

    setUnchanged();
}

math::Matrix4f& Entity::getTransformation() {
    return transformation_;
}

bool Entity::withTexture() const {
    return with_texture_;
}

void Entity::setColor(float r, float g, float b, float a) {
    color_ = {r, g, b, a};
}

void Entity::setColor(const math::Vector4f& color) {
    color_.r = color.x;
    color_.g = color.y;
    color_.b = color.z;
    color_.a = color.w;
}

void Entity::setColor(const math::Color4f& color) {
    color_ = color;
}

math::Color4f Entity::getColor() const {
    return color_;
}

unsigned int Entity::getVAO() const {
    return 0;
}

void Entity::setShow(bool is_show) {
    is_show_ = is_show;
}

bool Entity::isShow() const {
    return is_show_;
}

void Entity::setParent(Entity* parent) {
    parent_ = parent;
}

Entity* Entity::getParent() const {
    return parent_;
}

void Entity::addChild(Entity* child) {
    childs_[child->id_] = child;
    child->parent_ = this;
} 

void Entity::removeChild(Entity* child) {
    childs_.erase(child->id_);
}

Entity* Entity::getChild(StringID id) {
    return childs_[id];
}

Entity* Entity::getChildByName(const std::string& name) {
    return childs_[GetStringID(name)];
}

void Entity::setChanged() {
    is_changed_ = true;
    // also set changed for childs
    for (auto child : childs_) {
        child.second->setChanged();
    }
}

void Entity::setUnchanged() {
    is_changed_ = false;
}

}
}
