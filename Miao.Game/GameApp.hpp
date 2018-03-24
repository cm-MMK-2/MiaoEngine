#pragma once
#include <game.hpp>

extern Game* game;


class GameApp :public MGameBase
{
public:
	void OnInit()
	{
		//std::cout << "OnInit called!!!" << std::endl;
#pragma region Shader & Resources
		//load shader
		Shader& textShader = MResourceLoader::LoadShader("Shaders/text_vertex.glsl", "Shaders/text_fragment.glsl", nullptr);
		textShader.UseProgram().SetMatrix4("projection", game->projection);
		textShader.SetInteger("mainTexture", 0);

		//load font
		FT_Face noto = MResourceLoader::LoadFont("Fonts/NotoSansCJK-Regular.ttc");

		//render text test
		//MText* fpsCounter = game->renderer.CreateText(notoSans, L"", Rect(0, 0, 200, 30), 16, glm::vec3(0.5, 0.8f, 0.2f), &textShader, GL_STREAM_DRAW);
		game->renderer.CreateText(noto, L"ABCabc", Rect(0, 0, 200, 30), 12, glm::vec3(0.5f, 0.8f, 0.2f), &textShader);
		game->renderer.CreateText(noto, L"¼òÌå×Ö²âÊÔ", Rect(200, 0, 200, 30), 12, glm::vec3(0.5f, 0.8f, 0.2f), &textShader);
		game->renderer.CreateText(noto, L"·±ów×ÖœyÔ‡", Rect(400, 0, 200, 30), 12, glm::vec3(0.5f, 0.8f, 0.2f), &textShader);
		game->renderer.CreateText(noto, L"ÈÕ±¾ÕZ¥Æ¥¹¥È", Rect(600, 0, 200, 30), 12, glm::vec3(0.5f, 0.8f, 0.2f), &textShader);
		game->renderer.CreateText(noto, L"Spine And Input Test: ¡û:Run(Left) ¡ú:Run(Right) ¡ü:Jump ¡ý:Lie Down", Rect(0, 380, 600, 30), 12, glm::vec3(0.5f, 0.8f, 0.2f), &textShader);
		game->renderer.CreateText(noto, L"Sprite Animation:", Rect(0, 60, 300, 30), 12, glm::vec3(0.5f, 0.8f, 0.2f), &textShader);
		game->renderer.CreateText(noto, L"Scale & Rotation:", Rect(260, 60, 300, 30), 12, glm::vec3(0.5f, 0.8f, 0.2f), &textShader);
		//load shader resources
		Shader& defaultShader = MResourceLoader::LoadShader("Shaders/default_vertex.glsl", "Shaders/default_fragment.glsl", nullptr);
		defaultShader.UseProgram().SetInteger("mainTexture", 0);
		defaultShader.SetMatrix4("projection", game->projection);

		//Texture test
		Texture2D& tex01 = MResourceLoader::LoadTexture("Textures/01.png", true);
		rotImage = game->renderer.CreateImage(&tex01, Rect(600, 80, 256, 256), 0.5f, Vec3(1, 1, 1), &defaultShader);
		game->renderer.CreateImage(&tex01, Rect(300, 80, 128, 128), 0, Vec3(1, 1, 1), &defaultShader);

		//sprite animation test
		Texture2D& anim01 = MResourceLoader::LoadTexture("Textures/sprite1.png", true);
		Shader& spriteSheetShader = MResourceLoader::LoadShader("Shaders/spritesheet_vertex.glsl", "Shaders/spritesheet_fragment.glsl", nullptr);
		spriteSheetShader.UseProgram().SetInteger("mainTexture", 0);
		spriteSheetShader.SetMatrix4("projection", game->projection);
		game->renderer.CreateSpriteSheetAnimation(&anim01, Rect(100, 100, 50, 50), Vec2(0.1f, 0.1667f), 0, Vec3(1, 1, 1), &spriteSheetShader);

		//load shader resources
		Shader& meshShader = MResourceLoader::LoadShader("Shaders/mesh_vertex.glsl", "Shaders/mesh_fragment.glsl", nullptr);
		meshShader.UseProgram().SetInteger("mainTexture", 0);
		meshShader.SetMatrix4("projection", game->projection);
#pragma endregion

#pragma region spine animation test
		SpineData& spineBoyData = MResourceLoader::LoadSpineData("spineboy", "./SpineExport/spineboy.atlas", "./SpineExport/spineboy.skel");
		spineBoy = game->renderer.CreateSpineObject(spineBoyData, &meshShader, Vec2(200, 660), Vec2(0.3f, 0.3f));

		for (size_t i = 0; i < spineBoy->animationCount; i++)
		{
			std::cout << "animation " << i << ":" << spineBoy->animations[i]->name << std::endl;
		}
		spineBoy->PlayAnimation(2, true);
#pragma endregion

#pragma region KeyEvents
		//keys need to be listened
		uint32_t keys[] = { GLFW_KEY_UP, GLFW_KEY_LEFT, GLFW_KEY_DOWN, GLFW_KEY_RIGHT };
		MInputState::SetAllKeys(keys, 4);

		//listen to keys
		game->SetKeyEventListener(
			//keyDownEvent
			[&, keys](uint32_t keyIndex) {
				std::cout << "KeyDown: " << keys[keyIndex] << std::endl;
				switch (keyIndex)
				{
					case 0:
					{
						spineBoy->PlayAnimation(3, true);
					}
					break;
					case 1:
					{
						spineBoy->SetFlipX(true);
						spineBoy->PlayAnimation(4, true);
					}
					break;
					case 2:
					{
						spineBoy->PlayAnimation(0, true);
					}
					break;
					case 3:
					{						
						spineBoy->SetFlipX(false);
						spineBoy->PlayAnimation(4, true);
					}
					break;
					default:
					break;
				}
			},
			//keyUpEvent
			[&, keys](uint32_t keyIndex) {
				std::cout << "KeyUp: " << keys[keyIndex] << std::endl;
				if (!MInputState::keyStates[0] && !MInputState::keyStates[1] && !MInputState::keyStates[2] && !MInputState::keyStates[3])
				{
					spineBoy->PlayAnimation(2, true);
				}
			}
		);
#pragma endregion

	}

	//update every frame
	void OnUpdate(float deltaTime)
	{
		if (MInputState::keyStates[1])
		{
			spineBoy->position.x -= deltaTime * 100;
			//std::cout << "MoveLeft: " << deltaTime << std::endl;
		}
		if (MInputState::keyStates[3])
		{
			spineBoy->position.x += deltaTime * 100;
			//std::cout << "MoveRight: " << deltaTime << std::endl;
		}
		rotImage->rotation += deltaTime;
	}

private:
	MSpineObject* spineBoy;
	MImage* rotImage;
};