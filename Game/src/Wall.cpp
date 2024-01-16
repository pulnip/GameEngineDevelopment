#include "Component/BoxComponent.hpp"
#include "Component/Component.hpp"
#include "Component/MoveComponent.hpp"
#include "SubEngine/ActorManager.hpp"
#include "SubEngine/GraphicsEngine.hpp"

#include "Wall.hpp"

void Wall::injectDependency() noexcept {
    auto self = weak_from_this();

    bc = Component::make<BoxComponent>(self);
    mc = Component::make<MoveComponent>(self);

    bc->Observable<ColorRect>::subscribe(std::dynamic_pointer_cast<GraphicsEngine>(
        owner.lock()->requestSubEngine(SubEngineName::GraphicsEngine).value().lock()));
}