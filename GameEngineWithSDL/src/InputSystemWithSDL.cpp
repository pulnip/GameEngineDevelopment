#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>

#include "GameEngine/GameEngine.hpp"
#include "InputSystemWithSDL.hpp"
#include "SubEngine/GameLogic.hpp"

void InputSystemWithSDL::update(const float &deltaTime) noexcept {
    SDL_Event event;
    // 큐에 여전히 이벤트가 남아있는 동안
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            Observable<GameStatus>::notify(GameStatus::FORCE_QUIT);
        }
    }
    
    const uint8_t *keyState = SDL_GetKeyboardState(nullptr);

    if(keyState[escapeKeycode()]){
        Observable<GameStatus>::notify(GameStatus::GAME_OVER);
    }

    for(auto& [key, target]: keyMap){
        if(keyState[key]){
            target.notify( {Key::Status::PRESSED, key} );
        }
    }
}

uint8_t InputSystemWithSDL::escapeKeycode() const noexcept{
    return SDL_SCANCODE_ESCAPE;
}
