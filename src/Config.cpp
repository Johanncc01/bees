#include "Config.hpp"
#include <JSON/JSONSerialiser.hpp>
// window


Config::Config (const j::Value& cfg): mConfig(cfg)
, simulation_debug(mConfig["debug"].toBool())
, show_world_humidity(mConfig["simulation"]["world"]["show humidity"].toBool())
, window_simulation_width(mConfig["window"]["simulation"]["size"].toDouble())
, window_simulation_height(mConfig["window"]["simulation"]["size"].toDouble())
, window_stats_height(mConfig["window"]["stats"]["height"].toDouble())
 , window_control_width(mConfig["window"]["control"]["width"].toDouble())
, window_title(mConfig["window"]["title"].toString())
, window_antialiasing_level(mConfig["window"]["antialiasing level"].toInt())
 
// stats
, stats_refresh_rate(mConfig["stats"]["refresh rate"].toDouble())

// simulation

, simulation_time_factor(mConfig["simulation"]["time"]["factor"].toDouble())
, simulation_fixed_step(mConfig["simulation"]["time"]["fixed step"].toDouble())
, simulation_time_max_dt(sf::seconds(mConfig["simulation"]["time"]["max dt"].toDouble()))
				   //, simulation_background(mConfig["simulation"]["background"].toString())
				   //, simulation_debug_background(mConfig["simulation"]["debug background"].toString())
				   //, simulation_size(mConfig["simulation"]["world"]["default size"].toInt())								   
// world
 , world_init_file(mConfig["simulation"]["world"]["file"].toString())
, world_cells(mConfig["simulation"]["world"]["default cells"].toInt())
, world_size(mConfig["simulation"]["world"]["default size"].toInt())
, world_nb_grass_seeds(mConfig["simulation"]["world"]["seeds"]["grass"].toInt())
, world_nb_water_seeds(mConfig["simulation"]["world"]["seeds"]["water"].toInt())				   

, world_humidity_init_level(mConfig["simulation"]["world"]["generation"]["humidity"]["initial level"].toDouble())
, world_humidity_decay_rate(mConfig["simulation"]["world"]["generation"]["humidity"]["initial level"].toDouble())
, world_humidity_threshold(mConfig["simulation"]["world"]["generation"]["humidity"]["threshold"].toDouble())				   
  ,world_generation_steps(mConfig["simulation"]["world"]["generation"]["steps"].toInt())
  ,world_generation_smoothness_level(mConfig["simulation"]["world"]["generation"]["smoothness"]["level"].toInt())
  ,water_seeds_teleport_proba(mConfig["simulation"]["world"]["seeds"]["water teleport probability"].toDouble())
  ,smoothness_water_neighbor_ratio(mConfig["simulation"]["world"]["generation"]["smoothness"]["water neighbourhood ratio"].toDouble())
  ,smoothness_grass_neighbor_ratio(mConfig["simulation"]["world"]["generation"]["smoothness"]["grass neighbourhood ratio"].toDouble())
 ,grass_texture(mConfig["simulation"]["world"]["textures"]["grass"].toString())	 ,rock_texture(mConfig["simulation"]["world"]["textures"]["rock"].toString())	,water_texture(mConfig["simulation"]["world"]["textures"]["water"].toString())

