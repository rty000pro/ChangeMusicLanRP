#include <iostream>
#include <filesystem> 
#include <cstdlib>
#include <string>
#include <windows.h>

namespace fs = std::filesystem;


const std::string baseDir = "C:/Program Files (x86)/Steam/steamapps/workshop/content/4000/3310371040";
const std::string extractedPath = baseDir + "/extracted";
const std::string gmaPath = baseDir + "/gmpublisher.gma";
const std::string gmadExe = "C:/Program Files (x86)/Steam/steamapps/common/GarrysMod/bin/gmad.exe";
fs::path BaseFolder = "C:/Program Files (x86)/Steam/steamapps/common/GarrysMod/garrysmod/addons/ChangerMusicLanRP/sound/lanrp/music";
fs::path SearchFolderLANRPMUSIC = "C:/Program Files (x86)/Steam/steamapps/common/GarrysMod/garrysmod/addons/ChangerMusicLanRP";
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
    SearchDir = BaseFolder.string();
    if (!fs::exists(SearchDir)) {
        OpenGMAD();
        SearchDir = "C:/Program Files (x86)/Steam/steamapps/workshop/content/4000/3310371040/extracted/sound/lanrp/music/";
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

    if (fs::create_directories(BaseFolder)) 
    {
        for (auto pozor : { "calm","epic","other","tense" }) {
            fs::create_directories(BaseFolder / pozor);
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
    if (fs::exists(SearchFolderLANRPMUSIC)) {
        for (const auto& entry : fs::directory_iterator(SearchFolderLANRPMUSIC)) {
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
        fs::path destination = BaseFolder / NameFolder / PathNameMusicFile.filename();
        CreateDirMusic();

        fs::copy_file(PathNameMusicFile, destination, fs::copy_options::overwrite_existing);
        std::cout << "Successfully added: " << PathNameMusicFile.filename() << " to " << NameFolder << std::endl;
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem Error: " << e.what() << std::endl;
    }
}
void InterfaceConsole()
{
    bool ExitProgram = true;
    while (ExitProgram)
    {
        std::cout << "0 - Create Folder LanRP Music.\n1 - Add Music.\n2 - Remove Music Folder.\n3 - See what music is currently installed.\n4 - Exit programm." << std::endl;
        int ChoiceInterface;
        std::string NameType, PathMusicFile;
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
            std::cout << "Debug: Trying to open [" << PathMusicFile << "]" << std::endl;

            AddMusicInDirMusic(NameType, PathMusicFile);
            break;
        }
        case 2:
            RemoveAllFolder();
            break;
        case 3:
            ReadMusicDirectory();
            break;
        case 4:
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