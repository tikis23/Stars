#pragma once
#include "System.h"
#include <filesystem>
#include <string>
#include <vector>
#include <deque>
/* Save Format:

						suns:
						[
							sun:
							[
		
							]
							sun:
							[

							]
						]
						orbits:
						[
							orbit:
							[
								planet:
								[

								]
								moon:
								[

								]
								moon:
								[

								]
							]
						]

*/

class SaveManager
{
public:
	static void Save(const char* path, System* system);
	static System* Load(const char* path);
	static void Menu(System** system);
private:
	//struct browserState
	//{
	//	std::string pathString;
	//	int selection = -1;
	//};
	struct historyEntry
	{
		std::string directoryPath;
		std::string selectedItemPath;
		bool operator==(const historyEntry& test) const
		{
			return (test.directoryPath == this->directoryPath);
		}
	};
	static std::vector<std::string> extractPath(std::string path);
	static std::string combinePath(std::vector<std::string>& path);

	static bool BrowserSave(std::string path, System* system);
	static bool BrowserLoad(std::string path, System** system);
	static void Search(std::string path, std::string item);
	static int FileSystem(std::filesystem::directory_entry it, int index);
	static System* FileBrowser(System* system);
	static int searchStage; // stage 0 == noSearch&&noResult; stage 1 == search&&noResult; stage 2 == noSearch&&result;
	static std::deque<historyEntry> historyUndo;
	static std::deque<historyEntry> historyRedo;
	static std::vector<std::filesystem::directory_entry> searchResults;
	static historyEntry currentState;
	static std::string saveExtension;

	SaveManager();
};