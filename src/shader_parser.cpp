#include "shader_parser.hpp"
#include "file_manager.hpp"

#include <regex>
#include <algorithm>

const char* GetFragmentShaderHeader() {
	return
		"#version 330\n"
		"precision highp float;\n"
		"uniform vec3 iResolution;\n"
		"uniform float iTime;\n"
		"uniform float iTimeDelta;\n"
		"uniform float iFrame;\n"
		"uniform float iChannelTime[4];\n"
		"uniform vec4 iMouse;\n"
		"uniform vec4 iDate;\n"
		"uniform float iSampleRate;\n"
		"uniform vec3 iChannelResolution[4];\n"
		"uniform float iAudioSum;\n"
		"uniform float iAudioAverage;\n"
		"in vec4 gl_FragCoord;\n"
		"out vec4 gl_FragColor;\n";
}

ShaderParser::ShaderParser()
{

}

ShaderParser::~ShaderParser()
{

}

ShaderParseResult ShaderParser::Parse(const std::string& filepath)
{
	std::string content = FileManager::ReadFile(filepath).c_str();

	auto actions = parseActions(content);
	processActions(content, actions);

	content.insert(0, GetFragmentShaderHeader());

	ShaderParseResult result;
	result.Actions = actions;
	result.Result = content;
	return result;
}

std::vector<ShaderAction> ShaderParser::parseActions(const std::string& content) const
{
	std::regex action_regex("^\/\/!.*#([a-z]{1,}) \"(.*)\".*$");

	auto action_begin = std::sregex_iterator(content.begin(), content.end(), action_regex);
	auto action_end = std::sregex_iterator();

	std::vector<ShaderAction> actions;

	std::smatch submatch;
	for (std::sregex_iterator i = action_begin; i != action_end; ++i) {
		std::smatch match = *i;
		std::string match_str = match.str();

		if (std::regex_search(match_str, submatch, action_regex)) {
			std::vector<std::string> args;
			for (auto arg = submatch.begin() + 2; arg != submatch.end(); ++arg) {
				args.push_back(*arg);
			}
			actions.push_back(std::make_tuple(submatch[1], args));
		}
	}

	std::reverse(actions.begin(), actions.end());
	return actions;
}

bool ShaderParser::processActions(std::string& content, const std::vector<ShaderAction>& actions)
{
	for (const auto& action : actions) {
		const std::string name = std::get<0>(action);
		const std::vector<std::string> args = std::get<1>(action);
	
		if (name == "include") {
			const std::string include = FileManager::ReadFile("shaders/" + args[0]);
			content.insert(0, include);
		}
	}

	return true;
}