// flowers
, initial_flower_count(mConfig["simulation"]["env"]["initial"]["flower"]["count"].toInt())
, flowers_max_failures(mConfig["simulation"]["env"]["initial"]["flower"]["max failures"].toInt())
, flower_min_size(mConfig["simulation"]["env"]["initial"]["flower"]["size"]["min"].toDouble())
, flower_max_size(mConfig["simulation"]["env"]["initial"]["flower"]["size"]["max"].toDouble())
, flower_manual_size(mConfig["simulation"]["env"]["initial"]["flower"]["size"]["manual"].toDouble())
, flower_nectar_min(mConfig["simulation"]["env"]["initial"]["flower"]["nectar"]["min"].toDouble())
, flower_nectar_max(mConfig["simulation"]["env"]["initial"]["flower"]["nectar"]["max"].toDouble())
, flower_growth_threshold(mConfig["simulation"]["flower"]["growth"]["threshold"].toDouble())
, flower_growth_split(mConfig["simulation"]["flower"]["growth"]["split"].toDouble())				      
// hives
, initial_hive_count(mConfig["simulation"]["env"]["initial"]["hive"]["count"].toInt())
, hives_max_failures(mConfig["simulation"]["env"]["initial"]["hive"]["max failures"].toInt())
, hive_min_size(mConfig["simulation"]["env"]["initial"]["hive"]["size"]["min"].toDouble())
, hive_max_size(mConfig["simulation"]["env"]["initial"]["hive"]["size"]["max"].toDouble())
, hive_manual_size(mConfig["simulation"]["env"]["initial"]["hive"]["size"]["manual"].toDouble())
,hiveable_factor(mConfig["simulation"]["env"]["initial"]["hive"]["hiveable factor"].toDouble())
,hive_initial_nectar(mConfig["simulation"]["hive"]["initial"]["nectar"].toDouble())
,hive_texture(mConfig["simulation"]["hive"]["texture"].toString())
,hive_reproduction_nectar_threshold(mConfig["simulation"]["hive"]["reproduction"]["nectar threshold"].toDouble())
,hive_reproduction_max_bees(mConfig["simulation"]["hive"]["reproduction"]["max bees"].toInt())				   
,hive_reproduction_scout_proba(mConfig["simulation"]["hive"]["reproduction"]["scout probability"].toDouble())	
  // env
, max_flowers(mConfig["simulation"]["env"]["max flowers"].toInt())

// flower generator
,flower_generator_delay(mConfig["simulation"]["flower generator"]["delay"].toDouble())
,active_flower_generator(mConfig["simulation"]["flower generator"]["active"].toBool())
 // bees				   
,scout_texture(mConfig["simulation"]["bees"]["scout"]["texture"].toString())
,worker_texture(mConfig["simulation"]["bees"]["worker"]["texture"].toString())
,worker_size(mConfig["simulation"]["bees"]["worker"]["size"].toDouble())
,scout_size(mConfig["simulation"]["bees"]["scout"]["size"].toDouble())
,worker_initial_energy(mConfig["simulation"]["bees"]["worker"]["energy"]["initial"].toDouble())
,scout_initial_energy(mConfig["simulation"]["bees"]["scout"]["energy"]["initial"].toDouble())
,worker_speed(mConfig["simulation"]["bees"]["worker"]["speed"].toDouble())
,scout_speed(mConfig["simulation"]["bees"]["scout"]["speed"].toDouble())
,worker_energy_to_leave_hive(mConfig["simulation"]["bees"]["worker"]["energy"]["to leave hive"].toDouble())
,scout_energy_to_leave_hive(mConfig["simulation"]["bees"]["scout"]["energy"]["to leave hive"].toDouble())
,worker_transfer_rate(mConfig["simulation"]["bees"]["worker"]["transfer rate"].toDouble())
,worker_eating_rate(mConfig["simulation"]["bees"]["worker"]["energy"]["consumption rates"]["eating"].toDouble())
,scout_eating_rate(mConfig["simulation"]["bees"]["scout"]["energy"]["consumption rates"]["eating"].toDouble())
,worker_pollen_capacity(mConfig["simulation"]["bees"]["worker"]["max pollen capacity"].toDouble())
,worker_harvest_rate(mConfig["simulation"]["bees"]["worker"]["harvest rate"].toDouble())
,scout_energy_seek_flowers(mConfig["simulation"]["bees"]["scout"]["energy"]["to seek flowers"].toDouble())
,scout_max_sharing(mConfig["simulation"]["bees"]["scout"]["sharing"]["max"].toInt())				      
// end				   
{ }


								   

// for debugging purposes
void Config::switchDebug(){
	mConfig["debug"] = j::boolean(!mConfig["debug"].toBool());
	simulation_debug = mConfig["debug"].toBool();
}

bool Config::getDebug() const {
return simulation_debug;
}

bool Config::showHumidity() const {
  return show_world_humidity;
}


void Config::switchHumidity(){
  mConfig["simulation"]["world"]["show humidity"] = j::boolean(!mConfig["simulation"]["world"]["show humidity"].toBool());
	show_world_humidity = mConfig["simulation"]["world"]["show humidity"].toBool();
}


