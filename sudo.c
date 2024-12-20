#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

int main(int argc, char *argv[]){
	char argvStr[10000] = "";
	for (int i = 1; i < argc; i = i + 1){
        if (i > 1) {
            strcat(argvStr, " ");
        }
        strcat(argvStr, argv[i]);
	}

    SHELLEXECUTEINFO sei = {0};
    sei.cbSize = sizeof(SHELLEXECUTEINFO);
    sei.lpVerb = "runas";
    sei.lpFile = "cmd.exe";
    char parameters[10486];
    snprintf(parameters, sizeof(parameters), "/C \"%s\"", argvStr);
    sei.lpParameters = parameters;
    sei.nShow = SW_SHOWNORMAL;
    if (!ShellExecuteEx(&sei)) {
        DWORD error = GetLastError();
        fprintf(stderr, "ShellExecuteEx failed with error: %lu\n", error);
        return 1;
    }

	return 0;
}