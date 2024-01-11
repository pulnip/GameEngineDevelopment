#include <ranges>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "SubEngine.hpp"

SubEngine::SubEngine(const std::weak_ptr<GameEngine> owner) noexcept
    : owner(owner) {}

void SubEngine::postConstruct() noexcept {
    notify(PSMSG::Lifetime::CONSTRUCTED, shared_from_this());
}

// Resource Manager

SDL_ResourceManager::~SDL_ResourceManager() {
    for (auto &[file, skin] : skins) {
        SDL_DestroyTexture(skin);

        SDL_Log("Texture Unloaded: %s", file.c_str());
    }
}

void SDL_GraphicsEngine::initBackground() noexcept {
    // 후면 버퍼를 단색으로 클리어
    SDL_SetRenderDrawColor(*renderer,
                           0, 0, 255, // 파란 배경
                           255);

    SDL_RenderClear(*renderer);
}

void SDL_GraphicsEngine::changeColorBuffer() noexcept {
    SDL_RenderPresent(*renderer);
}

std::optional<SDL_Texture *>
SDL_ResourceManager::getSkin(const std::string &fileName) noexcept {
    auto [it, skinNotLoaded] = skins.try_emplace(fileName, nullptr);
    auto &texture = it->second;

    if (skinNotLoaded) {
        texture = loadTexture(fileName);
    }
    // skin file itself not exist.
    if (texture == nullptr) {
        return std::nullopt;
    }
    return texture;
}

constexpr bool HW_RENDERING = false;

SDL_Texture *
SDL_ResourceManager::loadTexture(const std::string &fileName) noexcept {
    SDL_Texture *texture;

    if (context.expired()) {
        return nullptr;
    }
    auto renderer = context.lock();

    // 파일로부터 로딩
    if (HW_RENDERING) {
        texture = IMG_LoadTexture(*renderer, fileName.c_str());
        if (texture == nullptr) {
            SDL_Log("Failed to Load Texture from File: %s", fileName.c_str());
        }
    } else {
        SDL_Surface *surface = IMG_Load(fileName.c_str());
        if (surface == nullptr) {
            SDL_Log("Failed to Load Surface from File: %s", fileName.c_str());
            return nullptr;
        }

        texture = SDL_CreateTextureFromSurface(*renderer, surface);
        if (!texture) {
            SDL_Log("Failed to Create Texture from Surface: %s", SDL_GetError());
        }
        SDL_FreeSurface(surface);
    }

    return texture;
}

// Input System
InputSystem::InputSystem(const std::weak_ptr<GameEngine> owner) noexcept : SubEngine(owner) {}

void SDL_InputSystem::update(const float &deltaTime) noexcept {
    SDL_Event event;
    // 큐에 여전히 이벤트가 남아있는 동안
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            Observable<GameStatus>::notify(GameStatus::FORCE_QUIT);
            break;
        }
    }

    const uint8_t *keyState = SDL_GetKeyboardState(nullptr);
    // ESC로 게임 종료
    if (keyState[SDL_SCANCODE_ESCAPE]) {
        Observable<GameStatus>::notify(GameStatus::FORCE_QUIT);
    }
}

SDL_InputSystem::SDL_InputSystem(const std::weak_ptr<GameEngine> owner) noexcept : InputSystem(owner) {}

// Graphics Engine

SDL_GraphicsEngine::~SDL_GraphicsEngine() {
    SDL_DestroyRenderer(*renderer);
    SDL_DestroyWindow(*window);
}

void SDL_GraphicsEngine::postConstruct() noexcept {
    SubEngine::postConstruct();

    window = std::make_shared<SDL_Window *>(SDL_CreateWindow(
        "GameEngine Programming in C++",
        100,
        100,
        1024,
        768,
        0));
    if (*window == nullptr) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return;
    }

    renderer = std::make_shared<SDL_Renderer *>(SDL_CreateRenderer(
        *window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    if (*renderer == nullptr) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return;
    }

    // Init SDL Image Library
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
        return;
    }
}

void SDL_GraphicsEngine::onNotify(ColorRect rect) noexcept {
    // transform position: center to top-left
    rect.position = rect.position - rect.size / 2;

    auto _rect = SDL_Rect{
        static_cast<int>(rect.position.x),
        static_cast<int>(rect.position.y),
        static_cast<int>(rect.size.x),
        static_cast<int>(rect.size.y),
    };

    SDL_SetRenderDrawColor(*renderer,
                           rect.color.red,
                           rect.color.green,
                           rect.color.blue,
                           rect.color.alpha);
    SDL_RenderFillRect(*renderer, &_rect);
}

void SDL_GraphicsEngine::onNotify(SDL_Sprite sprite) noexcept {
    // transform position: center to top-left
    sprite.position = sprite.position - sprite.size / 2;

    auto _rect = SDL_Rect{
        static_cast<int>(sprite.position.x),
        static_cast<int>(sprite.position.y),
        static_cast<int>(sprite.size.x),
        static_cast<int>(sprite.size.y),
    };

    SDL_RenderCopyEx(*renderer,
                     sprite.texture,
                     // 텍스처의 특정 영역 (nullptr은 전체영역)
                     nullptr,
                     // 어느 위치에, 어느 크기로 렌더링할 지
                     &_rect,
                     // 라디안을 각도로 변환
                     Math::toDegree(sprite.rotation),
                     // 회전 중심점
                     nullptr,
                     SDL_FLIP_NONE);
}
