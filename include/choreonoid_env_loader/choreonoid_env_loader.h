#pragma once
#include <cnoid/Body>

namespace choreonoid_env_loader{
  void generateEnvironment(const std::string project_location,
			   const std::string conf_file,
			   const std::string mesh_dir,
			   std::vector<cnoid::BodyPtr>& obstacles
			   );
  void generateEnvironment(const std::string project_location,
			   const std::string conf_file,
			   const std::string mesh_dir,
			   std::vector<cnoid::BodyPtr>& static_obstacles,
			   std::vector<cnoid::BodyPtr>& movable_obstacles
			   );
}
