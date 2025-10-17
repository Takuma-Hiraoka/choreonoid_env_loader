#include <choreonoid_env_loader/choreonoid_env_loader.h>
#include <cnoid/URDFBodyLoader>
#include <cnoid/EigenUtil>
#include <filesystem>
#include <tinyxml2.h>

namespace choreonoid_env_loader{
  void generateEnvironment(const std::string project_location,
			   const std::string conf_file,
			   const std::string mesh_dir,
			   std::shared_ptr<moveit_extensions::InterpolatedPropagationDistanceField> field,
			   std::vector<cnoid::BodyPtr>& static_obstacles,
			   std::vector<cnoid::BodyPtr>& movable_obstacles
			   ) {
    static_obstacles.clear();
    movable_obstacles.clear();
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(std::string(project_location + "/" + conf_file).c_str())) std::cerr << "Cannot open " << project_location + "/" + conf_file << std::endl;

    tinyxml2::XMLElement* root = doc.FirstChildElement("robot");

    if (!root) std::cerr << "No <robot> element found" << std::endl;

    for (tinyxml2::XMLElement* link = root->FirstChildElement("link"); link != nullptr; link = link->NextSiblingElement("link")) {
      std::string name = link->Attribute("name");
      if (name == "world") continue;
      std::string category = link->Attribute("category");
      std::string model = link->Attribute("model");

      cnoid::URDFBodyLoader loader;
      cnoid::BodyPtr body = new cnoid::Body();
      if(loader.load(body, project_location + "/" + model + "_" + category + ".urdf")) {
	body->setName(name);
	body->rootLink()->setName(name);
	static_obstacles.push_back(body);
      } else if (loader.load(body, mesh_dir + "/" + category + "/" + model + "/" + model + ".urdf")){
	body->setName(name);
	body->rootLink()->setName(name);
	movable_obstacles.push_back(body);
      } else std::cerr << "cannnot load urdf of link " << name << std::endl;
    }

    for (tinyxml2::XMLElement* joint = root->FirstChildElement("joint"); joint != nullptr; joint = joint->NextSiblingElement("joint")) {
      tinyxml2::XMLElement* child = joint->FirstChildElement("child");
      std::string name = child->Attribute("link");
      tinyxml2::XMLElement* origin = joint->FirstChildElement("origin");
      cnoid::Matrix3 rot;
      {
	std::vector<double> rpy;
	std::stringstream ss(origin->Attribute("rpy"));
	double v;
	while (ss >> v) rpy.push_back(v);
	if (rpy.size() != 3) std::cerr << "error! length of " << name << " rpy is not 3" << std::endl;
	rot = cnoid::rotFromRpy(rpy[0], rpy[1], rpy[2]);
      }
      cnoid::Vector3 pos; 
      {
	std::vector<double> xyz;
	std::stringstream ss(origin->Attribute("xyz"));
	double v;
	while (ss >> v) xyz.push_back(v);
	if (xyz.size() != 3) std::cerr << "error! length of " << name << " xyz is not 3" << std::endl;
        pos = cnoid::Vector3(xyz[0], xyz[1], xyz[2]);
      }
      for (int i=0; i<static_obstacles.size(); i++) {
	if (name == static_obstacles[i]->name()) {
	  static_obstacles[i]->rootLink()->p() = pos;
	  static_obstacles[i]->rootLink()->R() = rot;
	}
      }
      for (int i=0; i<movable_obstacles.size(); i++) {
	if (name == movable_obstacles[i]->name()) {
	  movable_obstacles[i]->rootLink()->p() = pos;
	  movable_obstacles[i]->rootLink()->R() = rot;
	}
      }
    }
     
  }

}
