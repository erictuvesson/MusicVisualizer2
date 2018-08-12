#pragma once

#include <string>
#include <tuple>
#include <vector>

typedef std::tuple<std::string, std::vector<std::string>> ShaderAction;

struct ShaderParseResult
{
	std::vector<ShaderAction> Actions;
	std::string Result;
};

class ShaderParser
{
public:
	ShaderParser();
	virtual ~ShaderParser();

	ShaderParseResult Parse(const std::string& filepath);

private:
	std::vector<ShaderAction> parseActions(const std::string& content) const;
	bool processActions(std::string& content, const std::vector<ShaderAction>& actions);
};
