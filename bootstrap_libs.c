#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
const char* args[] = {"cmake",
                      "..",
                      "-DBUILD_SHARED_LIBS=OFF",
                      "-DCMAKE_INSTALL_PREFIX=../install",
                      "-DFT_DISABLE_PNG=ON",
                      "-DFT_DISABLE_BZIP2=ON",
                      "-DFT_DISABLE_HARFBUZZ=ON",
                      "-DFT_DISABLE_BROTLI=ON",
                      "-DFT_DISABLE_ZLIB=ON",
                      NULL};

#ifdef _WIN32 // sorry but im using system in the win32 branch, subprocesses are horrible in windows
#include <direct.h>
const char* build_dir = "libs\\freetype\\build"; // why do they use backslashes, its so stupid
int main() {
    if (_mkdir(build_dir) != 0 && errno != EEXIST) {
        perror("mkdir failed");
        return 1;
    }
    if (_chdir(build_dir) != 0) {
        perror("chdir failed");
        return 1;
    }

    int ret = system("cmake .. -DBUILD_SHARED_LIBS=OFF "
                     "-DCMAKE_INSTALL_PREFIX=../install "
                     "-DFT_DISABLE_PNG=ON -DFT_DISABLE_BZIP2=ON "
                     "-DFT_DISABLE_HARFBUZZ=ON -DFT_DISABLE_BROTLI=ON "
                     "-DFT_DISABLE_ZLIB=ON");
    if (ret != 0)
        return ret;

    ret = system("cmake --build . --config release");
    if (ret != 0)
        return ret;
    ret = system("cmake --install . --config release");
    if (ret != 0)
        return ret;
}
#elif defined(__linux__) || defined(__APPLE__) // POSIX branch (I'm NOT doing BSD)
const char* build_dir = "libs/freetype/build";
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
int main() {
    int err = mkdirat(AT_FDCWD, "libs/freetype/build/", 0777);

    if (err == -1 && errno != EEXIST) {
        fprintf(stderr, "something went wrong trying to create build directory in freetype\n");
        perror("mkdir failed");
        exit(1);
    }

    err = chdir(build_dir);
    if (err == -1) {
        perror("changing directory did not work");
        return 1;
    }
    if (fork() == 0) { // newly spwned child process
        int err = execvp(args[0],
                         args); // using execve instead of system (using system is kind of unsafe)
        if (err == -1) {
            perror("cmake failed");
        }
    } else { // parent process
        wait(NULL);
        const char* cmd[] = {"make", NULL, NULL};
        if (fork() == 0) {
            if (execvp("make", cmd) == -1) {
                perror("make command got errors");
                return 1;
            }
        } else {
            wait(NULL);
            cmd[1] = "install";
            execvp("make", cmd);
        }
    }
    return 0;
}
#else
int main() {
    printf("Are you running TempleOS??\n");
    return 1;
}
#endif
