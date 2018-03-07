#pragma once
// GLFW
#include <GLFW/glfw3.h>
#include <map>
class MInputState
{
public:

	static std::map<uint32_t, uint32_t> keySet;
	static bool keyStates[256];
	static size_t keyCount;

	static void SetAllKeys(uint32_t* keys, size_t count)
	{
		keySet.clear();
		for (size_t i = 0; i < count; i++)
		{
			keySet[i] = keys[i];
		}
	}

	static void SetKeys(uint32_t* ids, uint32_t* keys, size_t count)
	{
		for (size_t i = 0; i < count; i++)
		{
			keySet[ids[i]] = keys[i];
		}
	}

	static void SetKey(uint32_t id, uint32_t key)
	{
		keySet[id] = key;
	}
};

std::map<uint32_t, uint32_t> MInputState::keySet;
bool MInputState::keyStates[] = { false };