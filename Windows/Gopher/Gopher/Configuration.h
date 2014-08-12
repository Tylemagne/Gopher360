#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "../inih/cpp/INIReader.h"
#include "../inih/ini.h"
#include <vector>
#include <map>

static int handler(void* user, const char* section, const char* name, const char* value);

class Configuration
{
public:
	Configuration(const char* fileName);
	Configuration(INIReader*);
	float GetSpeedStage(int stage);
	float GetFloat(const std::string value);
	void InsertIntoOptionsMap(const std::string optionName, const std::string optionValue);
private:
	void DumpInitialIni(const char* fileName);
	
	std::map<std::string, std::string> options;
	std::vector<float> speedStages;
};
#endif // CONFIGURATION_HPP