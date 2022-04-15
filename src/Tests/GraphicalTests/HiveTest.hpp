/*
 * prjsv 2022
 * Marco Antognini & Jamila Sam
 */

#pragma once

#include <Application.hpp>
/*!
 * Test Hive creation in toric world + Generic Bee random movement
 *
 * This test has the ability to manually create Hive with `H` under
 * the cursor position (after testing whether the position is "hiveable"
 * or not). Additionally, generic bees can be added to the last created
 * hive, if any, using `B`.
 *
 */

class FakeHive : public Hive {

public:
  void addTestBee(); 
};

class HiveTest : public Application
{
public:
    using Application::Application;

  virtual void onRun() override final;
  virtual void onUpdate(sf::Time) override final;
  virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
  virtual void onSimulationStart() override final;
  
  virtual void onDraw(sf::RenderTarget& target) override final;
  virtual void resetStats() override final;
  virtual std::string getHelpTextFile() const override final;
  std::string virtual getWindowTitle() const override final;
private:
  
   bool  mShowHiveableZone = false;   ///< Enable/disable hiveable zone visualisation
  FakeHive* mLastHive         = nullptr; ///< Keep track of the last hive that was created
  bool  mHiveCreated = false;
  Vec2d mPosOfLastCreatedHive;
};

IMPLEMENT_MAIN(HiveTest);
