#pragma once
#include <unordered_map>
#include <string>

//    /\_/\
//   ( o.o )      <---- el gato pro
//    > ^ <

enum class RES_TYPE
{
	SHADER,
	TEXTURE,
	MODEL,
	CUBEMAP,
	// m�s... quiz�
};

class ResourceManager
{
public:
	// cach�:
	std::unordered_map<std::string, void*> res_cache;
	int default_texture;

	ResourceManager();
	~ResourceManager() = default;

	void* load_resource(RES_TYPE tipo, const std::string& path);

private:
	void* load_external_resource(RES_TYPE tipo, const std::string& path);
};
