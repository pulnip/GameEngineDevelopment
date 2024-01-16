#pragma once

#include <memory>
#include <optional>

#include "Math.hpp"
#include "Observer.hpp"
#include "gefwd.hpp"

class IActor: public Observer<Lifetime, IComponent> {
  public:
    virtual ~IActor() = default;
    // 액터의 상태를 추적하는 데 사용
    enum class State;

    // GameEngine에서 호출하는 함수
    virtual void update(const float& deltaTime) noexcept = 0;

    virtual const State& getState() const noexcept = 0;
    virtual const std::weak_ptr<ActorManager>&
    getActorManager() const noexcept = 0;
    virtual std::optional<std::weak_ptr<IComponent>>
    queryComponent(const ComponentName name) noexcept = 0;

    // Should Move to MoveComponent
    virtual const Vector2& getPosition() const noexcept = 0;
    virtual void setPosition(const Vector2 pos) noexcept = 0;
    virtual void setBaseSize(const Vector2& bsize) noexcept = 0;
    virtual Vector2 getSize() const noexcept = 0;
    virtual const Math::Real& getScale() const noexcept = 0;
    virtual const Math::Real& getRotation() const noexcept = 0;
    virtual void setRotation(const Math::Real& rot) noexcept = 0;
};

enum class IActor::State {
    EActive,
    EPaused,
    EDead
};