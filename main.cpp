#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <filesystem>

typedef struct Execute {
    int result;
    std::string directory;
} Execute;

//Shell Functions
[[noreturn]] void shell_loop();
std::string shell_read_line();
std::vector<std::string> shell_split_line(const std::string& line);
Execute shell_execute(const std::vector<std::string>& args, std::string current_location);

//Shell Commands
void shell_ls(const std::vector<std::string>& args, std::string current_location);
void shell_help();
void shell_exit();
void shell_pwd(const std::string& current_location);
std::string shell_cd(const std::vector<std::string>& args, std::string current_location);
void shell_clear();

//TODO: make far more efficient (I believe there's already a posix way of doing this)
void shell_clear() {
    for (int i = 0; i < 100; i++) {
        std::cout << std::endl;
    }
}

std::string shell_cd(const std::vector<std::string>& args, std::string current_location) {
    if (args.size() == 1) {
        std::cout << current_location << std::endl;
        return current_location;
    }

    else {
        current_location = args.at(1);
        return current_location;
    }
}

void shell_pwd(const std::string& current_location) {
    std::cout << current_location << std::endl;
}

void shell_ls(const std::vector<std::string>& args, std::string current_location) {
    std::string location = std::move(current_location);

    if (args.size() == 2) {
        location = args.at(1);
    }

    for (const auto & i: std::filesystem::directory_iterator(location)) {
        std::cout << i.path() << std::endl;
    }
}

void shell_help() {
    std::cout << "T-Shell Commands:\n\n";
    std::cout << "help: List all commands\n";
    std::cout << "ls: List files in current directory\n";
    std::cout << "exit: Leave shell\n";
    std::cout << "pwd: Print current directory\n";
    std::cout << "cd: Change directory\n";
    std::cout << "clear: Clear the screen\n";
}

void shell_exit() {
    exit(EXIT_SUCCESS);
}

//Shell Functions
std::string shell_read_line() {
    std::string line;

    std::getline(std::cin, line);

    return line;
}

std::vector<std::string> shell_split_line(const std::string& line) {
    std::vector<std::string> words;
    int start;
    int end = 0;

    while ((start = line.find_first_not_of(' ', end)) != std::string::npos) {
        end = line.find(' ', start);
        words.push_back(line.substr(start, end - start));
    }

    return words;
}

Execute shell_execute(const std::vector<std::string>& args, std::string current_location) {
    std::string command;

    if (args.empty()) {
        Execute result = {1, current_location};
        return result;
    }
    //TODO: turn all these things into enum then make it a switch case
    else if (args.at(0) == "exit") {
        shell_exit();
        Execute result = {-1, current_location};
        return result;
    }

    else if (args.at(0) == "help") {
        shell_help();
        Execute result = {1, current_location};
        return result;
    }

    else if (args.at(0) == "ls") {
        shell_ls(args, current_location);
        Execute result = {1, current_location};
        return result;
    }

    else if (args.at(0) == "pwd") {
        shell_pwd(current_location);
        Execute result = {1, current_location};
        return result;
    }

    else if (args.at(0) == "cd") {
        current_location = shell_cd(args, current_location);
        Execute result = {1, current_location};
        return result;
    }

    else if (args.at(0) == "clear") {
        shell_clear();
        Execute result = {1, current_location};
        return result;
    }

    else {
        for (const std::string& i : args) {
            command += i + " ";
        }

        system(command.c_str());

        Execute result = {1, current_location};
        return result;
    }
}

[[noreturn]] void shell_loop() {
    std::string line;
    std::vector<std::string> args;
    std::string current_location = "./";

    while (true) {
        std::cout << ">";
        line = shell_read_line();
        args = shell_split_line(line);
        Execute result = shell_execute(args, current_location);
        current_location = result.directory;
    }
}

int main(int argc, char **argv) {
    shell_loop();

    return EXIT_SUCCESS;
}