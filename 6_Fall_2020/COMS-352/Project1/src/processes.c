/**
 * This file prints the prompt, gets the user's input, and then runs it.
 * Foreground processes are run on a child process, and waited on by the main parent.
 * Background processes are run by a child processes that watches it in order to print exit statements.
 * @author ajharms
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#include "input.h"
#include "bools.h"
#include "processes.h"

/**
 * Make sure the type is the same for piping
 */
typedef int ProcStatus;

/**
 * Default mode to create new file in
 */
#define DEFAULT_OPEN_MODE 0755

/**
 * Prompt to print
 */
#define PROMPT "352> "

/**
 * Command to tell the shell to exit
 */
#define EXIT_COMMAND "exit"

/**
 * Counts the number of background processes ran and
 * used to print the output "[<#>] <pid>"
 */
static unsigned backgroundProcessCount = 1;

/**
 * Prints the final status when a background process exits
 * @param bkCount
 * Number of background status
 * @param in
 * input struct used to create the process
 * @param status
 * status to print
 */
void printFinalStatus(struct input* in, char* status) {
    printf("[%d] %s", backgroundProcessCount, status);
    for (int i = 0; i < in->argc; i++) printf(" %s", in->argv[i]);
    printf("\n");
}

/**
 * Starts a program based off the user's input. Uses the answer found at
 * https://stackoverflow.com/a/13710144 in order to tell if execvp succeeded.
 * @param in
 * input to launch program from
 * @return
 * pid of process created
 */
pid_t start(struct input* in) {
    int fd[2];
    pipe(fd);

    pid_t pid = fork();
    if (pid == 0) {
        close(fd[0]);
        fcntl(fd[1], F_SETFD, fcntl(fd[1], F_GETFD) | FD_CLOEXEC);

        ProcStatus status = -1;
        // Open the file if needed
        if (in->file != NULL) {
            int std;
            int filed = -3;
            // Redirect in
            if (in->op & IN_OP) {
                filed = open(in->file, O_RDONLY);
                std = STDIN_FILENO;
            // Redirect out
            } else if (in->op & OUT_OP) {
                filed = open(in->file, (O_WRONLY | O_CREAT | O_TRUNC), DEFAULT_OPEN_MODE);
                std = STDOUT_FILENO;
            }
            // Duplicate the file descriptor if the file opened
            if (filed == -1) {
                write(fd[1], &status, sizeof(ProcStatus));
                exit(status);
            }
            dup2(filed, std);
        }

        // Allow the input struct to be freed
        unsigned lnAll = 0;
        for (int i = 0; i < in->argc; i++) lnAll = strlen(in->argv[i]) + 1;

        char* args[in->argc + 1];
        char allArgs[lnAll];

        for (int i = 0; i < in->argc + 1; i++) args[i] = NULL;
        for (int i = 0, j = 0; i < in->argc; i++, j++) {
            char* arg = in->argv[i];
            args[i] = &allArgs[j];
            for (int k = 0; arg[k] != '\0'; k++, j++) {
                allArgs[j] = arg[k];
            }
            allArgs[j] = '\0';
        }
        freeInput(in);

        // Start the program
        execvp(args[0], args);

        // Program didn't start
        status = -2;
        write(fd[1], &status, sizeof(ProcStatus));
        close(fd[1]);
        exit(status);
    }

    // Check to make sure program launched
    close(fd[1]);
    ProcStatus status = 0;
    read(fd[0], &status, sizeof(ProcStatus));
    close(fd[0]);

    // Print any errors if necessary
    switch (status) {
        case -1:
            printf("file not found\n");
            break;
        case -2:
            printf("command not found\n");
            break;
        case -3:
            printf("shell352: Something went wrong determining file redirection\n");
            break;
        default: return pid;
    }
    return 0;
}

/**
 * Checks if a child pid is running, and if not
 * prints the proper exit status
 * @param in
 * input struct used to create the child process
 * @param parent
 * parent to the process that ran this method
 * @param child
 * child that we want to watch
 * @return
 * true if the child is still running, false if it has stopped
 */
boolean watchPID(struct input* in, pid_t parent, pid_t child) {
    // Get the status
    int status = 0;
    pid_t w = waitpid(child, &status, WNOHANG);
    // Make sure waitpid ran correctly
    if (w == child) {
        // Check to see if child exited
        if (WIFEXITED(status)) {
            status = WEXITSTATUS(status);
            // Everything went fine
            if (status == 0) {
                printFinalStatus(in, "Done");
                return false;
            // Program did not run fine
            } else {
                char s[20];
                sprintf(s, "Exit %d", status);
                printFinalStatus(in, s);
                return false;
            }
        // Check to see if child was stopped
        } else if (WIFSIGNALED(status)) {
            printFinalStatus(in, "Terminated");
            return false;
        }
    }
    // If the parent has been stopped (either via the exit
    // command or via kill) the parent ID will change to
    // whomever the OS hands it off to (e.g. systemd).
    // In that case, kill the child.
    if (parent != getppid()) {
        kill(child, SIGKILL);
        printFinalStatus(in, "Terminated");
        return false;
    }
    return true;
}

/**
 * Starts a process and runs it in the background
 * @param in
 * input from user
 */
void startBackground(struct input* in) {
    // Get the parent before the fork
    pid_t parent = getpid();
    int fd[2];
    pipe(fd);

    // This child process will watch the process we
    // actually want to run in order to tell when
    // we need to print out the finishing message
    pid_t child = fork();
    if (child == 0) {
        close(fd[0]);
        ProcStatus status = 0;
        int pid = start(in);

        // Failed to start
        if (pid == 0) {
            status = -1;
            write(fd[1], &status, sizeof(ProcStatus));
        // Watch the child
        } else {
            printf("[%d] %d\n", backgroundProcessCount, pid);
            write(fd[1], &status, sizeof(ProcStatus));
            while(watchPID(in, parent, pid));
        }

        freeInput(in);
        close(fd[1]);
        exit(0);
    }

    // Check to see if the child started,
    // and if so increment the background
    // process counter
    close(fd[1]);
    ProcStatus status = 0;
    read(fd[0], &status, sizeof(ProcStatus));
    if (status == 0) backgroundProcessCount++;
}

/**
 * Gets input from the user and runs it
 * @return
 * false if the exit command is typed, true otherwise
 */
boolean runShell() {
    // Print the prompt
    printf(PROMPT);
    fflush(stdout);

    // Get the input
    struct input* in = getInput();
    if (in == NULL) return true;
    else if (!strcmp(*in->argv, EXIT_COMMAND)) {
        freeInput(in);
        return false;
    }

    // Execute the input
    if (in->op & BACKGROUND_OP) {
        startBackground(in);
    } else {
        pid_t pid = start(in);
        if (pid != 0) waitpid(pid, NULL, 0);
    }
    freeInput(in);

    return true;
}