/*
 * prjsv 2022
 * Marco Antognini & Jamila Sam
 */

#pragma once

#include <Application.hpp>
/*!
 * This tests the behaviour of workers. When a hive is created
 * (using `H`), a worker is created as well. Note that hives can
 * be created only after a flower was planted.
 */

class FakeHive : public Hive {

public:
  Bee* addTestBee(){
    return addBee(0); // add a worker bee (proba. of ScoutBee == 0)
  }
};

class WorkerTest : public Application
{
public:
    using Application::Application;

  virtual void onRun() override final;
  virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
  virtual void onSimulationStart() override final;
  virtual void onUpdate(sf::Time) override final;
  virtual void onDraw(sf::RenderTarget& target) override final;
  virtual void resetStats() override final;
  virtual std::string getHelpTextFile() const override final;
  std::string virtual getWindowTitle() const override final;
  
  virtual ~WorkerTest() {}
private:
  bool mShowFlowerZone   = false; ///< Enable/disable flower zone visualisation
  bool mShowHiveableZone = false; ///< Enable/disable hiveable zone visualisation
  FakeHive* mLastHive         = nullptr; ///< Keep track of the last hive that was created
  bool  mFlowerCreated    = false;
  Vec2d mFlowerPosition;
  bool  mHiveCreated = false;
  Vec2d mPosOfLastCreatedHive;
};
IMPLEMENT_MAIN(WorkerTest);
