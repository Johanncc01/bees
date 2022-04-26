/*
 * prjsv 2022
 * Marco Antognini & Jamila Sam
 */

#include <Application.hpp>
#include <Env/Env.hpp>
#include <Env/Hive.hpp>
#include "HiveBigTest.hpp"

#include <cassert>


void FakeHive::addTestBee() {
  // TODO UNCOMMENT WHEN HIVE::ADDBEE IS CODED
  addBee();
}

std::string HiveBigTest::getWindowTitle() const {
  return getAppConfig().window_title  + " (HiveBigTest)";
}

void HiveBigTest::onRun()
{
    // Setup stats
    Application::onRun();
    // add Stats graphs
    setStats(false);
    resetStats();
}

void HiveBigTest::onUpdate(sf::Time){
  if (mHiveCreated) {
    auto dummyRadius = 1;
    mLastHive = (FakeHive*)getEnv().getCollidingHive(Collider(mPosOfLastCreatedHive, dummyRadius));
    assert(mLastHive != nullptr);
    std::cout << "New hive created\n";
    mHiveCreated=false;
  }
}

void HiveBigTest::onEvent(sf::Event event, sf::RenderWindow&)
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

      // Save the world   ─=≡ΣΞ(つಠل͜ಠ)つ
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

        case sf::Keyboard::B:
            if (mLastHive != nullptr) {
          mLastHive->addTestBee();
          std::cout << "bee added" << std::endl;
            }
            break;

        case sf::Keyboard::H:
            mShowHiveableZone = true;
        break;
        }
    } else if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
        default: break;

        case sf::Keyboard::H:
        {
        mShowHiveableZone = false;
        auto position = getCursorPositionInView();
        if (getEnv().addHiveAt(position)) {
          mHiveCreated = true;
          mPosOfLastCreatedHive=position;
        }
        else {
          std::cout << "Couldn't create new hive\n";
            }

    }
            break;

        // Add flower: try to create a new one and disable visualisation zone
        case sf::Keyboard::F:
            mShowFlowerZone = false;
            if (getEnv().addFlowerAt(getCursorPositionInView())) {
                std::cout << "New flower created\n";
            } else {
                std::cout << "Couldn't create new flower\n";
            }
            break;
    }
    }
}

void HiveBigTest::onDraw(sf::RenderTarget& target)
{
    // Render visualisation for hiveable
    if (mShowHiveableZone) {
        auto pos = getCursorPositionInView();
    // TODO: UNCOMMENT IF ENV::DRAWHIVEABLEZONE IS CODED
        getEnv().drawHiveableZone(target, pos);
    }

    if (mShowFlowerZone) {
        auto pos = getCursorPositionInView();
    // UNCOMMENT WHEN THE METHOD IS CODED
        getEnv().drawFlowerZone(target, pos);
    }
}

void HiveBigTest::onSimulationStart()
{
  Application::onSimulationStart();
  // add more actions here if needed
}

void HiveBigTest::resetStats(){
  Application::resetStats();
  // add Stats graphs when needed
}

std::string HiveBigTest::getHelpTextFile() const {
  return RES_LOCATION + "help-hiveTest.txt";
}
