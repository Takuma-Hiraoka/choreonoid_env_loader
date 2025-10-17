#pragma once
#include <cnoid/Body>
#include <ik_constraint2_distance_field/ik_constraint2_distance_field.h>

namespace choreonoid_env_loader{
  void generateEnvironment(const std::string project_location,
			   const std::string conf_file,
			   const std::string mesh_dir,
			   std::shared_ptr<moveit_extensions::InterpolatedPropagationDistanceField> field,
			   std::vector<cnoid::BodyPtr>& static_obstacles,
			   std::vector<cnoid::BodyPtr>& movable_obstacles
			   );

}
