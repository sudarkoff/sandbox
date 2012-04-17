#define STRICT
#include <windows.h>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "Renames file that is currently in use.\n"
            "\nUSAGE:\n\tmvinuse <src> <dst>\n";
        return -1;
    }

    BOOL res = MoveFileEx(argv[1], argv[2], MOVEFILE_DELAY_UNTIL_REBOOT);
    if (!res) {
        DWORD err = GetLastError();
        LPVOID lpMsgBuf;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);
        std::cerr << "ERROR: " << lpMsgBuf << std::endl;
        LocalFree( lpMsgBuf );
        return err;
    }
    else {
        std::cout << "Now, reboot the computer.\n";
        return 0;
    }
}