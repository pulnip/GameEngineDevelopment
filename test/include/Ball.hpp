#pragma once

#include "myfwd.hpp"
#include "Engine/Logger.hpp"
#include "Actor/Vanilla.hpp"

namespace Test{
    class Ball final: public Actor::Vanilla{
      public:
        void initAbility() noexcept override final;

      private:
        void updateActor(const Game::Time&) noexcept override final;

      private:
        Logging::Bind logger={"Ball", id};
    };
}
