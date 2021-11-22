#define _CRT_SECURE_NO_WARNINGS
#include "SaveManager.h"
#include "ImGuiWindow.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <thread>
#include <Windows.h>
#include <string>

std::string GetCurrentDirectory()
{
    char buffer[1000];
    GetModuleFileNameA(NULL, buffer, 1000);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");

    return std::string(buffer).substr(0, pos);
}

std::string outputVec(glm::vec3 output)
{
    return std::to_string(output.x) + " " + std::to_string(output.y) + " " + std::to_string(output.z);
}

std::string outputVec(glm::vec4 output)
{
    return std::to_string(output.x) + " " + std::to_string(output.y) + " " + std::to_string(output.z) + " " + std::to_string(output.w);
}

std::string normalizeString(std::string str)
{
    for (auto& it : str)
    {
        if (it == '\\')
            it = '/';
    }
    return str;
}

template <typename TP>
std::time_t to_time_t(TP tp)
{
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
        + system_clock::now());
    return system_clock::to_time_t(sctp);
}

std::string convertTimeString(std::string input)
{
    const std::string monthNames[] = {
        "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
    };
    input = input.substr(input.find_first_of(',') + 2, input.length());
    size_t index;
    // day
    int day = std::stoi(input, &index);
    std::string dayString = std::to_string(day);
    if(day < 10)
        dayString = '0' + std::to_string(day);
    // month
    input = input.substr(index + 1, input.length());
    std::string monthString = input.substr(0, input.find(' '));
    int month = 1;
    for (int i = 0; i < 12; i++)
        if (monthString == monthNames[i])
            month = i + 1;
    monthString = std::to_string(month);
    if (month < 10)
        monthString = '0' + std::to_string(month);
    // year
    input = input.substr(input.find_first_of(' '), input.length());
    int year = std::stoi(input, &index);
    input = input.substr(index + 1, input.length());
    return (std::to_string(year) + '-' + monthString + '-' + dayString + ' ' + input);
}

