#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

int main(int argc, char *argv[]){
	char argvStr[100000] = "";

	if (strcmp(argv[1], "-t") == 0){
		char command[100000];
		for (int i = 2; i < argc; i++){
			if (i > 2) {
            	strcat(argvStr, " ");
        	}
        	strcat(argvStr, argv[i]);
		}
    	#ifdef __x86_64__
        	snprintf(command, sizeof(command), "Nsudo64.exe -U:T %s", argvStr);
    	#elif __i386__    
        	snprintf(command, sizeof(command), "Nsudo32.exe -U:T %s", argvStr);
    	#endif
		system(command);
	}
	else{
		for (int i = 1; i < argc; i++){
        	if (i > 1) {
            	strcat(argvStr, " ");
        	}
        	strcat(argvStr, argv[i]);
		}
		SHELLEXECUTEINFO sei = {0};
    	sei.cbSize = sizeof(SHELLEXECUTEINFO);
    	sei.lpVerb = "runas";
    	sei.lpFile = "cmd.exe";
    	char parameters[100000];
    	snprintf(parameters, sizeof(parameters), "/C \"%s\"", argvStr);
    	sei.lpParameters = parameters;
    	sei.nShow = SW_SHOWNORMAL;
    	if (!ShellExecuteEx(&sei)) {
        	DWORD error = GetLastError();
        	fprintf(stderr, "ShellExecuteEx failed with error: %lu\n", error);
        	return 1;
    	}
	}

	return 0;
}
