#pragma once

#include <set>

#include "SubEngine.hpp"

class GraphicsEngine: public SubEngine,
    public Observer<ColorRect>
{
  private:
    using Drawable = std::shared_ptr<DrawComponent>;
    struct DrawOrder {
        bool operator()(const Drawable &lhs, const Drawable &rhs) const;
    };

  public:
    virtual ~GraphicsEngine() = default;

    void append(Drawable d) noexcept;

  protected:
    GraphicsEngine() noexcept=default;

  private:
    SubEngineName getName() const noexcept override final{
        return SubEngineName::GraphicsEngine;
    }
    void update(const float &deltaTime) noexcept override final;
    virtual void injectDependency() noexcept override final{ setAttribute(); }

  private:
    virtual void onNotify(ColorRect rect) noexcept override=0;

    virtual void setAttribute() noexcept=0;
    virtual void prepareRendering() noexcept=0;
    virtual void finalizeRendering() noexcept=0;

  private:
    // ordered by Draw Order(Draw Component's);
    std::multiset<Drawable, DrawOrder> drawables;
};

class NullGraphicsEngine: public GraphicsEngine{
  private:
    void onNotify(ColorRect rect) noexcept override final;

    void prepareRendering() noexcept final{}
    void finalizeRendering() noexcept final{}
};
