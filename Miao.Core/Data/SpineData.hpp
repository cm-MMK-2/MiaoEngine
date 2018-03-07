#pragma once
#include <spine/spine.h>
#include <iostream>
enum class SpineDataType
{
	Binary,
	Json
};

class SpineData
{
public:
	SpineData(const char* atlasPath, const char* dataPath, SpineDataType dataType = SpineDataType::Binary)
	{
		atlas = spAtlas_createFromFile(atlasPath, 0);

		if (dataType == SpineDataType::Binary)
		{
			skeletonData = readSkeletonBinaryData(dataPath, atlas, 1);		
		}
		else
		{
			skeletonData = readSkeletonJsonData(dataPath, atlas, 1);
		}

		// Create the spAnimationStateData
		animationStateData = spAnimationStateData_create(skeletonData);
		animationStateData->defaultMix = 0.5f;
	}
	~SpineData()
	{
		spAnimationStateData_dispose(animationStateData);
		spSkeletonData_dispose(skeletonData);
		spAtlas_dispose(atlas);
	}

	//-------------------------resource
	spSkeletonData* skeletonData;
	spAtlas* atlas;
	spAnimationStateData* animationStateData;
private:
	static spSkeletonData* readSkeletonJsonData(const char* filename, spAtlas* atlas, float scale) {
		spSkeletonJson* json = spSkeletonJson_create(atlas);
		json->scale = scale;
		spSkeletonData* skeletonData = spSkeletonJson_readSkeletonDataFile(json, filename);
		if (!skeletonData) {
			printf("%s\n", json->error);
			return nullptr;
		}
		spSkeletonJson_dispose(json);
		return skeletonData;
	}

	static spSkeletonData* readSkeletonBinaryData(const char* filename, spAtlas* atlas, float scale) {
		spSkeletonBinary* binary = spSkeletonBinary_create(atlas);
		binary->scale = scale;
		spSkeletonData *skeletonData = spSkeletonBinary_readSkeletonDataFile(binary, filename);
		if (!skeletonData) {
			printf("%s\n", binary->error);
			return nullptr;
		}
		spSkeletonBinary_dispose(binary);
		return skeletonData;
	}
};