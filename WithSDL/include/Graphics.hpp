#pragma once

#include <SDL2/SDL_render.h>
#include "Blueprint.hpp"
#include "Engine/Graphics.hpp"
#include "Engine/Logger.hpp"
#include "WithSDL/Skin.hpp"

namespace WithSDL{
    class Graphics final: public Engine::Graphics{
        friend class Core;
      public:
        static void make() noexcept{
            ::Engine::Graphics::make<Graphics>();
        }

        void draw(WithSDL::Skin::Flyweight::Sprite) noexcept;
        void* context() noexcept override final;

      private:
        void initialize(const ::Blueprint::Window&) noexcept override final;
        void destroyAll() noexcept override final;

        void draw(::Skin::Flyweight::ColorRect) noexcept override final;

        void clearScreen() noexcept override final;
        void swapBuffer() noexcept override final;
    
      private:
        Logging::Bind logger={"Graphics", "SDL"};
        SDL_Window* windowHandle=nullptr;
        SDL_Renderer* renderer=nullptr;
    };
}