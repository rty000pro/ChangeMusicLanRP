#include <iostream>
#include <filesystem> 
#include <cstdlib>
#include <string>
#include <windows.h>

namespace fs = std::filesystem;

std::string BaseSteam = "C:/Program Files (x86)/Steam";
std::string BaseGmod = "C:/Program Files (x86)/Steam/steamapps/common/GarrysMod";
const std::string baseDir = BaseSteam + "/steamapps/workshop/content/4000/3310371040";
const std::string extractedPath = baseDir + "/extracted";
const std::string gmaPath = baseDir + "/gmpublisher.gma";
const std::string gmadExe = BaseGmod + "/bin/gmad.exe";
fs::path BaseFolder = BaseGmod + "/garrysmod/addons/ChangerMusicLanRP";
fs::path MusicFolder = BaseFolder / "/sound/lanrp/music";

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
        std::cout << "Extracted successfully!" << std::endl;
    }
    else {
        std::cerr << "Error: Extraction failed!" << std::endl;
    }
}
void ReadMusicDirectory()
{
    std::cout << "Checking files..." << std::endl;
    std::string SearchDir;
    SearchDir = MusicFolder.string();
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
        std::cerr << "Scan error: " << e.what() << std::endl;
    }
}
void CreateDirMusic()
{
    std::cout << "Create folder..." << std::endl;

    if (fs::create_directories(MusicFolder)) 
    {
        for (auto pozor : { "calm","epic","other","tense" }) {
            fs::create_directories(MusicFolder / pozor);
        }
        std::cout << "Created folder Successfuly!" << std::endl;
        
    }
    else
    {
        std::cout << "The folder has already been created!" << std::endl; 
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
        std::cout << "Remove successfuly!" << std::endl;
    }
}
void AddMusicInDirMusic(fs::path NameFolder, fs::path PathNameMusicFile)
{
    try {
        if (!fs::exists(PathNameMusicFile)) {
            std::cerr << "Error: Source file does not exist!" << std::endl;
            return;
        }
        fs::path destination = MusicFolder / NameFolder / PathNameMusicFile.filename();
        CreateDirMusic();

        fs::copy_file(PathNameMusicFile, destination, fs::copy_options::overwrite_existing);
        std::cout << "Successfully added: " << PathNameMusicFile.filename() << " to " << NameFolder << std::endl;
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem Error: " << e.what() << std::endl;
    }
}
void ManualInstallationBase(std::string BaseGmodSet, std::string BaseSteamSet)
{
    BaseGmod = BaseGmodSet;
    std::cout << "Path Gmod [" + BaseGmod + "]  installating" << std::endl;
    BaseSteam = BaseSteamSet;
    std::cout << "Path Steam [" + BaseSteam + "]  installating" << std::endl;
}
void DeleteMusicInDirMusic(std::string NameMusicalFile)
{
    for (const auto& entry : fs::directory_iterator(MusicFolder))
    {
        if (entry.path().filename() == NameMusicalFile) {
            if (fs::remove(NameMusicalFile))
            {
                std::cout << NameMusicalFile << " Deleted" << std::endl;
            }
            else
            {
                std::cout << "Error! No file!" << std::endl;
            }
            
        }
    }
}
void InterfaceConsole()
{
    bool ExitProgram = true;
    while (ExitProgram)
    {
        std::cout << "0 - Create Folder LanRP Music.\n1 - Add Music.\n2 - Remove Music\n3 - Remove Music Folder\n4 - See what music is currently installed.\n5 - Change Path to Gmod and Steam\n6 - Exit programm.\nTotal path Gmod and Steam:\n" << BaseGmod << std::endl << BaseSteam << std::endl;
        int ChoiceInterface;
        std::string NameType, PathMusicFile,NameMusicFile;
        std::string PathGmod, PathSteam;
        std::cout << "Enter:";
        std::cin >> ChoiceInterface;
        switch (ChoiceInterface)
        {
        case 0:
            CreateDirMusic();
            break;
        case 1: {
            std::cout << "Enter category (calm, epic, etc.): ";
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
            break;
        }
        case 2:
            std::cout << "Please enter name Music File:";
            std::cin >> NameMusicFile;
            DeleteMusicInDirMusic(NameMusicFile);
            break;
        case 3:
            RemoveAllFolder();
            break;
        case 4:
            ReadMusicDirectory();
            break;
        case 5:
            std::cout << "Enter path gmod:";
            std::cin >> PathGmod;
            std::cout << "Enter path steam:";
            std::cin >> PathSteam;
            ManualInstallationBase(PathGmod, PathSteam);
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