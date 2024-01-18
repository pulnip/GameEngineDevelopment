#include <SDL2/SDL_keyboard.h>

#include "Component/Component.hpp"
#include "Component/InputComponent.hpp"
#include "Component/MoveComponent.hpp"
#include "SubEngine/ActorManager.hpp"

#include "AnimSpriteComponent.hpp"
#include "ResourceManagerWithSDL.hpp"

#include "Ship.hpp"

void Ship::updateActor(const float &deltaTime) noexcept {
    mc->velocity = {0.0f, 0.0f};
    mc->rotationVelocity = 0.0;
}
Ship::Ship() noexcept{
}
void Ship::injectDependency() noexcept {
    auto self = weak_from_this();

    sc = Component::make<AnimSpriteComponent>(self);
    ic = Component::make<InputComponent>(self);
    mc = Component::make<MoveComponent>(self);

    mc->position = {500.0f, 500.0f};

    ic->setKey(SDL_SCANCODE_Q, [&rv = mc->rotationVelocity()]() {
        rv += -Math::PI;
    });
    ic->setKey(SDL_SCANCODE_E, [&rv = mc->rotationVelocity()]() {
        rv += Math::PI;
    });
    ic->setKey(SDL_SCANCODE_D, [&v = mc->velocity(), &r = mc->rotation()]() {
        v += Vector2::forward(r) * 300.0f;
    });
    ic->setKey(SDL_SCANCODE_A, [&v = mc->velocity(), &r = mc->rotation()]() {
        v += Vector2::forward(r) * -300.0f;
    });

    if (owner.expired()) {
        return;
    }
    auto o = owner.lock()->query(SubEngineName::ResourceManager);

    if (!o.has_value()) return;
    if (o.value() == nullptr) return;
    auto manager = std::dynamic_pointer_cast<ResourceManagerWithSDL>(o.value());

    std::vector<std::optional<SDL_Texture *>> opAnims = {
        manager->getTexture("Ship01.png"),
        manager->getTexture("Ship02.png"),
        manager->getTexture("Ship03.png"),
        manager->getTexture("Ship04.png")};

    std::vector<SDL_Texture *> anims;
    for (auto &o : opAnims) {
        if (o.has_value()) {
            anims.emplace_back(o.value());
        }
    }

    sc->setAnimTextures(anims);
}
