/*
 * prjsv 2022
 * Marco Antognini & Jamila Sam
 */

#include <Application.hpp>
#include <Env/World.hpp>
#include "WorldTest.hpp"

std::string WorldTest::getWindowTitle() const {
  return getAppConfig().window_title  + " (WorldTest)";
}

void WorldTest::onRun()
{
    // Setup stats
   Application::onRun();
   // add Stats graphs
   setStats(false);
   resetStats();
}

void WorldTest::onSimulationStart()
{
  Application::onSimulationStart();
  setIgnoreEnv(true);
  // add more actions here if needed
}

Vec2d WorldTest::getEnvSize() const
{
  auto size(mLoaded ? mWorld.getSize() :
	    getAppConfig().world_size);
  return {size,size};
}

void WorldTest::onEvent(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        default: break;

        case sf::Keyboard::L:
	  // TODO : UNCOMMENT WHEN THE METHOD IS CODED
          //  mWorld.loadFromFile();
	    resetViewOnLoad();
            break;

        case sf::Keyboard::O:
	  // TODO : UNCOMMENT WHEN THE METHOD IS CODED
	  //mWorld.saveToFile();
            break;

        case sf::Keyboard::G:
	  mWorld.reset(event.key.shift ? true : false);
	  resetViewOnLoad();
            break;

        case sf::Keyboard::S:
	   // TODO : UNCOMMENT WHEN THE METHOD IS CODED
	  //mWorld.smooths(event.key.shift ? 10 : 1, true);
            break;

        case sf::Keyboard::P:
	   // TODO : UNCOMMENT WHEN THE METHOD IS CODED
	  //mWorld.steps(event.key.shift ? 100 : 1, true);
            break;
        }
    }
}

void WorldTest::onDraw(sf::RenderTarget& target)
{
    mWorld.drawOn(target);
}

void WorldTest::resetStats(){
  Application::resetStats();
  // add specific graphs if needed
}

void WorldTest::resetViewOnLoad(){
  mLoaded= true;
  onSimulationStart();
  createViews();

}
std::string WorldTest::getHelpTextFile() const {
  return RES_LOCATION + "help-worldTest.txt";
}
