#pragma once
#include <components/Model.hpp>

class ModelImporter 
{
public:
	static const std::shared_ptr<Model> Load(const std::string& filename);
};