/*
 * prjsv 2022
 * Marco Antognini & Jamila Sam
 */

#include <Application.hpp>
#include <Env/Env.hpp>
#include "FlowerTest.hpp"

std::string FlowerTest::getWindowTitle() const {
  return getAppConfig().window_title  + " (FlowerTest)";
}

void FlowerTest::onRun()
{
    // Disable flower generator
    getValueConfig()["simulation"]["flower generator"]["active"] = j::boolean(false);
    // Setup stats
    Application::onRun();
    // add Stats graphs
    setStats(false);
    resetStats();
}

void FlowerTest::onEvent(sf::Event event, sf::RenderWindow&)
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
	  //UNCOMMENT WHEN CODED
	  //getEnv().saveWorldToFile();
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
        }
    } else if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
        default: break;
	 /*  UNCOMMENT WHEN THE NEEDED MATERIAL IS CODED 
        // Add flower: try to create a new one and disable visualisation zone
        case sf::Keyboard::F:
            mShowFlowerZone = false;
            if (getEnv().addFlowerAt(getCursorPositionInView())) {
                std::cout << "New flower created\n";
            } else {
                std::cout << "Couldn't create new flower\n";
            }
            break;
	 */
        }
    }
}

void FlowerTest::onDraw(sf::RenderTarget& target)
{
    if (mShowFlowerZone) {
        auto pos = getCursorPositionInView();
	// UNCOMMENT WHEN THE METHOD IS CODED
        //getEnv().drawFlowerZone(target, pos);
    }
}

void FlowerTest::onSimulationStart()
{
  Application::onSimulationStart();
  // add more actions here if needed
}

void FlowerTest::resetStats(){
  Application::resetStats();
  // add Stats graphs when needed
}

std::string FlowerTest::getHelpTextFile() const {
  return RES_LOCATION + "help-flowerTest.txt";
}
