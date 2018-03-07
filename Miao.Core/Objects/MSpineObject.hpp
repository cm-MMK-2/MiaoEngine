#pragma once
#include <spine/extension.h>
#include <spine/spine.h>
#include "../MResourceLoader.hpp"
#include "MObject.hpp"
#include "../Data/SpineData.hpp"

//spine implemetation
char* _spUtil_readFile(const char* path, int* length) {
	return _readFile(path, length);
}

void _spAtlasPage_createTexture(spAtlasPage* self, const char* path) {
	Texture2D& texture = MResourceLoader::LoadTexture(path);

	// if texture loading failed, self->rendererObject, self->width and
	// self->height remain 0 and we simply return.
	//if (!texture) return;

	// store the Texture on the rendererObject so we can
	// retrieve it later for rendering.
	self->rendererObject = &texture;

	// store the texture width and height on the spAtlasPage
	// so spine-c can calculate texture coordinates for
	// rendering.
	self->width = texture.Width;
	self->height = texture.Height;
}

void _spAtlasPage_disposeTexture(spAtlasPage* self) {
	// if the rendererObject is not set, loading failed
	// so we do not need to dispose of anything.
	Texture2D* texture = (Texture2D*)self->rendererObject;
	if (!texture) return;
	// Dispose the texture
	MResourceLoader::UnloadTexture(texture->ID);
}


#define GL_BLEND_MODE_NORMAL glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
#define GL_BLEND_MODE_ADDITIVE glBlendFunc(GL_ONE, GL_ONE)
#define GL_BLEND_MODE_MULTIPLY glBlendFunc(GL_ZERO, GL_SRC_COLOR)
#define GL_BLEND_MODE_SCREEN glBlendFunc(GL_SRC_ALPHA, GL_ONE)
enum class GLBlendMode
{
	Normal,
	Additive,
	Multiply,
	Screen
};

// A single vertex with UV 
typedef struct spVertex {
	// Position in x/y plane
	float x, y;

	// UV coordinates
	float u, v;

	// Color, each channel in the range from 0-1
	// (Should really be a 32-bit RGBA packed color)
	float r, g, b, a;
} spVertex;

class MSpineObject :public MObject
{
public:
	//"data/spineboy.json", "data/spineboy.skel", "data/spineboy.atlas"
	MSpineObject(SpineData& spineData, Shader* _shader,Vec2 _position, Vec2 _scale, unsigned long _id)
	{
		id = _id;
		//Create setup pose data shared by instance data (spAtlas, spSkeletonData, spAnimationStateData) at game or level startup, dispose it at game or level end.
		//Create instance data(spSkeleton, spAnimationState) when the corresponding game object is created, dispose it when the corresponding game object is destroyed.

		position = _position;
		scale = _scale;

		//--------------------for instance
		animationState = spAnimationState_create(spineData.animationStateData);
		skeleton = spSkeleton_create(spineData.skeletonData);

		this->animations = GetAnimations(&animationCount);

		shader = *_shader;
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

	}
	~MSpineObject()
	{
		spAnimationState_dispose(animationState);
		spSkeleton_dispose(skeleton);

		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}

	void SetFlipX(bool _flipX = false)
	{
		if (_flipX != flipX)
		{
			flipX = !flipX;
			this->scale.x = -this->scale.x;
		}
	}

	void PlayAnimation(int animationId, bool loop)
	{
		if (curAnimation != animationId)
		{
			curAnimation = animationId;
			spAnimationState_setAnimation(animationState, 0, this->animations[animationId], loop);
		}
	}

	void Draw(double dt)
	{
		// First update the animation state by the delta time
		spAnimationState_update(animationState, dt);

		// Next, apply the state to the skeleton
		spAnimationState_apply(animationState, skeleton);

		// Calculate world transforms for rendering
		spSkeleton_updateWorldTransform(skeleton);

		shader.UseProgram();


		//glm::mat4 model;
		//model = glm::translate(model, Vec3(200, 200, 0.0f));

		//model = glm::translate(model, Vec3(0.5f * 100, 0.5f * 100, 0.0f));
		//model = glm::rotate(model, 0.0f, Vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, Vec3(-0.5f * 100, -0.5f * 100, 0.0f));

		//model = glm::scale(model, Vec3(100, 100, 1.0f));

		//shader.SetMatrix4("model", model);

		// Hand off rendering the skeleton to the engine
		drawSkeleton(skeleton);
	}

