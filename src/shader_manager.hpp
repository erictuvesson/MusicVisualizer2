#pragma once

/**
 * ShaderManager, manages all currently loaded shaders and it's resources.
 */
class ShaderManager
{
public:
	ShaderManager();
	virtual ~ShaderManager();

	void Update(float elapsedTime);
};
