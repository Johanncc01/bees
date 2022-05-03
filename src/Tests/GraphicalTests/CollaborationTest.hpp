/*
 * prjsv 2022
 * Marco Antognini & Jamila Sam
 */

#pragma once

#include <Application.hpp>

/*!
 * This tests the behaviour of scouts. When a hive is created
 * (using `H`), a scout and a worker are created as well.
 */

class FakeHive : public Hive {

public:
  Bee* addTestBee(double scoutProba){
    return addBee(scoutProba);
  }
};

class CollaborationTest : public Application
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
  virtual ~CollaborationTest() {}
private:
  bool mShowFlowerZone   = false; ///< Enable/disable flower zone visualisation
  bool mShowHiveableZone = false; ///< Enable/disable hiveable zone visualisation
  FakeHive* mLastHive         = nullptr; ///< Keep track of the last hive that was created
  bool  mFlowerCreated    = false;
  Vec2d mFlowerPosition;
  bool  mHiveCreated = false;
  Vec2d mPosOfLastCreatedHive;
};
IMPLEMENT_MAIN(CollaborationTest);
