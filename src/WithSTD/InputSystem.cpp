#include <iostream>
#include <set>
#include <string>
#include "Engine/InputSystem.hpp"
#include "Engine/Logger.hpp"

using namespace WithSTD;

InputSystem::InputSystem() noexcept:
    logger(std::make_unique<Engine::BindedLogger>("InputSystem", "STD"))
{
    logger->debug("constructed");
}

InputSystem::~InputSystem(){
    logger->debug("destructed");
}

void InputSystem::update(const Game::Time& deltaTime) noexcept{
    std::set<char> log;
    std::string str;

    std::cin >> str;
    for(char c: str){
        log.emplace(c);
    }

    auto it=log.begin();
    for(auto& [key, publisher]: keyMap){
        while(*it < key) ++it;
        if(it == log.end()) break;

        if(*it == key) publisher.notify({Skin::Key::Status::PRESSED, key});
    }
}