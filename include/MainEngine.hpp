#pragma once

#include <memory>

#include "myfwd.hpp"

class MainEngine{
  public:
    MainEngine() noexcept;
    ~MainEngine();

    void start() noexcept;
};