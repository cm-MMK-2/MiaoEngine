#pragma once

#include <GLFW/glfw3.h>
#include "Miao.Game/GameApp.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "MAssetsBrowser.hpp"
#include "SteamMatch.hpp"

class MainGUI
{
public:
	static MainGUI* Create(GLFWwindow *_window) {
		if (instance == nullptr)
		{
			instance = new MainGUI(_window);
		}
		return instance;
	}

	static void Destroy() {
		if (instance != nullptr)
		{
			delete instance;
		}
	}

	//called when main window resize, adjust gui windows' size
	static void Resize(int width, int height)
	{
		if (instance != nullptr)
		{
			instance->windowWidth = width;
			instance->windowHeight = height;
		}
	}

	//update every frame
	void Update()
	{
		ImGui_ImplGlfwGL3_NewFrame();

		UpdateMainMenu();

		UpdateAssetBrowser();

		UpdateGameView();

		UpdateSteamMatchMaking();
	}

	void Render() {
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
	}

private:
	static MainGUI* instance;
	GLFWwindow *mainWindow;
	ImGuiIO* io;
#pragma region main menu
#pragma endregion
	int windowWidth;
	int windowHeight;
#pragma region assets browser
	static char folder_icon[];
	static char file_icon[];
	static char folder_open_icon[];
	FolderNode tree;
	FolderNode* lastSelectedFolder = nullptr;
	std::vector<AssetObject> assets;
	char assetsWindowTitle[256];
#pragma endregion
#pragma region game view
	bool gameViewVisible = false;
	bool playGameView = false;
	ImVec2 gameViewSize = ImVec2(1280, 720);
	GLuint gameViewTexture;
	GameApp gameApp;
#pragma endregion

#pragma region steam match making
	SteamMatch steamMatch;
#pragma endregion
	//constructor
	MainGUI(GLFWwindow *_window)
	{
		mainWindow = _window;
		// Setup Dear ImGui binding
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		io = &ImGui::GetIO();
		io->Fonts->AddFontFromFileTTF("../fonts/NotoSansCJK-Regular.ttc", 18.0f);
		//io.Fonts->AddFontFromFileTTF("../fonts/NotoSansCJK-Regular.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesChinese());
		//io.Fonts->AddFontFromFileTTF("../fonts/NotoSansCJK-Regular.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		static const ImWchar ranges[] ={0xf15b, 0xf15b, 0xf07b, 0xf07c, 0};
		io->Fonts->AddFontFromFileTTF("../fonts/fa-solid-900.ttf", 16.0f, &icons_config,  &ranges[0]);
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
		ImGui_ImplGlfwGL3_Init(mainWindow, true);
		glfwGetWindowSize(mainWindow, &windowWidth, &windowHeight);
		
		//style
		ImGuiStyle& style = ImGui::GetStyle();

		style.GrabRounding = 0.f;
		style.WindowRounding = 0.f;
		style.ScrollbarRounding = 3.f;
		style.FrameRounding = 3.f;
		style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
		style.Colors[ImGuiCol_Text] = ImVec4(0.99f, 0.99f, 0.99f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.96f);
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.3f, 0.3f, 0.3f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.4f, 0.62f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.32f, 0.42f, 0.55f, 1.00f);
		style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
		if (Game::Create(gameViewSize.x, gameViewSize.y, &gameApp) == 0)
		{
			gameViewTexture = game->CreateSceneTexture(mainWindow);
		}
	}

	//destructor
	~MainGUI()
	{
		// Cleanup
		ImGui_ImplGlfwGL3_Shutdown();
		ImGui::DestroyContext();
	}

	//display assets folders
	void LoopFolderNodes(FolderNode& node)
	{
		using namespace ImGui;
		if (&node != &tree)
		{
			Indent();
		}
		for (auto& folder : node.children)
		{
			if (Selectable((std::string(folder.isOpen ? folder_open_icon : folder_icon) + folder.path.filename().u8string()).c_str(), folder.isSelected, ImGuiSelectableFlags_AllowDoubleClick))
			{
				if (IsMouseDoubleClicked(0))
				{
					folder.isOpen = !folder.isOpen;
				}
				else
				{
					if (lastSelectedFolder)
					{
						lastSelectedFolder->isSelected = false;
					}
					lastSelectedFolder = &folder;
					folder.isSelected = true;
					MAssetsBrowser::SetPath(folder.path);
				}
			}
			if (folder.isOpen)
			{
				LoopFolderNodes(folder);
			}
		}
		Unindent();
	}

	void UpdateMainMenu()
	{
		using namespace ImGui;
		if (BeginMainMenuBar())
		{
			if (BeginMenu("File"))
			{
				if (MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
				if (MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
				if (MenuItem("Exit", "Alt+F4")) { glfwSetWindowShouldClose(mainWindow, GL_TRUE); }
				ImGui::EndMenu();
			}
			if (BeginMenu("Window"))
			{
				MenuItem("Preview", NULL, &gameViewVisible);
				ImGui::EndMenu();
			}
			EndMainMenuBar();
		}
	}

	void UpdateAssetBrowser()
	{
		using namespace ImGui;
		SetNextWindowPos(ImVec2(0, windowHeight * 3 / 4));
		SetNextWindowSize(ImVec2(windowWidth, windowHeight / 4));

		if (MAssetsBrowser::IsPathChanged())
		{
			MAssetsBrowser::UpdateList(assets);
			sprintf(assetsWindowTitle, "assets: %s", MAssetsBrowser::currentPath.u8string().c_str());
		}
		if (MAssetsBrowser::treeUpdateRequired)
		{
			MAssetsBrowser::UpdateTree(tree);
		}
		Begin(assetsWindowTitle, NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		BeginChild("AssetsSelection", ImVec2(windowWidth / 3, 0));
		LoopFolderNodes(tree);
		EndChild();
		SameLine();
		BeginChild("AssetsList");
		for (auto& asset : assets)
		{
			if (Selectable((std::string(asset.isDirectory ? folder_icon : file_icon) + asset.path.filename().u8string()).c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
			{
				if (asset.isDirectory && IsMouseDoubleClicked(0))
				{
					FolderNode* parent;
					if (lastSelectedFolder)
					{
						lastSelectedFolder->isSelected = false;
						parent = lastSelectedFolder;
					}
					else
					{
						parent = &tree;
					}
					for (auto& folder : parent->children)
					{
						if (folder.path == asset.path)
						{
							folder.isSelected = true;
							//folder.isOpen = true;
							FolderNode* temp = &folder;
							do
							{
								temp->isOpen = true;
								//std::cout << temp->path.u8string() << std::endl;
								temp = temp->parent;
							} while (temp);
							lastSelectedFolder = &folder;
							break;
						}
					}

					MAssetsBrowser::SetPath(asset.path);
				}
			}
		}
		EndChild();
		End();
	}

	void UpdateGameView()
	{
		using namespace ImGui;
		if(gameViewVisible)
		{
			SetNextWindowSize(gameViewSize, ImGuiCond_FirstUseEver);
			Begin("Game", &gameViewVisible);
			if (playGameView) {
				game->RenderSceneTexture(io->DeltaTime);
				glViewport(0, 0, windowWidth, windowHeight);
			}
			// Get the current cursor position (where your window is)
			ImVec2 pos = ImGui::GetCursorScreenPos();
			GetWindowDrawList()->AddImage(
				(void *)gameViewTexture, pos,
				ImVec2(pos.x + gameViewSize.x, pos.y + gameViewSize.y), ImVec2(0, 1), ImVec2(1, 0));
			End();
		}
	}

	void UpdateSteamMatchMaking()
	{
		using namespace ImGui;

		steamMatch.Update();

		Begin("SteamMatch");
		if (Button("List Rooms"))
		{
			steamMatch.RequestLobbiesList();
		}
		End();
	}
};

char MainGUI::folder_icon[] = u8"\uf07b\u0020";
char MainGUI::folder_open_icon[] = u8"\uf07c\u0020";
char MainGUI::file_icon[] = u8"\uf15b\u0020";
MainGUI* MainGUI::instance = nullptr;