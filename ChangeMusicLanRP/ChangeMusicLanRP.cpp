#include <iostream>
#include <filesystem> 
#include <cstdlib>
#include <string>
#include <windows.h>
#include <map>
#include <vector>

namespace fs = std::filesystem;

bool ExitProgram = true;
std::string BaseSteam = "C:/Program Files (x86)/Steam"; // Место положение Steam
std::string BaseGmod = "C:/Program Files (x86)/Steam/steamapps/common/GarrysMod"; // Место положение Gmod
const std::string baseDir = BaseSteam + "/steamapps/workshop/content/4000/3310371040";// Место положение lan rp content
const std::string extractedPath = baseDir + "/extracted";// Сохранение всех файлов из gma в папку
const std::string gmaPath = baseDir + "/gmpublisher.gma";
const std::string gmadExe = BaseGmod + "/bin/gmad.exe";
std::string BaseFolder = BaseGmod + "/garrysmod/addons/ChangerMusicLanRP";
std::string MusicFolder = BaseFolder + "/sound/lanrp/music";
std::map<std::string,std::vector<std::string>> list_music;
std::map<std::string, std::vector<std::string>> list_music_gma;

void ErrorWarning(std::string error)
{
    std::cout << "\033[31m" + error + "\033[0m" << std::endl;
    std::cerr << error << std::endl;
}
void SuccesfullyWarning(std::string successfully)
{
    std::cout << "\033[32m" + successfully + "\033[0m" << std::endl;
}
void IfTrueDirectoryFile() {
    if (fs::exists(extractedPath)) {
        std::cout << "Deleting trash folder..." << std::endl;
        std::string cmd = "rd /s /q \"" + extractedPath + "\"";
        std::system(cmd.c_str());
    }
}
void OpenGMAD() {
    IfTrueDirectoryFile();
    std::cout << "Extracting GMA..." << std::endl;

    std::string cmd = "\"\"" + gmadExe + "\" extract -file \"" + gmaPath + "\" -out \"" + extractedPath + "\"\"";
    std::system(cmd.c_str());

    if (fs::exists(extractedPath)) {
        SuccesfullyWarning("Extracted successfully!");
    }
    else {
        ErrorWarning("Error: Extraction failed!");
    }
}
void InitListMusic()
{
    std::cout << "Init list..." << std::endl;
    std::string TypeMusic;
    OpenGMAD();
    std::string SearchDir = baseDir + "/extracted/sound/lanrp/music/";
    for (const auto& entry : fs::directory_iterator(SearchDir)) {
        if (entry.is_directory()) {
            TypeMusic = "\033[1m" + entry.path().filename().string() + ":\033[0m";
            if (fs::is_empty(entry.path())) {
                continue;
            }
            for (const auto& file : fs::directory_iterator(entry.path())) {
                list_music[TypeMusic].push_back("\033[90m" + file.path().filename().string() + "\033[0m");
                list_music_gma[TypeMusic].push_back("\033[90m" + file.path().filename().string() + "\033[0m");
            }
        }
    }
    if (fs::exists(MusicFolder))
    {
        SearchDir = MusicFolder;
        for (const auto& entry : fs::directory_iterator(SearchDir)) {
            if (entry.is_directory()) {
                TypeMusic = "\033[1m" + entry.path().filename().string()  + ":\033[0m";
                if (fs::is_empty(entry.path())) {
                    continue;
                }
                for (const auto& file : fs::directory_iterator(entry.path())) {
                    list_music[TypeMusic].push_back("\033[32m" + file.path().filename().string() + "\033[0m");
                }
            }
        }
    }
    IfTrueDirectoryFile();
}
void InitNewList()
{
    list_music.clear();
    for (const auto& [section, songs] : list_music_gma) {
        for (const auto& song : songs) {
            list_music[section].push_back("\033[90m" + song + ":\033[0m");
        }
    }
    std::string TypeMusic;
    std::string SearchDir = MusicFolder;
    for (const auto& entry : fs::directory_iterator(SearchDir)) {
        if (entry.is_directory()) {
            TypeMusic = "\033[1m" + entry.path().filename().string() + ":\033[0m";
            if (fs::is_empty(entry.path())) {
                continue;
            }
            for (const auto& file : fs::directory_iterator(entry.path())) {
                list_music[TypeMusic].push_back("\033[32m" + file.path().filename().string() + "\033[0m");
            }
        }
    }
}
void ReadMusicDirectory()
{
    std::cout << "Read list..." << std::endl;
    for (const auto& [section, songs] : list_music) {
        std::cout << section << std::endl;
        for (const auto& song : songs) {
            std::cout << "   " << song << std::endl;
        }
    }
}
void CreateDirMusic()
{
    std::cout << "Create folder..." << std::endl;

    if (fs::create_directories(MusicFolder)) 
    {
        for (auto pozor : { "/calm","/epic","/other","/tense" }) {
            fs::create_directories(MusicFolder + pozor);
        }
        SuccesfullyWarning("Created folder Successfuly!");
    }
    else
    {
        std::cout << "\033[38;5;208mThe folder has already been created!\033[0m" << std::endl; 
    }
}
void RemoveAllFolder() 
{
    std::cout << "Cleaning music folders..." << std::endl;
    if (fs::exists(BaseFolder)) {
        for (const auto& entry : fs::directory_iterator(BaseFolder)) {
            if (fs::is_directory(entry.path())) {
                for (const auto& file : fs::directory_iterator(entry.path())) {
                    fs::remove_all(file.path());
                }
            }
        }
        SuccesfullyWarning("Remove successfuly!");
    }
    else 
    {
        ErrorWarning("Error! This no file!");
    }
}
void AddMusicInDirMusic(fs::path NameFolder, fs::path PathNameMusicFile)
{
    if (fs::exists(MusicFolder)) 
    {
        try {
            if (!fs::exists(PathNameMusicFile)) {
                ErrorWarning("Error: Source file does not exist!");
                return;
            }
            fs::path destination = MusicFolder / NameFolder / PathNameMusicFile.filename();

            fs::copy_file(PathNameMusicFile, destination, fs::copy_options::overwrite_existing);
            SuccesfullyWarning("Successfully added: " + PathNameMusicFile.filename().string() + " to " + NameFolder.string());
        }
        catch (const fs::filesystem_error& e) {
            ErrorWarning("Filesystem Error: " + std::string(e.what()));
        }
    }
    else
    {
        ErrorWarning("Error! You not create LanRP Folder");
        return;
    }
    InitNewList();
}
void ManualInstallationBase(std::string BaseGmodSet, std::string BaseSteamSet)
{
    BaseGmod = BaseGmodSet;
    SuccesfullyWarning("Path Gmod [" + BaseGmod + "]  installating");
    BaseSteam = BaseSteamSet;
    SuccesfullyWarning("Path Steam [" + BaseSteam + "]  installating");
}
void DeleteMusicInDirMusic(std::string NameMusicalFile)
{
    std::error_code ec;

    for (auto it = list_music.begin(); it != list_music.end(); ++it) {
        auto& vec = it->second;
        auto vec_it = std::find(vec.begin(), vec.end(), NameMusicalFile);

        if (vec_it != vec.end()) {
            vec.erase(vec_it);
            break;
        }
    }
    for (const auto& entry : fs::recursive_directory_iterator(MusicFolder))
    {
        if (fs::is_regular_file(entry) && entry.path().filename() == NameMusicalFile)
        {
            if (fs::remove(entry.path(), ec))
            {
                SuccesfullyWarning(NameMusicalFile + " Deleted");
            }
            else
            {
                ErrorWarning("Error! " + ec.message());
            }
        }
    }
    InitNewList();
}
void CoutStatusInterface()
{
    std::string StatusGmodPath = (BaseGmod.find("Steam/steamapps/common/GarrysMod") != std::string::npos && fs::exists(BaseGmod))
        ? "\033[32m" + BaseGmod + "\033[0m"
        : "\033[31m" + (BaseGmod.empty() ? "Not found" : BaseGmod) + "\033[0m";

    std::string StatusSteamPath = (BaseSteam.find("/Steam") != std::string::npos && fs::exists(BaseSteam))
        ? "\033[32m" + BaseSteam + "\033[0m"
        : "\033[31m" + (BaseSteam.empty() ? "Not found" : BaseSteam) + "\033[0m";
    std::string StatusFolderLanRP = fs::exists(MusicFolder)
        ? "\033[32mFolder exists!\033[0m"
        : "\033[31mFolder not found!\033[0m";
    std::cout << "--- ChangeMusicLanRP ---\n"
        << "1 - Create Folder LanRP Music - " << StatusFolderLanRP << "\n"
        << "2 - Add Music.\n"
        << "3 - Remove Music\n"
        << "4 - Remove Music Folder\n"
        << "5 - See what music is currently installed.\n"
        << "6 - Change Path to Gmod and Steam\n"
        << "0 - Exit program.\n"
        << "Total path Gmod and Steam:\n"
        << StatusGmodPath << "\n"
        << StatusSteamPath << std::endl;
}
void InterfaceConsole()
{
    InitListMusic();
    while (ExitProgram)
    {
        system("cls");
        CoutStatusInterface();
        int ChoiceInterface;
        std::string NameType, PathMusicFile,NameMusicFile;
        std::string PathGmod, PathSteam;
        std::cout << "Enter:";
        if (!(std::cin >> ChoiceInterface)) {
            std::cout << "Error: Please enter a number!" << std::endl;
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            Sleep(1000);
            continue;
        }
        if (ChoiceInterface > 6) 
        {
            std::cout << "Error: There is no such command" << std::endl;
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            Sleep(1000);
            continue;
        }
        switch (ChoiceInterface)
        {
        default :
            std::cout << "Error! There is no such command" << std::endl;
            Sleep(500);
            break;
        case 1:
            CreateDirMusic();
            Sleep(500);
            break;
        case 2: {

            std::cout << "Enter category (calm,epic,tense,other): ";
            std::cin >> NameType;
            std::cout << "Enter full Path to Music File: ";
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::getline(std::cin, PathMusicFile);
            PathMusicFile.erase(0, PathMusicFile.find_first_not_of(' '));
            PathMusicFile.erase(PathMusicFile.find_last_not_of(' ') + 1);
            if (!PathMusicFile.empty() && (PathMusicFile.front() == '"' || PathMusicFile.front() == '\'')) {
                PathMusicFile.erase(0, 1);
            }
            if (!PathMusicFile.empty() && (PathMusicFile.back() == '"' || PathMusicFile.back() == '\'')) {
                PathMusicFile.pop_back();
            }
            AddMusicInDirMusic(NameType, PathMusicFile);
            Sleep(1500);
            break;
        }
        case 3: {
            std::cout << "Enter music file name to delete: ";
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::getline(std::cin, NameMusicFile);
            NameMusicFile.erase(0, NameMusicFile.find_first_not_of(' '));
            NameMusicFile.erase(NameMusicFile.find_last_not_of(' ') + 1);
            DeleteMusicInDirMusic(NameMusicFile);
            Sleep(1200);
            break;
        }
        case 4:
            RemoveAllFolder();
            Sleep(1000);
            break;
        case 5:
            ReadMusicDirectory();
            std::cout << "Please press Enter to close!" << std::endl;
            std::cin.get();
            std::cin.get();//Костыль
            break;
        case 6:
            std::cout << "Enter path gmod:";
            std::cin >> PathGmod;
            std::cout << "Enter path steam:";
            std::cin >> PathSteam;
            ManualInstallationBase(PathGmod, PathSteam);
            Sleep(2000);
            break;
        case 0:
            ExitProgram = false;
            break;
        }
    }
    return;
}
int main() {
    InterfaceConsole();
    return 0;
}