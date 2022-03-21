/*
 * prjsv 2016-22
 * Marco Antognini & Jamila Sam
 */

#pragma once

#include <string>
#include "JSON/JSON.hpp"
#include <SFML/Graphics.hpp>


class Config
{
private:

  j::Value mConfig;
  bool simulation_debug;
  bool show_world_humidity;
  
public:
  
  Config(const j::Value& cfg);
  
	// enables / disables debug mode
  void switchDebug();
  bool getDebug() const;

  void switchHumidity();
  
  bool  showHumidity() const;
  
  // returns read
  j::Value& getJsonRead(){
    return mConfig;
  }

  j::Value const& getJsonRead() const {
		return mConfig;
	}


	//window
	const double window_simulation_width;
	const double window_simulation_height;
	const double window_stats_height;
  	const double window_control_width;
	const std::string  window_title;
	const int window_antialiasing_level;

	
	// stats
	const double stats_refresh_rate;
	const std::string stats_log_folder = "logs/";
	const std::string stats_log_prefix = "log_";
	const std::string stats_log_header = "# Plot with GNUPLOT : gnuplot -e \"plot for [i=1:6] 'log_0.txt' u i w l title columnheader(i)\"";

	// debug
	const sf::Color debug_text_color = sf::Color::White;
	const size_t default_debug_text_size = 20;


	//simulation (general)

	const double  simulation_time_factor;
	const double  simulation_fixed_step;
	const sf::Time  simulation_time_max_dt;
  //const std::string simulation_background;
  //	const std::string simulation_debug_background;
  //	const int simulation_size;
  //	const int world_size;

  // World
  const std::string world_init_file;
  const int world_cells;
  const int world_size;
  const int world_nb_grass_seeds;
  const int world_nb_water_seeds;
  
  //const bool show_world_humidity;
  const double world_humidity_init_level;
  const double world_humidity_decay_rate;
  const double world_humidity_threshold;
  
  const int world_generation_steps;
  const int world_generation_smoothness_level;
  const double water_seeds_teleport_proba;
  const double smoothness_water_neighbor_ratio;
  const double smoothness_grass_neighbor_ratio;

  const std::string grass_texture;
  const std::string rock_texture;
  const std::string water_texture;
  // flowers
  const int initial_flower_count;
  const int flowers_max_failures;
  const double flower_min_size;
  const double flower_max_size;
  const double flower_manual_size;
  const double flower_nectar_min;
  const double flower_nectar_max;
  const double flower_growth_threshold;
   const double flower_growth_split;
  
  // hives
  const int initial_hive_count;
  const int hives_max_failures;
  const double hive_min_size;
  const double hive_max_size;
  const double hive_manual_size;
  const double hiveable_factor;
  const double hive_initial_nectar;
  const std::string hive_texture;
  const double hive_reproduction_nectar_threshold;
  const int hive_reproduction_max_bees;
  const double hive_reproduction_scout_proba;
  // env
  const int max_flowers;

  // flower generator
  const double flower_generator_delay;
  const bool active_flower_generator;

  // bees
  // more constants can be added if needed
  const std::string scout_texture;
  const std::string worker_texture;
  const double worker_size;
  const double scout_size;
  const double worker_initial_energy;
  const double scout_initial_energy;
  const double worker_speed;
  const double scout_speed;
  const double worker_energy_to_leave_hive;
  const double scout_energy_to_leave_hive;
  const double worker_transfer_rate;
  const double worker_eating_rate;
  const double scout_eating_rate;
  const double worker_pollen_capacity;
  const double worker_harvest_rate;
  const double scout_energy_seek_flowers;
  const int scout_max_sharing;
};


