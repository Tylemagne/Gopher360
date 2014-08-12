#include "Configuration.h"

Configuration::Configuration(const char* fileName)
{
	if (ini_parse(fileName, handler, this) < 0) {
		printf("\nCouldn't load '%s', creating the standard file\n", fileName);
		DumpInitialIni(fileName);
	}
}

void Configuration::DumpInitialIni(const char* fileName)
{
	// print speed stages
	FILE* f = fopen(fileName, "w");
	fprintf(f, "[speedStages]\nspeed_low = 0.000075\nspeed_med = 0.000125\nspeed_high = 0.000175\n");
	fclose(f);
	ini_parse(fileName, handler, this);
}

float Configuration::GetSpeedStage(int stage)
{
	const char* stages[] = { "low", "med", "high" };
	std::string stageName = "speedStages.speed_";
	stageName += stages[stage];
	return GetFloat(options[stageName]);
}

float Configuration::GetFloat(const std::string value)
{
	return std::stof(value);
}

void Configuration::InsertIntoOptionsMap(const std::string optionName, const std::string optionValue)
{
	options.insert(std::make_pair(optionName, optionValue));
}

static int handler(void* user, const char* section, const char* name,
	const char* value)
{
	Configuration * config = (Configuration*)user;
	std::string optionName = section + std::string(".") + std::string(name);
	config->InsertIntoOptionsMap(optionName, std::string(value));
	return 1;
}