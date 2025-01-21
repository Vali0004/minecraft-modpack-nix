#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>
#define TAB "  "

namespace stdfs = std::filesystem;

int main(int argc, char** argv)
{
    if (argc <= 2)
    {
        std::cout << "Invalid args!" << std::endl << "Expected: modpack-nix [modpack_name] [path]" << std::endl;
        return 1;
    }
    std::string name{ argv[1] };
    stdfs::path path{ argv[2] };
    if (stdfs::exists(path))
    {
        bool found_usable_paths{ false };
        if (stdfs::is_empty(path))
        {
            std::cout << "No files found in path!" << std::endl;
        }
        std::vector<std::string> files{};
        if (stdfs::exists(path / "mods"))
        {
            found_usable_paths = true;
            for (auto& ent : stdfs::directory_iterator{ path / "mods" })
            {
                auto& p{ ent.path() };
                if (!ent.is_directory())
                {
                    files.push_back("mods/" + p.filename().string());
                }
            }
        }
        if (stdfs::exists(path / "resourcepacks"))
        {
            found_usable_paths = true;
            for (auto& ent : stdfs::directory_iterator{ path / "resourcepacks" })
            {
                auto& p{ ent.path() };
                if (!ent.is_directory())
                {
                    files.push_back("resourcepack/" + p.filename().string());
                }
            }
        }
        // Might not be a bad idea to reformat this shit
        std::stringstream output{};
        output << "{" << std::endl;
        for (auto& f : files)
        {
            output << TAB "\"" << f << "\"" << " = {" << std::endl;                   
            output << TAB TAB "src = ./" << name << "/" << f << ";" << std::endl;
            output << TAB "};" << std::endl;
        }
        output << "}";
        std::ofstream nixfile{ name + ".nix" };
        nixfile << output.str();
        nixfile.close();
        std::cout << output.str() << std::endl << std::endl;
        std::cout << "Found " << files.size() << " usable files" << std::endl;
        std::cout << "Output is in " << name << ".nix" << std::endl;
    }
    else
    {
        std::cout << "Not a valid path!" << std::endl;
    }
    return 0;
}