#include <SDL2/SDL.h>

#include "GameEngineWithSDL.hpp"

#include "SubEngine/ActorManager.hpp"
#include "SubEngine/GameLogic.hpp"
#include "GraphicsEngineWithSDL.hpp"
#include "InputSystemWithSDL.hpp"
#include "ResourceManagerWithSDL.hpp"
#include "TimerWithSDL.hpp"


GameEngineWithSDL::~GameEngineWithSDL() {
    SDL_Quit();
}

GameEngineWithSDL::GameEngineWithSDL() noexcept {
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    }
}

void GameEngineWithSDL::injectDependency() noexcept {
    auto self = weak_from_this();

    resourceManager = SubEngine::make<ResourceManagerWithSDL>(self);
    inputSystem = SubEngine::make<InputSystemWithSDL>(self);
    gameLogic = SubEngine::make<NullGameLogic>(self);
    actorManager = SubEngine::make<NullActorManager>(self);
    graphicsEngine = SubEngine::make<GraphicsEngineWithSDL>(self);
    timer = Timer::make<TimerWithSDL>();

    inputSystem->Observable<GameStatus>::subscribe(gameLogic);
}