void SaveManager::Save(const char* path, System* system)
{
    if (system == nullptr)
        return;
    std::ofstream output(path, std::ios::out | std::ios::trunc);
    if (output.is_open())
    {
        // suns
        output << "suns:\n[";
        for (auto& it : system->suns)
        {
            // sun
            output << "\n\tsun:\n\t[";
            
            output << "\n\t\tint detail: "      << it->planet->variables.detail;
            output << "\n\t\tbool isSun: "      << it->planet->variables.isSun;
            output << "\n\t\tbool isPlanet: "   << it->planet->variables.isPlanet;
            output << "\n\t\tbool isGas: "      << it->planet->variables.isGas;
            output << "\n\t\tbool isMoon: "     << it->planet->variables.isMoon;
            output << "\n\t\tbool hasRing: "    << it->planet->variables.hasRing;
            output << "\n\t\tfloat maxHeight: " << it->planet->variables.max_height;
            output << "\n\t\tfloat minHeight: " << it->planet->variables.min_height;
            output << "\n\t\tvec3 maxHeightColor: " << outputVec(it->planet->variables.maxColor);
            output << "\n\t\tvec3 minHeightColor: " << outputVec(it->planet->variables.minColor);
            output << "\n\t\tfloat size: "      << it->planet->variables.size;
            output << "\n\t\tfloat mult: "      << it->planet->variables.mult;
            output << "\n\t\tfloat origin: "    << it->planet->variables.origin;
            output << "\n\t\tfloat dist: "      << it->planet->variables.dist;
            output << "\n\t\tfloat speed: "     << it->planet->variables.speed;
            output << "\n\t\tfloat ringSize: "  << it->planet->variables.ringSize;
            output << "\n\t\tvec3 planetRot: "  << outputVec(it->planet->variables.planetRot);
            output << "\n\t\tvec3 ringRot: "    << outputVec(it->planet->variables.ringRot);
            output << "\n\t\tvec4 planetCol: "  << outputVec(it->planet->variables.color);
            output << "\n\t\tvec4 ringCol: "    << outputVec(it->planet->variables.colorRing);

            output << "\n\t]";
        }

        // orbits
        output << "\n]\norbits:\n[";
        for (int i = 0; i < system->orbits.size(); i++)
        {
            // orbit
            output << "\n\torbit:\n\t[";

            // planet
            if (system->orbits[i]->planet != nullptr)
            {
                auto& it = system->orbits[i]->planet;
                output << "\n\t\tplanet:\n\t\t[";

                output << "\n\t\t\tint detail: " << it->variables.detail;
                output << "\n\t\t\tbool isSun: " << it->variables.isSun;
                output << "\n\t\t\tbool isPlanet: " << it->variables.isPlanet;
                output << "\n\t\t\tbool isGas: " << it->variables.isGas;
                output << "\n\t\t\tbool isMoon: " << it->variables.isMoon;
                output << "\n\t\t\tbool hasRing: " << it->variables.hasRing;
                output << "\n\t\t\tfloat maxHeight: " << it->variables.max_height;
                output << "\n\t\t\tfloat minHeight: " << it->variables.min_height;
                output << "\n\t\tvec3 maxHeightColor: " << outputVec(it->variables.maxColor);
                output << "\n\t\tvec3 minHeightColor: " << outputVec(it->variables.minColor);
                output << "\n\t\t\tfloat size: " << it->variables.size;
                output << "\n\t\t\tfloat mult: " << it->variables.mult;
                output << "\n\t\t\tfloat origin: " << it->variables.origin;
                output << "\n\t\t\tfloat dist: " << it->variables.dist;
                output << "\n\t\t\tfloat speed: " << it->variables.speed;
                output << "\n\t\t\tfloat ringSize: " << it->variables.ringSize;
                output << "\n\t\t\tvec3 planetRot: " << outputVec(it->variables.planetRot);
                output << "\n\t\t\tvec3 ringRot: " << outputVec(it->variables.ringRot);
                output << "\n\t\t\tvec4 planetCol: " << outputVec(it->variables.color);
                output << "\n\t\t\tvec4 ringCol: " << outputVec(it->variables.colorRing);

                output << "\n\t\t]";
            }
            // moon
            for (auto& it : system->orbits[i]->moons)
            {
                output << "\n\t\tmoon:\n\t\t[";

                output << "\n\t\t\tint detail: " << it->variables.detail;
                output << "\n\t\t\tbool isSun: " << it->variables.isSun;
                output << "\n\t\t\tbool isPlanet: " << it->variables.isPlanet;
                output << "\n\t\t\tbool isGas: " << it->variables.isGas;
                output << "\n\t\t\tbool isMoon: " << it->variables.isMoon;
                output << "\n\t\t\tbool hasRing: " << it->variables.hasRing;
                output << "\n\t\t\tfloat maxHeight: " << it->variables.max_height;
                output << "\n\t\t\tfloat minHeight: " << it->variables.min_height;
                output << "\n\t\tvec3 maxHeightColor: " << outputVec(it->variables.maxColor);
                output << "\n\t\tvec3 minHeightColor: " << outputVec(it->variables.minColor);
                output << "\n\t\t\tfloat size: " << it->variables.size;
                output << "\n\t\t\tfloat mult: " << it->variables.mult;
                output << "\n\t\t\tfloat origin: " << it->variables.origin;
                output << "\n\t\t\tfloat dist: " << it->variables.dist;
                output << "\n\t\t\tfloat speed: " << it->variables.speed;
                output << "\n\t\t\tfloat ringSize: " << it->variables.ringSize;
                output << "\n\t\t\tvec3 planetRot: " << outputVec(it->variables.planetRot);
                output << "\n\t\t\tvec3 ringRot: " << outputVec(it->variables.ringRot);
                output << "\n\t\t\tvec4 planetCol: " << outputVec(it->variables.color);
                output << "\n\t\t\tvec4 ringCol: " << outputVec(it->variables.colorRing);

                output << "\n\t\t]";
            }
            output << "\n\t]";
        }
        output << "\n]";
        output.close();
    }
}

