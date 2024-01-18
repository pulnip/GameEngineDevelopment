#pragma once

#include <cstdint>
#include <map>

#include "Observable.hpp"
#include "SubEngine.hpp"

struct Key {
    enum class Status {
        PRESSED,
        RELEASED
    } status;
    uint8_t key;
};

class InputSystem: public SubEngine,
    public Observable<GameStatus>
{
  public:
    virtual ~InputSystem()=default;
    virtual void update(const float& deltaTime) noexcept override=0;

    void registerKey(
        const uint8_t key,
        const std::weak_ptr<Observer<Key>> subscriber
    ) noexcept;

  protected:
    InputSystem() noexcept=default;

  private:
    SubEngineName getName() const noexcept override final{
        return SubEngineName::InputSystem;
    }

  private:
    virtual void injectDependency() noexcept override=0;

  protected:
    std::map<uint8_t, Observable<Key>> keyMap;
};

class NullInputSystem: public InputSystem{
  private:
    void update(const float&) noexcept override final{}
    void injectDependency() noexcept override final{}
};

// for std input
class InputSystem_default: public InputSystem{
  private:
    void update(const float& deltaTime) noexcept override final;
    void injectDependency() noexcept override final{}
};
