#include <algorithm>
#include "Engine/Logger.hpp"
#include "Actor/Vanilla.hpp"
#include "Component/Ability.hpp"

using namespace Actor;

int Vanilla::seed=0;

Vanilla::Vanilla() noexcept: id(++seed),
    logger(std::make_unique<Engine::BindedLogger>("Actor", id))
{
    logger->debug("constructed");
}

Vanilla::~Vanilla(){
    logger->debug("destructed");
}

void Vanilla::update(const Game::Time& deltaTime) noexcept{
    if(state != State::Active) return;

    for(auto& c: ordered){
        c->update(deltaTime);
    }
    updateActor(deltaTime);
}

void Vanilla::add(pComponent ptr) noexcept{
    components.emplace(ptr->getType(), ptr);
    ordered.emplace(ptr);
}

using Type=Component::Type;

std::optional<pComponent> Vanilla::get(Type type) noexcept{
    auto it=components.find(type);
    if(it==components.end()){
        return std::nullopt;
    }
    return it->second;
}

void Vanilla::remove(Type type) noexcept{
    components.erase(type);
    std::erase_if(ordered, [type](const pComponent& ptr){
        return ptr->getType()==type;
    });
}

bool Vanilla::UpdateOrder::operator()(
    const pComponent& lhs, const pComponent& rhs
) const noexcept{
    return lhs->getUpdateOrder() < rhs->getUpdateOrder();
}