System* SaveManager::Load(const char* path)
{
    System* system = new System();
    std::ifstream input(path, std::ios::in);
    if (input.is_open())
    {
        std::string s;
        input >> s;
        if (s == "suns:")
        {
            input >> s;
            if (s == "[")
            {
                while (!input.eof())
                {
                    input >> s;
                    if (s == "]")
                        break;
                    else if (s == "sun:")
                    {
                        input >> s;
                        if (s == "[")
                        {
                            PlanetVariables variables;
                            input >> s >> s >> variables.detail;
                            input >> s >> s >> variables.isSun;
                            input >> s >> s >> variables.isPlanet;
                            input >> s >> s >> variables.isGas;
                            input >> s >> s >> variables.isMoon;
                            input >> s >> s >> variables.hasRing;
                            input >> s >> s >> variables.max_height;
                            input >> s >> s >> variables.min_height;
                            input >> s >> s >> variables.maxColor.x >> variables.maxColor.y >> variables.maxColor.z;
                            input >> s >> s >> variables.minColor.x >> variables.minColor.y >> variables.minColor.z;
                            input >> s >> s >> variables.size;
                            input >> s >> s >> variables.mult;
                            input >> s >> s >> variables.origin;
                            input >> s >> s >> variables.dist;
                            input >> s >> s >> variables.speed;
                            input >> s >> s >> variables.ringSize;
                            input >> s >> s >> variables.planetRot.x >> variables.planetRot.y >> variables.planetRot.z;
                            input >> s >> s >> variables.ringRot.x >> variables.ringRot.y >> variables.ringRot.z;
                            input >> s >> s >> variables.color.x >> variables.color.y >> variables.color.z >> variables.color.w;
                            input >> s >> s >> variables.colorRing.x >> variables.colorRing.y >> variables.colorRing.z >> variables.colorRing.w;
                            input >> s;
                            if (s == "]")
                            {
                                system->suns.emplace_back(new System::_orbit_);
                                system->suns.back()->planet = new Planet(variables);
                            }
                            else return nullptr;
                        }
                        else return nullptr;
                    }
                    else return nullptr;
                }
            }
            else return nullptr;
        }
        else return nullptr;

        input >> s;
        if (s == "orbits:")
        {
            input >> s;
            if (s == "[")
            {
                while (!input.eof())
                {
                    input >> s;
                    if (s == "]")
                        break;
                    else if (s == "orbit:")
                    {
                        input >> s;
                        if (s == "[")
                        {
                            system->orbits.emplace_back(new System::_orbit_);
                            while (!input.eof())
                            {
                                input >> s;
                                if (s == "]")
                                    break;
                                else if (s == "planet:")
                                {
                                    input >> s;
                                    if (s == "[")
                                    {
                                        PlanetVariables variables;
                                        input >> s >> s >> variables.detail;
                                        input >> s >> s >> variables.isSun;
                                        input >> s >> s >> variables.isPlanet;
                                        input >> s >> s >> variables.isGas;
                                        input >> s >> s >> variables.isMoon;
                                        input >> s >> s >> variables.hasRing;
                                        input >> s >> s >> variables.max_height;
                                        input >> s >> s >> variables.min_height;
                                        input >> s >> s >> variables.maxColor.x >> variables.maxColor.y >> variables.maxColor.z;
                                        input >> s >> s >> variables.minColor.x >> variables.minColor.y >> variables.minColor.z;
                                        input >> s >> s >> variables.size;
                                        input >> s >> s >> variables.mult;
                                        input >> s >> s >> variables.origin;
                                        input >> s >> s >> variables.dist;
                                        input >> s >> s >> variables.speed;
                                        input >> s >> s >> variables.ringSize;
                                        input >> s >> s >> variables.planetRot.x >> variables.planetRot.y >> variables.planetRot.z;
                                        input >> s >> s >> variables.ringRot.x >> variables.ringRot.y >> variables.ringRot.z;
                                        input >> s >> s >> variables.color.x >> variables.color.y >> variables.color.z >> variables.color.w;
                                        input >> s >> s >> variables.colorRing.x >> variables.colorRing.y >> variables.colorRing.z >> variables.colorRing.w;
                                        input >> s;
                                        if (s == "]")
                                            system->orbits.back()->planet = new Planet(variables);
                                        else return nullptr;
                                    }
                                    else return nullptr;
                                }
                                else if (s == "moon:")
                                {
                                    input >> s;
                                    if (s == "[")
                                    {
                                        PlanetVariables variables;
                                        input >> s >> s >> variables.detail;
                                        input >> s >> s >> variables.isSun;
                                        input >> s >> s >> variables.isPlanet;
                                        input >> s >> s >> variables.isGas;
                                        input >> s >> s >> variables.isMoon;
                                        input >> s >> s >> variables.hasRing;
                                        input >> s >> s >> variables.max_height;
                                        input >> s >> s >> variables.min_height;
                                        input >> s >> s >> variables.maxColor.x >> variables.maxColor.y >> variables.maxColor.z;
                                        input >> s >> s >> variables.minColor.x >> variables.minColor.y >> variables.minColor.z;
                                        input >> s >> s >> variables.size;
                                        input >> s >> s >> variables.mult;
                                        input >> s >> s >> variables.origin;
                                        input >> s >> s >> variables.dist;
                                        input >> s >> s >> variables.speed;
                                        input >> s >> s >> variables.ringSize;
                                        input >> s >> s >> variables.planetRot.x >> variables.planetRot.y >> variables.planetRot.z;
                                        input >> s >> s >> variables.ringRot.x >> variables.ringRot.y >> variables.ringRot.z;
                                        input >> s >> s >> variables.color.x >> variables.color.y >> variables.color.z >> variables.color.w;
                                        input >> s >> s >> variables.colorRing.x >> variables.colorRing.y >> variables.colorRing.z >> variables.colorRing.w;
                                        input >> s;
                                        if (s == "]")
                                            system->orbits.back()->moons.emplace_back(new Planet(variables));
                                        else return nullptr;
                                    }
                                    else return nullptr;
                                }
                                else return nullptr;
                            }
                        }
                        else return nullptr;
                    }
                    else return nullptr;
                }
            }
            else return nullptr;
        }
        else return nullptr;
        input.close();
    }
    else return nullptr;
    return system;
}

