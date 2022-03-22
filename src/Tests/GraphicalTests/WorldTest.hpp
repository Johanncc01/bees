/*
 * prjsv 2022
 * Marco Antognini & Jamila Sam
 */

#pragma once
#include <Application.hpp>
/*!
 * Test world procedural generation.
 *
 * Initially the world is *not* initialised. One can load a world from a file,
 * or reset the world to initialised the seed and then manually generate the world,
 * or generate a new world using the application settings.
 *
 * Actions:
 *
 *  - I: load world from file
 *
 *  - O: save world to file
 *
 *  - R: reset world -- initialise seeds
 *
 *  - Shift+R: reset world -- generate a new world using cfg settings
 *
 *  - Space: after seeds are initialised, apply one step of the p.g. algorithm
 *
 *  - Shift+Space: idem, but apply 100 steps
 *
 *  - S: after seeds are initialised, apply one smoothing pass
 *
 *  - Shift+S: idem, but apply 10 smoothing passes
 *
 */

class WorldTest: public Application
{
 private:
  World mWorld;
  bool mLoaded = false;
public:
    using Application::Application;

  virtual void onRun() override final;
  virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
  virtual void onSimulationStart() override final;
  
  virtual void onDraw(sf::RenderTarget& target) override final;
  virtual void resetStats() override final;
  virtual Vec2d getEnvSize() const override final;
  virtual std::string getHelpTextFile() const override final;
  std::string virtual getWindowTitle() const override final;

private:
  void resetViewOnLoad();
};

IMPLEMENT_MAIN(WorldTest);

