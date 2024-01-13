#include "Asteroid.hpp"
#include "ActorManager.hpp"
#include "AnimSpriteComponent.hpp"
#include "Component.hpp"
#include "MoveComponent.hpp"
#include "ResourceManagerWithSDL.hpp"

Asteroid::Asteroid(const std::weak_ptr<ActorManager> owner) noexcept
    : Actor(owner) {
    position = {
        static_cast<float>(Math::random(0, 1024)),
        static_cast<float>(Math::random(0, 768))};
    rotation = Math::random(0, 1024) / Math::PI;
}
void Asteroid::postConstruct() noexcept {
    auto self = weak_from_this();

    sc = Component::make<AnimSpriteComponent>(self);
    mc = Component::make<MoveComponent>(self);

    mc->velocity = Vector2::forward(Math::random(-Math::PI, Math::PI)) * Math::random(0, 300);
    mc->rotationVelocity = Math::random(-Math::PI / 2, Math::PI / 2);

    if (owner.expired()) {
        return;
    }
    auto o = owner.lock()->requestSubEngine(SubEngineName::ResourceManager);

    if (!o.has_value()) {
        return;
    }
    auto wpManager = o.value();

    if (o.value().expired()) {
        return;
    }
    auto manager = std::dynamic_pointer_cast<ResourceManagerWithSDL>(wpManager.lock());
    auto oTexture = manager->getTexture("Asteroid.png");

    if (!oTexture.has_value()) {
        return;
    }
    sc->setTexture(oTexture.value());
}