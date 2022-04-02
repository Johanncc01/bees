/*
 * prjsv 2022
 * Marco Antognini & Jamila Sam
 */

#pragma once

#include <Application.hpp>

class FlowerTest : public Application
{
public:
  using Application::Application;

  virtual void onRun() override final;
  virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
  virtual void onSimulationStart() override final;
  
  virtual void onDraw(sf::RenderTarget& target) override final;
  virtual void resetStats() override final;
   virtual std::string getHelpTextFile() const override final;
   std::string virtual getWindowTitle() const override final;
private:
    bool mShowFlowerZone   = false; ///< Enable/disable flower zone visualisation
};

IMPLEMENT_MAIN(FlowerTest);

