/*
 * prjsv 2022
 * Marco Antognini
 */

#ifndef INFOSV_FINAL_APPLICATION_HPP
#define INFOSV_FINAL_APPLICATION_HPP

#include <Application.hpp>

class FinalApplication : public Application
{
public:
    using Application::Application;

  virtual void onRun() override final;
  virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
  virtual void onSimulationStart() override final;
  
  virtual void onDraw(sf::RenderTarget& target) override final;
  virtual void resetStats() override final;
private:
    bool mShowFlowerZone   = false; ///< Enable/disable flower zone visualisation
    bool mShowHiveableZone = false; ///< Enable/disable hiveable zone visualisation
};

#endif // INFOSV_FINAL_APPLICATION_HPP
