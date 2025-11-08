#include <SFML/Graphics.hpp>
#include <headers/gameScreen.h>
#include <headers/dataStruct.h>
#include <iostream>
#include <string>
#include <limits.h>
#include <mach-o/dyld.h>
#include <unistd.h>
#include <libgen.h>

void set_working_directory_to_executable_path() {
    char path[PATH_MAX];
    uint32_t size = PATH_MAX;

    // Get the path of the executable
    if (_NSGetExecutablePath(path, &size) == 0) {
        // 'dirname' modifies the path buffer in place to get the directory
        char* dir = dirname(path);
        
        // Change the current working directory
        if (chdir(dir) != 0) {
            std::cerr << "Error changing working directory to: " << dir << std::endl;
        } else {
            std::cout << "Working directory set to: " << dir << std::endl;
        }
    } else {
        std::cerr << "Error getting executable path." << std::endl;
    }
}


int main(int argc, char** argv) {
    // Call this function at the very beginning of main
    set_working_directory_to_executable_path();
    DataStruct DataTuple; 
    GameScreen GameLoop(DataTuple);

    return 0;
}
