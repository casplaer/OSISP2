#include <iostream>
#include <unistd.h>
#include <wait.h>

volatile sig_atomic_t flag = 0;

void signal_handler(int signum) {
    flag = 1;
}

int main() {
    signal(SIGTERM, signal_handler);
    signal(SIGKILL, signal_handler);
    signal(SIGINT, signal_handler);

    int counter = 0;
    while (counter < 20) {
        std::cout << "Counter: " << counter << std::endl;
        counter++;

        // fake operation
        for (int i = 0; i < 100000000; i++) {}

        if (flag) {
            pid_t child_pid = fork();

            if (child_pid == 0) {
                std::cout << "Child process created." << std::endl;
                return 0;
            } else if (child_pid > 0) {
                wait(NULL);
                std::cout << "Parent process finished." << std::endl;
            } else {
                std::cerr << "Error: Fork failed." << std::endl;
                exit(EXIT_FAILURE);
            }

            flag = 0;
        }

    }

    std::cout << "FINISHED" << std::endl;

    return 0;
}