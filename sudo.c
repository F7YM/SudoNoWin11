#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define MAX_BUF_SIZE 32767  // Windows最大命令行长度限制

// 安全拼接命令行参数
int build_command_args(char* dest, size_t dest_size, int start_idx, int argc, char* argv[]) {
    size_t remaining = dest_size - 1; // 保留null终止符空间
    dest[0] = '\0';

    for (int i = start_idx; i < argc && remaining > 0; i++) {
        size_t arg_len = strlen(argv[i]);
        
        // 添加空格（非第一个参数）
        if (i > start_idx) {
            if (remaining < 2) break; // 需要1字符+null终止符
            strncat(dest, " ", remaining);
            remaining -= 1;
        }

        // 添加参数
        if (arg_len >= remaining) {
            arg_len = remaining - 1; // 截断到剩余空间
            strncat(dest, argv[i], arg_len);
            remaining = 0;
        } else {
            strncat(dest, argv[i], remaining);
            remaining -= arg_len;
        }
    }
    return remaining > 0 ? 0 : -1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [option] <command>\n", argv[0]);
        return 1;
    }

    char command_args[MAX_BUF_SIZE] = "";

    // 公共执行函数
    int execute_shell(const char* verb, const char* file, const char* args_format, ...) {
        char final_args[MAX_BUF_SIZE];
        va_list args;
        va_start(args, args_format);
        vsnprintf(final_args, sizeof(final_args), args_format, args);
        va_end(args);

        SHELLEXECUTEINFO sei = {0};
        sei.cbSize = sizeof(sei);
        sei.lpVerb = verb;
        sei.lpFile = file;
        sei.lpParameters = final_args;
        sei.nShow = SW_SHOWNORMAL;

        return ShellExecuteEx(&sei) ? 0 : 1;
    }

    int result = 0;
    const char* option = argv[1];

    if (strcmp(option, "-t") == 0) {
        if (build_command_args(command_args, sizeof(command_args), 2, argc, argv) != 0) {
            fprintf(stderr, "Command too long!\n");
            return 1;
        }

        const char* nsudo = 
        #ifdef _WIN64
            "Nsudo64.exe";
        #else
            "Nsudo32.exe";
        #endif
        
        char command[MAX_BUF_SIZE];
        snprintf(command, sizeof(command), "%s -U:T cmd /C \"%s\"", nsudo, command_args);
        result = system(command);
    }
    else if (strcmp(option, "-p") == 0) {
        if (build_command_args(command_args, sizeof(command_args), 2, argc, argv) != 0) {
            fprintf(stderr, "Command too long!\n");
            return 1;
        }
        result = execute_shell("runas", "powershell.exe", "%s", command_args);
    }
    else if (strcmp(option, "-pt") == 0) {
        if (build_command_args(command_args, sizeof(command_args), 2, argc, argv) != 0) {
            fprintf(stderr, "Command too long!\n");
            return 1;
        }

        const char* nsudo = 
        #ifdef _WIN64
            "Nsudo64.exe";
        #else
            "Nsudo32.exe";
        #endif
        
        char command[MAX_BUF_SIZE];
        snprintf(command, sizeof(command), "%s -U:T powershell \"%s\"", nsudo, command_args);
        result = system(command);
    }
    else {
        if (build_command_args(command_args, sizeof(command_args), 1, argc, argv) != 0) {
            fprintf(stderr, "Command too long!\n");
            return 1;
        }
        char parameters[MAX_BUF_SIZE];
        snprintf(parameters, sizeof(parameters), "/C \"%s\"", command_args);
        result = execute_shell("runas", "cmd.exe", "%s", parameters);
    }

    if (result != 0) {
        fprintf(stderr, "Execution failed with error: %lu\n", GetLastError());
        return 1;
    }
    return 0;
}
