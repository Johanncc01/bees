/*
 * prjsv 2022
 * Marco Antognini
 */

#include <Config.hpp>
#include "FinalApplication.hpp"

#include <cassert>

IMPLEMENT_MAIN(FinalApplication);

void FinalApplication::onRun()
{
  // Setup stats
  Application::onRun();
  // add Stats graphs
  setStats(true);
  resetStats();
}

void FinalApplication::onSimulationStart()
{
  Application::onSimulationStart();
  // add more actions here if needed
}

void FinalApplication::onEvent(sf::Event event, sf::RenderWindow&)
{
     if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        default: break;
	    // Load the world
        case sf::Keyboard::L:
	  setResetting(true);
	  getEnv().loadWorldFromFile();
	  getStats().reset();           
	  onSimulationStart();
	  createViews();
	  break;
	    
	  // Save the world
        case sf::Keyboard::O:
	  // UNCOMMENT IF CODED
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

        case sf::Keyboard::H:
            mShowHiveableZone = true;
            break;
        }
    } else if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
        default: break;

        // Add flower: try to create a new one and disable visualisation zone
        case sf::Keyboard::F:
            mShowFlowerZone = false;
            if (getEnv().addFlowerAt(getCursorPositionInView())) {
                std::cout << "New flower created\n";
            } else {
                std::cout << "Couldn't create new flower\n";
            }
            break;

        // Add hive: try to create a new one and disable "hiveable" zone
        case sf::Keyboard::H:
            mShowHiveableZone = false;
            if (getEnv().addHiveAt(getCursorPositionInView())) {
	      resetStats();
                std::cout << "New hive created\n";
            } else {
                std::cout << "Couldn't create new hive\n";
            }
            break;
        }
    }
}
void FinalApplication::onDraw(sf::RenderTarget& target)
{
    if (mShowFlowerZone) {
        auto pos = getCursorPositionInView();
	// UNCOMMENT IF CODED
        //getEnv().drawFlowerZone(target, pos);
    }

    if (mShowHiveableZone) {
        auto pos = getCursorPositionInView();
	// UNCOMMENT IF CODED
        //getEnv().drawHiveableZone(target, pos);
    }
}

void FinalApplication::resetStats(){
  Application::resetStats();
  /* // UNCOMMENT WHEN STATS ARE CODED
   addGraph(s::GENERAL, { s::FLOWERS, s::HIVES, s::SCOUTS, s::WORKERS, }, 0, 300);
   const auto hives_titles(getAppEnv().getHivesIds());
   addGraph(s::HIVES, hives_titles , 0, 1000);
   setActiveGraph(0);
  */
   
}