	Vec2 position;
	Vec2 scale;

	//animation
	spAnimation **animations;
	size_t animationCount;
	//current animation
	int curAnimation = -1;
	bool flipX = false;
private:

#define MAX_VERTICES_PER_ATTACHMENT 2048
	float worldVerticesPositions[MAX_VERTICES_PER_ATTACHMENT];
	spVertex vertices[MAX_VERTICES_PER_ATTACHMENT];

	// Little helper function to add a vertex to the scratch buffer. Index will be increased
	// by one after a call to this function.
	void addVertex(float x, float y, float u, float v, float r, float g, float b, float a, int* index) {
		spVertex* vertex = &vertices[*index];
		vertex->x = x * scale.x + position.x;
		vertex->y = y * scale.y + position.y;
		vertex->u = u;
		vertex->v = v;
		vertex->r = r;
		vertex->g = g;
		vertex->b = b;
		vertex->a = a;
		*index += 1;
	}

	void drawSkeleton(spSkeleton* skeleton) {

		// For each slot in the draw order array of the skeleton
		for (int i = 0; i < skeleton->slotsCount; ++i) {
			spSlot* slot = skeleton->drawOrder[i];

			// Fetch the currently active attachment, continue
			// with the next slot in the draw order if no
			// attachment is active on the slot
			spAttachment* attachment = slot->attachment;
			if (!attachment) continue;

			// Calculate the tinting color based on the skeleton's color
			// and the slot's color. Each color channel is given in the
			// range [0-1], you may have to multiply by 255 and cast to
			// and int if your engine uses integer ranges for color channels.
			float tintR = skeleton->r * slot->r;
			float tintG = skeleton->g * slot->g;
			float tintB = skeleton->b * slot->b;
			float tintA = skeleton->a * slot->a;

			// Fill the vertices array depending on the type of attachment
			Texture2D* texture = nullptr;
			int vertexIndex = 0;
			if (attachment->type == SP_ATTACHMENT_REGION) {
				// Cast to an spRegionAttachment so we can get the rendererObject
				// and compute the world vertices
				spRegionAttachment* regionAttachment = (spRegionAttachment*)attachment;

				// Our engine specific Texture is stored in the spAtlasRegion which was
				// assigned to the attachment on load. It represents the texture atlas
				// page that contains the image the region attachment is mapped to
				texture = (Texture2D*)((spAtlasRegion*)regionAttachment->rendererObject)->page->rendererObject;

				// Computed the world vertices positions for the 4 vertices that make up
				// the rectangular region attachment. This assumes the world transform of the
				// bone to which the slot (and hence attachment) is attached has been calculated
				// before rendering via spSkeleton_updateWorldTransform
				spRegionAttachment_computeWorldVertices(regionAttachment, slot->bone, worldVerticesPositions);

				// Create 2 triangles, with 3 vertices each from the region's
				// world vertex positions and its UV coordinates (in the range [0-1]).
				addVertex(worldVerticesPositions[SP_VERTEX_X1], worldVerticesPositions[SP_VERTEX_Y1],
					regionAttachment->uvs[SP_VERTEX_X1], regionAttachment->uvs[SP_VERTEX_Y1],
					tintR, tintG, tintB, tintA, &vertexIndex);

				addVertex(worldVerticesPositions[SP_VERTEX_X2], worldVerticesPositions[SP_VERTEX_Y2],
					regionAttachment->uvs[SP_VERTEX_X2], regionAttachment->uvs[SP_VERTEX_Y2],
					tintR, tintG, tintB, tintA, &vertexIndex);

				addVertex(worldVerticesPositions[SP_VERTEX_X3], worldVerticesPositions[SP_VERTEX_Y3],
					regionAttachment->uvs[SP_VERTEX_X3], regionAttachment->uvs[SP_VERTEX_Y3],
					tintR, tintG, tintB, tintA, &vertexIndex);

				addVertex(worldVerticesPositions[SP_VERTEX_X1], worldVerticesPositions[SP_VERTEX_Y1],
					regionAttachment->uvs[SP_VERTEX_X1], regionAttachment->uvs[SP_VERTEX_Y1],
					tintR, tintG, tintB, tintA, &vertexIndex);

				addVertex(worldVerticesPositions[SP_VERTEX_X3], worldVerticesPositions[SP_VERTEX_Y3],
					regionAttachment->uvs[SP_VERTEX_X3], regionAttachment->uvs[SP_VERTEX_Y3],
					tintR, tintG, tintB, tintA, &vertexIndex);

				addVertex(worldVerticesPositions[SP_VERTEX_X4], worldVerticesPositions[SP_VERTEX_Y4],
					regionAttachment->uvs[SP_VERTEX_X4], regionAttachment->uvs[SP_VERTEX_Y4],
					tintR, tintG, tintB, tintA, &vertexIndex);
			}
			else if (attachment->type == SP_ATTACHMENT_MESH) {
				// Cast to an spRegionAttachment so we can get the rendererObject
				// and compute the world vertices
				spMeshAttachment* mesh = (spMeshAttachment*)attachment;

				// Check the number of vertices in the mesh attachment. If it is bigger
				// than our scratch buffer, we don't render the mesh. We do this here
				// for simplicity, in production you want to reallocate the scratch buffer
				// to fit the mesh.
				if (mesh->super.worldVerticesLength > MAX_VERTICES_PER_ATTACHMENT) continue;

				// Our engine specific Texture is stored in the spAtlasRegion which was
				// assigned to the attachment on load. It represents the texture atlas
				// page that contains the image the mesh attachment is mapped to
				texture = (Texture2D*)((spAtlasRegion*)mesh->rendererObject)->page->rendererObject;

				// Computed the world vertices positions for the 4 vertices that make up
				// the rectangular region attachment. This assumes the world transform of the
				// bone to which the slot (and hence attachment) is attached has been calculated
				// before rendering via spSkeleton_updateWorldTransform
				spMeshAttachment_computeWorldVertices(mesh, slot, worldVerticesPositions);

				// Mesh attachments use an array of vertices, and an array of indices to define which
				// 3 vertices make up each triangle. We loop through all triangle indices
				// and simply emit a vertex for each triangle's vertex.
				for (int i = 0; i < mesh->trianglesCount; ++i) {
					int index = mesh->triangles[i] << 1;
					addVertex(worldVerticesPositions[index], worldVerticesPositions[index + 1],
						mesh->uvs[index], mesh->uvs[index + 1],
						tintR, tintG, tintB, tintA, &vertexIndex);
				}

			}

			// Draw the given mesh.
			// - vertices is a pointer to an array of Vertex structures
			// - start defines from which vertex in the vertices array to start
			// - count defines how many vertices to use for rendering (should be divisible by 3, as we render triangles, each triangle requiring 3 vertices)
			// - texture the texture to use
			// - blendMode the blend mode to use
			//void engine_drawMesh(Vertex* vertices, int start, int count, Texture* texture, BlendMode blendmode);

			drawMesh(vertices, 0, vertexIndex, texture, (int)(slot->data->blendMode));
		}
	}

	void drawMesh(spVertex* vertices, int start, int count, Texture2D* texture, int blendmode)
	{
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, count * 8 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(4 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		texture->Bind();
		switch (blendmode) {
		case SP_BLEND_MODE_NORMAL:
			GL_BLEND_MODE_NORMAL;
			break;
		case SP_BLEND_MODE_ADDITIVE:
			GL_BLEND_MODE_ADDITIVE;
			break;
		case SP_BLEND_MODE_MULTIPLY:
			GL_BLEND_MODE_MULTIPLY;
			break;
		case SP_BLEND_MODE_SCREEN:
			GL_BLEND_MODE_SCREEN;
			break;
		default:
			// unknown Spine blend mode, fall back to
			// normal blend mode
			GL_BLEND_MODE_NORMAL;
		}

		glDrawArrays(GL_TRIANGLES, start, count);

		glBindVertexArray(0);
	}

	spAnimation **GetAnimations(uint32_t* count)
	{
		spSkeletonData* spdata = skeleton->data;
		*count = spdata->animationsCount;
		return spdata->animations;
	}

	GLuint vbo;
	GLuint vao;
	Shader shader;
	spSkeleton* skeleton;
	spAnimationState* animationState;
};