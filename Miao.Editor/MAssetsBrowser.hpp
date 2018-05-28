#pragma once
#include <string>
#include <iostream>
#include <filesystem>

namespace __stdfs = std::experimental::filesystem;

typedef struct
{
	__stdfs::path path;
	bool isDirectory;
}AssetObject;

class FolderNode
{
public:
	//int level = 0;
	bool isSelected = false;
	bool isOpen = false;
	__stdfs::path path;
	FolderNode* parent;
	std::vector<FolderNode> children;
};

class MAssetsBrowser
{
public:
	static void SetPath(__stdfs::path dirPath)
	{
		currentPath = dirPath;
	}

	static void AppendPath(__stdfs::path dirPath)
	{
		currentPath = currentPath.append(dirPath);
	}

	static bool IsPathChanged()
	{
		return MAssetsBrowser::oldPath != MAssetsBrowser::currentPath;
	}

	static void UpdateList(std::vector<AssetObject> & assets)
	{
		oldPath = currentPath;
		std::vector<AssetObject>().swap(assets);
		for (auto & p : __stdfs::directory_iterator(currentPath))
		{
			AssetObject asset;
			asset.path = p.path();
			asset.isDirectory = __stdfs::is_directory(p);
			assets.push_back(asset);
		}
	}

	static void UpdateTree(FolderNode & root)
	{
		treeUpdateRequired = false;
		root.parent = nullptr;
		root.path = ".";
		SetChildrensForFolderNode(root);
		SetParentForFolderNode(root);
	}

	static __stdfs::path currentPath;
	static bool treeUpdateRequired;

private:

	static void SetChildrensForFolderNode(FolderNode & node)
	{
		for (auto & p : __stdfs::directory_iterator(node.path))
		{
			if (__stdfs::is_directory(p))
			{
				FolderNode child_node;
				//child_node.parent = &node;
				child_node.path = p.path();
				//child_node.level = node.level + 1;
				SetChildrensForFolderNode(child_node);
				node.children.push_back(child_node);
			}
		}
	}

	static void SetParentForFolderNode(FolderNode & node)
	{
		for (auto & child : node.children)
		{
			child.parent = &node;
			SetParentForFolderNode(child);
		}
	}

	static __stdfs::path oldPath;
};

bool MAssetsBrowser::treeUpdateRequired = true;
__stdfs::path MAssetsBrowser::oldPath = "";
__stdfs::path MAssetsBrowser::currentPath = ".";