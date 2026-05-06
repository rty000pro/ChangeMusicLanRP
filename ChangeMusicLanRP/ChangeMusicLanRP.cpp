#include <iostream>
#include <filesystem> 
#include <cstdlib>
#include <string>
#include <windows.h>

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
BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_CLOSE_EVENT) {
        ExitProgram = false;
        IfTrueDirectoryFile();
        Sleep(1000);
        return TRUE;
    }
    return FALSE;
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
void ReadMusicDirectory()
{
    std::cout << "Checking files..." << std::endl;
    std::string SearchDir;
    SearchDir = MusicFolder;
    if (!fs::exists(SearchDir)) {
        OpenGMAD();
        SearchDir = baseDir + "/extracted/sound/lanrp/music/";
    }
    try {
        for (const auto& entry : fs::directory_iterator(SearchDir)) {
            if (entry.is_directory()) {
                std::cout << entry.path().filename().string() << ":" << std::endl;
                if (fs::is_empty(entry.path())) {
                    std::cout << "   (empty)" << std::endl;
                    continue;
                }
                for (const auto& file : fs::directory_iterator(entry.path())) {
                    std::cout << "   " << file.path().filename().string() << std::endl;
                }
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        ErrorWarning("Scan error: " + std::string(e.what()));
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
    for (const auto& entry : fs::directory_iterator(MusicFolder))
    {
        if (entry.path().filename() == NameMusicalFile) {
            if (fs::remove(NameMusicalFile))
            {
                SuccesfullyWarning(NameMusicalFile + " Deleted");
            }
            else
            {
                ErrorWarning("Error! No file!");
            }
            
        }
    }
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
        << "0 - Create Folder LanRP Music - " << StatusFolderLanRP << "\n"
        << "1 - Add Music.\n"
        << "2 - Remove Music\n"
        << "3 - Remove Music Folder\n"
        << "4 - See what music is currently installed.\n"
        << "5 - Change Path to Gmod and Steam\n"
        << "6 - Exit program.\n"
        << "Total path Gmod and Steam:\n"
        << StatusGmodPath << "\n"
        << StatusSteamPath << std::endl;
}
void InterfaceConsole()
{
    
    while (ExitProgram)
    {
        system("cls");
        CoutStatusInterface();
        int ChoiceInterface;
        std::string NameType, PathMusicFile,NameMusicFile;
        std::string PathGmod, PathSteam;
        std::cout << "Enter:";
        std::cin >> ChoiceInterface;
        switch (ChoiceInterface)
        {
        case 0:
            CreateDirMusic();
            Sleep(500);
            break;
        case 1: {

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
        case 2:
            std::cout << "Please enter name Music File:";
            std::cin >> NameMusicFile;
            DeleteMusicInDirMusic(NameMusicFile);
            Sleep(1000);
            break;
        case 3:
            RemoveAllFolder();
            Sleep(1000);
            break;
        case 4:
            ReadMusicDirectory();
            std::cout << "Please press Enter to close!" << std::endl;
            std::cin.get();
            std::cin.get();//Костыль
            break;
        case 5:
            std::cout << "Enter path gmod:";
            std::cin >> PathGmod;
            std::cout << "Enter path steam:";
            std::cin >> PathSteam;
            ManualInstallationBase(PathGmod, PathSteam);
            Sleep(2000);
            break;
        case 6:
            ExitProgram = false;
            IfTrueDirectoryFile();
            break;
        default:
            std::cout << "Error! code:01" << std::endl;
            break;
        }
    }
    return;
}
int main() {
    InterfaceConsole();
    return 0;
}