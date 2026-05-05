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

void IfTrueDirectoryFile() {
    if (fs::exists(extractedPath)) {
        std::cout << "Deleting old extracted folder..." << std::endl;
        std::string cmd = "rd /s /q \"" + extractedPath + "\"";
        std::system(cmd.c_str());
    }
}

void IfTrueGMADFile() {
    if (fs::exists(gmaPath)) {
        std::cout << "Deleting old GMA file..." << std::endl;

        std::string cmd = "del /f /q \"" + gmaPath + "\"";
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
#include <fstream>

void CreateAddonJson() {
    std::string jsonPath = extractedPath + "/addon.json";

    // Проверяем, существует ли файл, если нет — создаем
    if (!fs::exists(jsonPath)) {
        std::cout << "Creating missing addon.json..." << std::endl;
        std::ofstream file(jsonPath);
        file << "{\n"
            << "  \"title\": \"RedactedLANRPmusic\",\n"
            << "  \"type\": \"tool\",\n"
            << "  \"tags\": [\"fun\"],\n"
            << "  \"ignore\": [\"*.psd\", \"*.txt\"]\n"
            << "}";
        file.close();
    }
}
void CompilingGMADFile() {
    CreateAddonJson();
    IfTrueGMADFile();
    std::cout << "Compiling new GMAD file..." << std::endl;

    std::string cmd = "\"\"" + gmadExe + "\" create -folder \"" + extractedPath + "\" -out \"" + gmaPath + "\"\"";

    std::system(cmd.c_str());
}
void ReadMusicDirectory()
{
    std::cout << "Watch file..." << std::endl;

    std::string SeacrhDir = "C:/Program Files (x86)/Steam/steamapps/workshop\/content/4000/3310371040/extracted/sound/lanrp/music/";

    if (!std::filesystem::exists(SeacrhDir))
    {
        std::cout << "Error! Directory does not exist!" << std::endl;
        return;
    }
    for (auto& entry : std::filesystem::directory_iterator(SeacrhDir))
    {
        if (entry.is_directory())
        {
            std::cout << entry.path().filename().string() << ":" << std::endl;
            std::string DirectoryFileSearch = SeacrhDir + entry.path().filename().string() + "/";
            for (auto& entry_directiry : std::filesystem::directory_iterator(DirectoryFileSearch))
            {
                std::cout << "   " << entry_directiry.path().filename().string() << std::endl;
            }
        }
        if (entry.is_regular_file())
        {
            std::cout << entry.path().filename().string() << std::endl;
        }
    }
}
int main() {

    IfTrueDirectoryFile();

    OpenGMAD();

    ReadMusicDirectory();
    //CompilingGMADFile(); Временно не работает пока не разберусь с  models/gredwitch/bombs/500lbgp.sw.vtx


    Sleep(30000);
    return 0;
}