void SaveManager::Menu(System** system, Planet** orbitPlanet)
{
    System* temp = FileBrowser(*system);
    if (temp != nullptr)
    {
        *orbitPlanet = nullptr;
        *ImGuiWindow::Variable_int("PlanetSelectedID") = -1;
        *ImGuiWindow::Variable_int("OrbitSelectedID") = -1;
        delete* system;
        *system = temp;
    }
    if (ImGui::BeginMenu("Scene"))
    {
        if (ImGui::MenuItem("Open..", "Ctrl+O"))
            *ImGuiWindow::Variable_bool("Load") = true;
        if (ImGui::BeginMenu("New..", "Ctrl+N"))
        {
            if (ImGui::MenuItem("Generated")) { delete *system; *system = new System{ 2 }; }
            if (ImGui::MenuItem("Empty")) { delete *system; *system = new System{ 0 }; }
            if (ImGui::MenuItem("Predefined")) { delete *system; *system = new System{ 1 }; }
            ImGui::EndMenu();
        }
        if (system != nullptr && ImGui::MenuItem("Save..", "Ctrl+S")) 
            *ImGuiWindow::Variable_bool("Save") = true;
        ImGui::EndMenu();
    }
}

std::vector<std::string> SaveManager::extractPath(std::string path)
{
    std::string temp;
    std::vector<std::string> output;
    for (int i = 0; i < path.length(); i++)
    {
        if (path[i] == '/' || path[i] == '\\')
        {
            if(temp.length() >= 1)
                output.push_back(temp);
            temp = "";
        }
        else if (i == path.length() - 1)
        {
            temp.push_back(path[i]);
            output.push_back(temp);
        }
        else
        {
            temp.push_back(path[i]);
        }
    }
    return output;
}

std::string SaveManager::combinePath(std::vector<std::string>& path)
{
    std::string temp;
    for (auto& it : path)
    {
        temp += it + "/";
    }
    temp.erase(temp.end());
    return temp;
}

void SaveManager::Search(std::string path, std::string item)
{
    searchResults.clear();
    // recursive
    if (*ImGuiWindow::Variable_int("SearchType") == 0)
        for (auto const& it : std::filesystem::recursive_directory_iterator(path, std::filesystem::directory_options::skip_permission_denied))
        {
            if (searchStage != 1)
                break;
            try
            {
                if (it.path().filename().string().find(item) != std::string::npos)
                    searchResults.push_back(it);
            }
            catch (std::exception& e) 
            {
                continue;
            }
        }
    // directory
    else if (*ImGuiWindow::Variable_int("SearchType") == 1)
        for (auto const& it : std::filesystem::directory_iterator(path, std::filesystem::directory_options::skip_permission_denied))
        {
            if (searchStage != 1)
                break;
            try
            {
                if (it.path().filename().string().find(item) != std::string::npos)
                    searchResults.push_back(it);
            }
            catch (std::exception& e)
            {
                continue;
            }
        }
    searchStage = 2;
}

int SaveManager::FileSystem(std::filesystem::directory_entry it, int index)
{
    try
    {
        it.path().string();
    }
    catch (std::exception& e)
    {
        return 0;
    }
    // 0 == nothing | 1 == openFolder | 2 == loadFile
    int returnVal = 0;
    ImGui::PushID(index);

    // date
    std::filesystem::file_time_type file_time = std::filesystem::last_write_time(it.path());
    std::time_t tt = to_time_t(file_time);
    std::tm* gmt = std::localtime(&tt);
    std::stringstream buffer;
    buffer << std::put_time(gmt, "%A, %d %B %Y %H:%M");
    std::string fileDate = convertTimeString(buffer.str());


    // Size
    std::string bytes[] = {
        " B", " KB", " MB", " GB", " TB"
    };
    int byteIndex = 0;
    float size = it.file_size();
    for (int i = 1; i < 5; i++)
    {
        if (size / 1000.0f >= 1)
        {
            size /= 1000.0f;
            byteIndex = i;
        }
    }
    std::string fileSize = std::to_string(size);
    fileSize = fileSize.substr(0, fileSize.find_last_of('.') + 2) + bytes[byteIndex];
    std::string fileType = it.path().filename().extension().string();
    if (it.is_directory())
        fileType = "Folder";

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::ColorConvertFloat4ToU32({ 0.4, 0.4, 0.4, (float)(index % 2 + 1) / 8 }));
    // selectable
    float foldersWidth = 565;
    bool isSelected = (it.path().string() == currentState.selectedItemPath) ? true : false;
    if (ImGui::Selectable(it.path().filename().stem().string().c_str(), isSelected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap | ImGuiSelectableFlags_AllowDoubleClick, { foldersWidth, 20 }))
    {
        if (ImGui::IsMouseDoubleClicked(0))
        {
            if (it.is_directory())
                returnVal = 1;
            else
                returnVal = 2;
        }
        if (!isSelected)
        {
            currentState.selectedItemPath = it.path().string();
            *ImGuiWindow::Variable_string("FileNameBrowser") = it.path().filename().string();
        }
        if (isSelected)
        {
            //if (std::filesystem::path(currentState.selectedItemPath).parent_path().string() + '/' + *ImGuiWindow::Variable_string("FileNameBrowser") == normalizeString(it.path().string()))
            //    *ImGuiWindow::Variable_string("FileNameBrowser") = "";
            //currentState.selectedItemPath = "";
        }
    }
    ImGui::TableNextColumn();
    ImGui::Text(fileType.c_str());
    ImGui::TableNextColumn();
    if (!it.is_directory())
        ImGui::Text(fileSize.c_str());
    ImGui::TableNextColumn();
    ImGui::Text(fileDate.c_str());
    ImGui::PopID();
    return returnVal;
}

