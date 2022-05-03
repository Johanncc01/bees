/*
 * prjsv 2022
 * Marco Antognini & Jamila Sam
 */

#include <Application.hpp>
#include <Env/Env.hpp>
#include <Env/Hive.hpp>
#include <Env/WorkerBee.hpp>
#include <cassert>
#include "ScoutTest.hpp"

std::string ScoutTest::getWindowTitle() const {
  return getAppConfig().window_title  + " (ScoutTest)";
}

void ScoutTest::onRun()
{
    // Setup stats
    Application::onRun();
    // add Stats graphs
    setStats(false);
    resetStats();
}

void ScoutTest::onSimulationStart()
{
  Application::onSimulationStart();
    // Override a few settings

    auto& cfg = getValueConfig()["simulation"];

    auto& initial = cfg["env"]["initial"];
    initial["flower"]["count"] = j::number(0);
    initial["hive"]["count"] = j::number(0);

    auto& hive = cfg["hive"];
    hive["reproduction"]["nectar threshold"] = j::number(1e9); // a big number
    hive["reproduction"]["scout probability"] = j::number(1.);
    auto& generator = cfg["flower generator"];
    generator["active"] = j::boolean(false);
    setConfig(getValueConfig());
}

void ScoutTest::onUpdate(sf::Time){
  if (mHiveCreated) {
    auto dummyRadius = 1;
    mLastHive = (FakeHive*)getEnv().getCollidingHive(Collider(mPosOfLastCreatedHive, dummyRadius));
    assert(mLastHive != nullptr);
    std::cout << "New hive created\n";
    mLastHive->addTestBee();
    mHiveCreated=false;
  }
}
void ScoutTest::onEvent(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        default: break;
	  // Load the world
        case sf::Keyboard::L:
	  setResetting(true);
	  getEnv().loadWorldFromFile();
	  resetStats();           
	  onSimulationStart();
	  createViews();
	  break;
	    
	  // Save the world
        case sf::Keyboard::O:
            getEnv().saveWorldToFile();
            break;
	     // Toggle humidity level visualisation
        case sf::Keyboard::W: {
	  getAppConfig().switchHumidity();
            break;
        }   
        // Add flower: show the area covered by the flower
        case sf::Keyboard::F:
            mShowFlowerZone = true;
            break;

        case sf::Keyboard::H:
            mShowHiveableZone = true;
            break;
        }
    } else if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
        default: break;

	  case sf::Keyboard::L:
	  case sf::Keyboard::R:
            mFlowerCreated = false;
            break;

        // Add flower: try to create a new one and disable visualisation zone
        case sf::Keyboard::F:
            mShowFlowerZone = false;
	    mFlowerPosition = getCursorPositionInView();
            if (getEnv().addFlowerAt(mFlowerPosition)) {
				mFlowerCreated  = true;
                std::cout << "New flower created\n";
            } else {
                std::cout << "Couldn't create new flower\n";
            }
            break;

        case sf::Keyboard::H:
        {
            // Add hive: try to create a new one and disable "hiveable" zone
            mShowHiveableZone = false;
            auto position = getCursorPositionInView();
	    if (getEnv().addHiveAt(position)) {
	      mHiveCreated = true;
	      mPosOfLastCreatedHive=position;
            } else {
                std::cout << "Couldn't create new hive, add flowers first\n";
            }
            break;
        }
        }
    }
}

void ScoutTest::onDraw(sf::RenderTarget& target)
{
    if (mShowFlowerZone) {
        auto pos = getCursorPositionInView();
        getEnv().drawFlowerZone(target, pos);
    }

    if (mShowHiveableZone) {
        auto pos = getCursorPositionInView();
        getEnv().drawHiveableZone(target, pos);
    }
}

void ScoutTest::resetStats(){
  Application::resetStats();
  // add Stats graphs when needed
}

std::string ScoutTest::getHelpTextFile() const {
  return RES_LOCATION + "help-scoutTest.txt";
}