bool SaveManager::BrowserLoad(std::string path, System** system)
{
    if (*ImGuiWindow::Variable_bool("Load") == false)
        return false;
    if (!std::filesystem::exists(std::filesystem::path(path)))
        return false;
    if (std::filesystem::path(path).filename().extension().string() != saveExtension)
        return false;
    *system = Load(path.c_str());
    return true;
}

bool SaveManager::BrowserSave(std::string path, System* system)
{
    if (*ImGuiWindow::Variable_bool("Save") == false)
        return false;
    std::filesystem::path fsPath = std::filesystem::path(path);
    if (!std::filesystem::exists(fsPath.parent_path()))
        return false;
    if (!fsPath.has_extension())
        path += saveExtension;
    fsPath = std::filesystem::path(path);
    if (fsPath.extension().string() != saveExtension)
        return false;
    Save(path.c_str(), system);
    return true;
}

System* SaveManager::FileBrowser(System* system)
{
    System* output = nullptr;
    std::string name = "File Browser";
    int historySize = 50;

    bool* popupIsAlive = nullptr;
    bool search = false;
    bool cancelSearch = false;
    if (*ImGuiWindow::Variable_bool("Load") == true)
    {
        popupIsAlive = ImGuiWindow::Variable_bool("Load");
        name = "Load";
        ImGui::OpenPopup(name.c_str());
    }
    else if (*ImGuiWindow::Variable_bool("Save") == true)
    {
        popupIsAlive = ImGuiWindow::Variable_bool("Save");
        name = "Save";
        ImGui::OpenPopup(name.c_str());
    }

    historyEntry currentStateTestForChange = currentState;

    if (ImGui::BeginPopupModal(name.c_str(), popupIsAlive, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2, 3));
        ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, 0.4f);
        // undo
        if (historyUndo.empty())
        {
            ImGui::BeginDisabled();
            if (ImGui::ArrowButton("##historyBack", ImGuiDir_Left))
            {
                if (!historyUndo.empty())
                {
                    historyRedo.push_back(currentState);
                    currentState = historyUndo.back();
                    historyUndo.pop_back();
                    currentStateTestForChange = currentState;
                }
                cancelSearch = true;
                searchStage = 0;
            }
            ImGui::EndDisabled();
        }
        else if (ImGui::ArrowButton("##historyBack", ImGuiDir_Left))
        {
            if (!historyUndo.empty())
            {
                historyRedo.push_back(currentState);
                currentState = historyUndo.back();
                historyUndo.pop_back();
                currentStateTestForChange = currentState;
            }
            cancelSearch = true;
            searchStage = 0;
        }

        // redo
        ImGui::SameLine();
        if (historyRedo.empty())
        {
            ImGui::BeginDisabled();
            if (ImGui::ArrowButton("##historyReverse", ImGuiDir_Right))
            {
                if (!historyRedo.empty())
                {
                    historyUndo.push_back(currentState);
                    currentState = historyRedo.back();
                    historyRedo.pop_back();
                    currentStateTestForChange = currentState;
                }
                cancelSearch = true;
                searchStage = 0;
            }
            ImGui::EndDisabled();
        }
        else if (ImGui::ArrowButton("##historyReverse", ImGuiDir_Right))
        {
            if (!historyRedo.empty())
            {
                historyUndo.push_back(currentState);
                currentState = historyRedo.back();
                historyRedo.pop_back();
                currentStateTestForChange = currentState;
            }
            cancelSearch = true;
            searchStage = 0;
        }

        // back
        ImGui::SameLine();
        std::vector<std::string> tempPath = extractPath(currentState.directoryPath);
        if (tempPath.size() <= 1)
        {
            ImGui::BeginDisabled();
            if (ImGui::ArrowButton("##directoryBack", ImGuiDir_Up))
            {
                if (tempPath.size() > 1)
                    tempPath.pop_back();
                currentState.directoryPath = combinePath(tempPath);
                cancelSearch = true;
                searchStage = 0;
            }
            ImGui::EndDisabled();
        }
        else if (ImGui::ArrowButton("##directoryBack", ImGuiDir_Up))
        {
            if (tempPath.size() > 1)
                tempPath.pop_back();
            currentState.directoryPath = combinePath(tempPath);
            cancelSearch = true;
            searchStage = 0;
        }
        ImGui::PopStyleVar();
        // path 
        float pathWidth = 400;
        ImGui::SameLine();
        if (ImGui::BeginChild("##path", { pathWidth, 20 }, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
        {
            if(*ImGuiWindow::Variable_bool("TextMode") == true)
            {
                // text mode
                ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0, 0, 0, 0 });
                ImGui::SetNextItemWidth(pathWidth);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 0));
                if (ImGui::InputText("##inputTextPath", ImGuiWindow::Variable_string("InputTextPath"), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    cancelSearch = true;
                    searchStage = 0;
                    auto path = std::filesystem::path(*ImGuiWindow::Variable_string("InputTextPath"));
                    if (std::filesystem::exists(path))
                    {
                        if (std::filesystem::directory_entry(path).is_directory())
                        {
                            currentState.directoryPath = *ImGuiWindow::Variable_string("InputTextPath");
                            currentState.selectedItemPath = "";
                        }
                        else
                        {
                            currentState.directoryPath = path.parent_path().string();
                            currentState.selectedItemPath = *ImGuiWindow::Variable_string("InputTextPath");
                        }
                    }
                }
                ImGui::PopStyleVar();
                if (*ImGuiWindow::Variable_bool("SetKeyboardFocusInputPath") == false)
                {
                    *ImGuiWindow::Variable_bool("SetKeyboardFocusInputPath") = true;
                    ImGui::SetKeyboardFocusHere(-1);
                }
                else if (!ImGui::IsItemActive())
                {
                    // check if path is correct
                    if (searchStage == 0)
                    {
                        auto path = std::filesystem::path(*ImGuiWindow::Variable_string("InputTextPath"));
                        if (std::filesystem::exists(path))
                        {
                            if (std::filesystem::directory_entry(path).is_directory())
                            {
                                currentState.directoryPath = *ImGuiWindow::Variable_string("InputTextPath");
                                currentState.selectedItemPath = "";
                            }
                            else
                            {
                                currentState.directoryPath = path.parent_path().string();
                                currentState.selectedItemPath = *ImGuiWindow::Variable_string("InputTextPath");
                            }
                        }
                    }
                    *ImGuiWindow::Variable_bool("TextMode") = false;
                }
                ImGui::PopStyleColor();
            }
            else if (searchStage == 1)
            {
                std::string points = " ...";
                std::string text = "Searching for results in ";
                text += "";// PATH
                text += points.substr(0, ((int)(ImGui::GetTime() / 0.25f) % 5));
                ImGui::Text(text.c_str());
                ImGui::SameLine(-1, 0);
                if (ImGui::InvisibleButton("##textMode", { pathWidth + 1, 20 }))
                {
                    *ImGuiWindow::Variable_bool("TextMode") = true;
                    *ImGuiWindow::Variable_string("InputTextPath") = currentState.directoryPath;
                    *ImGuiWindow::Variable_bool("SetKeyboardFocusInputPath") = false;
                }
            }
            else if (searchStage == 2)
            {
                std::string text = "Search results in ";
                text += ""; // PATH
                ImGui::Text(text.c_str());
                ImGui::SameLine(-1, 0);
                if (ImGui::InvisibleButton("##textMode", { pathWidth + 1, 20 }))
                {
                    *ImGuiWindow::Variable_bool("TextMode") = true;
                    *ImGuiWindow::Variable_string("InputTextPath") = currentState.directoryPath;
                    *ImGuiWindow::Variable_bool("SetKeyboardFocusInputPath") = false;
                }
            }
            else
            {
                // interactive mode
                std::vector<std::string> currentPathBlocks = extractPath(currentState.directoryPath);
                int startIndex = 0;
                float size = 0;
                for (int i = currentPathBlocks.size() - 1; i >= 0; i--)
                {
                    size += ImGui::CalcTextSize((currentPathBlocks[i] + " >").c_str()).x;
                    if (size > pathWidth - 100)
                    {
                        startIndex = i + 1;
                        if (i == currentPathBlocks.size() - 1)
                            startIndex = i;
                        break;
                    }
                }
                if (startIndex != 0)
                {
                    if (ImGui::Selectable("...>", false, 0, ImGui::CalcTextSize("...>")));
                    ImGui::SameLine();
                }
                for (int i = startIndex; i < currentPathBlocks.size(); i++)
                {
                    ImGui::PushID(i);
                    std::string selectableName = currentPathBlocks[i] + " >";
                    if (ImGui::CalcTextSize(selectableName.c_str()).x > pathWidth - 100)
                        selectableName.insert(selectableName.length() - 2, "...");
                    while (ImGui::CalcTextSize(selectableName.c_str()).x > pathWidth - 100)
                        selectableName.erase(selectableName.end() - 6);
                    if (ImGui::Selectable(selectableName.c_str(), false, 0, ImGui::CalcTextSize(selectableName.c_str())))
                    {
                        currentPathBlocks.erase(currentPathBlocks.begin() + i + 1, currentPathBlocks.end());
                        currentState.directoryPath = combinePath(currentPathBlocks);
                    }
                    if (i < currentPathBlocks.size() - 1)
                        ImGui::SameLine();
                    ImGui::PopID();
                }
                ImGui::SameLine(-1, 0);
                if (ImGui::InvisibleButton("##textMode", { pathWidth, 20 }))
                {
                    *ImGuiWindow::Variable_bool("TextMode") = true;
                    *ImGuiWindow::Variable_string("InputTextPath") = currentState.directoryPath;
                    *ImGuiWindow::Variable_bool("SetKeyboardFocusInputPath") = false;
                }
            }
            ImGui::EndChild();
        }

        // search
        float searchWidth = 100;
        ImGui::SameLine();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
        if (*ImGuiWindow::Variable_int("SearchType") == 0)
            *ImGuiWindow::Variable_string("ButtonText") = ":";
        if (*ImGuiWindow::Variable_int("SearchType") == 1)
            *ImGuiWindow::Variable_string("ButtonText") = "|";
        if (ImGui::Button(ImGuiWindow::Variable_string("ButtonText")->c_str()))
        {
            *ImGuiWindow::Variable_int("SearchType") += 1;
            if (*ImGuiWindow::Variable_int("SearchType") == 2)
                *ImGuiWindow::Variable_int("SearchType") = 0;
        }
        ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
        ImGui::SameLine();
        ImGui::PopStyleVar();
        if (ImGui::BeginChild("##search", { searchWidth, 20 }, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
        {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0, 0, 0, 0 });
            if (ImGuiWindow::Variable_string("SearchItemString")->empty())
            {
                ImGui::TextDisabled("search");
                ImGui::SameLine(-1, -1);
            }
            ImGui::SetNextItemWidth(searchWidth);

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 0));
            if (ImGui::InputText("##search", ImGuiWindow::Variable_string("SearchItemString"), ImGuiInputTextFlags_EnterReturnsTrue))
                search = true;
            ImGui::PopStyleVar();

            ImGui::PopStyleColor();
            ImGui::EndChild();
        }
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {4, 0});
        ImGui::SameLine();
        if (ImGui::Button("X"))
            cancelSearch = true;
        ImGui::PopStyleColor();
        ImGui::SameLine();
        if (searchStage == 1)
            ImGui::Text(" %c", "|/-\\"[(int)(ImGui::GetTime() / 0.05f) & 3]);
        else if (ImGui::ArrowButton("##rightSearch", ImGuiDir_Right))
            search = true;
        ImGui::PopStyleVar();

        // quick acces
        ImGui::Dummy({ 0, 6 });
        if (ImGui::BeginChild("##quickAcces", { 73, 300 }, true))
        {
            ImGui::EndChild();
        }

        // file system
        float foldersWidth = 565;
        ImGui::SameLine(0, 8);
        if (ImGui::BeginChild("##folders", { foldersWidth, 300 }, true))
        {
            if (ImGui::BeginTable("table", 4, ImGuiTableFlags_Resizable))
            {
                ImGui::TableSetupColumn("Name", 0, 2.0f);
                ImGui::TableSetupColumn("Type", 0, 0.64f);
                ImGui::TableSetupColumn("Size", 0, 0.5f);
                ImGui::TableSetupColumn("Date Modified", 0, 0.9f);
                ImGui::TableHeadersRow();
                // files
                int index = 0;
                if (searchStage == 0 && !currentState.directoryPath.empty())
                    for (auto const& it : std::filesystem::directory_iterator(currentState.directoryPath, std::filesystem::directory_options::skip_permission_denied))
                    {
                        // 0 == nothing | 1 == openFolder | 2 == loadFile
                        int returnVal = FileSystem(it, index);
                        if (returnVal == 1)
                        {
                            currentState.directoryPath = it.path().string();
                            break;
                        }
                        else if (returnVal == 2)
                        {
                            // load / save
                            currentState.directoryPath = it.path().parent_path().string();
                            currentState.selectedItemPath = it.path().string();
                            if (BrowserLoad(currentState.selectedItemPath, &output))
                            {
                                *ImGuiWindow::Variable_bool("Load") = false;
                                ImGui::CloseCurrentPopup();
                            }
                            if (BrowserSave(currentState.selectedItemPath, system))
                            {
                                *ImGuiWindow::Variable_bool("Save") = false;
                                ImGui::CloseCurrentPopup();
                            }
                            break;
                        }
                        index++;
                    }
                else if(searchStage == 1 || searchStage == 2)
                {
                    if (searchResults.empty() && searchStage == 2)
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::Text("No Results.");
                    }
                    else
                        for (auto const& it : searchResults)
                        {
                            // 0 == nothing | 1 == openFolder | 2 == loadFile
                            int returnVal = FileSystem(it, index);
                            if (returnVal == 1)
                            {
                                currentState.directoryPath = it.path().string();
                                cancelSearch = true;
                                searchStage = 0;
                                break;
                            }
                            else if (returnVal == 2)
                            {
                                // load / save
                                currentState.selectedItemPath = it.path().string();
                                if (BrowserLoad(currentState.selectedItemPath, &output))
                                {
                                    *ImGuiWindow::Variable_bool("Load") = false;
                                    ImGui::CloseCurrentPopup();
                                }
                                if (BrowserSave(currentState.selectedItemPath, system))
                                {
                                    *ImGuiWindow::Variable_bool("Save") = false;
                                    ImGui::CloseCurrentPopup();
                                }
                                break;
                            }
                            index++;
                        }
                }
                ImGui::EndTable();
            }
            ImGui::EndChild();
        }

        // File Name
        if (ImGui::BeginChild("##fileName", { 73, 20 }, false, ImGuiWindowFlags_AlwaysUseWindowPadding))
        {
            ImGui::Text("File Name:");
            ImGui::EndChild();
        }
        ImGui::SameLine(0, 8);
        if (ImGui::BeginChild("##Filename", { foldersWidth, 20 }, true))
        {
            ImGui::SetNextItemWidth(foldersWidth);
            ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0, 0, 0, 0 });
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 0));
            std::filesystem::path path(currentState.selectedItemPath);
            if (ImGui::InputText("##fileNameInput", ImGuiWindow::Variable_string("FileNameBrowser"), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                if (BrowserLoad(currentState.directoryPath + '/' + *ImGuiWindow::Variable_string("FileNameBrowser"), &output))
                {
                    *ImGuiWindow::Variable_bool("Load") = false;
                    ImGui::CloseCurrentPopup();
                }
                if (BrowserSave(currentState.directoryPath + '/' + *ImGuiWindow::Variable_string("FileNameBrowser"), system))
                {
                    *ImGuiWindow::Variable_bool("Save") = false;
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
            ImGui::EndChild();
        }

        // load buttons
        ImGui::Dummy({ 0, 0 });
        ImGui::SameLine(foldersWidth - 5);
        if (*ImGuiWindow::Variable_bool("Load"))
        {
            if (ImGui::Button("Load"))
            {
                if (BrowserLoad(currentState.directoryPath + '/' + *ImGuiWindow::Variable_string("FileNameBrowser"), &output))
                {
                    *ImGuiWindow::Variable_bool("Load") = false;
                    ImGui::CloseCurrentPopup();
                }
            }
        }
        else if (*ImGuiWindow::Variable_bool("Save"))
        {
            if (ImGui::Button("Save"))
            {
                if (BrowserSave(currentState.directoryPath + '/' + *ImGuiWindow::Variable_string("FileNameBrowser"), system))
                {
                    *ImGuiWindow::Variable_bool("Save") = false;
                    ImGui::CloseCurrentPopup();
                }
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            cancelSearch = true;
            *ImGuiWindow::Variable_bool("Load") = false;
            *ImGuiWindow::Variable_bool("Save") = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleVar();
        ImGui::EndPopup();
    }
    if ((searchStage == 0 || searchStage == 2) && search && !ImGuiWindow::Variable_string("SearchItemString")->empty())
    {
        searchStage = 1;
        std::thread(SaveManager::Search, currentState.directoryPath, *ImGuiWindow::Variable_string("SearchItemString")).detach();
        ImGuiWindow::Variable_string("SearchItemString")->clear();
    }
    if (cancelSearch)
    {
        searchStage = 0;
        ImGuiWindow::Variable_string("SearchItemString")->clear();
        searchResults.clear();
    }
    search = false;
    cancelSearch = false;

    if(!(currentState == currentStateTestForChange))
        historyUndo.push_back(currentStateTestForChange);
    if (historyUndo.size() > historySize)
        historyUndo.pop_front();
    if (historyRedo.size() > historySize)
        historyRedo.pop_front();
    return output;
}

SaveManager::historyEntry SaveManager::currentState = {GetCurrentDirectory(), ""};
std::deque<SaveManager::historyEntry> SaveManager::historyUndo;
std::deque<SaveManager::historyEntry> SaveManager::historyRedo;
std::vector<std::filesystem::directory_entry> SaveManager::searchResults;
int SaveManager::searchStage = 0;
std::string SaveManager::saveExtension = ".sunsystem";