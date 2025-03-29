# Quran Reader

## Description
Quran reader using Raylib and C99

## Installation

build with 
```zig
zig build
```
 and run it with 
```zig
./zig-out/bin/quranReader-linux 
```
or 
```zig
./zig-out/bin/quranReader-windows.exe
```

alternatively for linux just run 

```zig
zig build run
```


## Credits

- raylib library https://github.com/raysan5/raylib
- arabic and Quran translation from https://reader.quranite.com
- cJSON library https://github.com/DaveGamble/cJSON
- reading file JSON https://stackoverflow.com/questions/19111481/reading-and-storing-dynamically-to-char-array
- bcrypt https://crypto.stackexchange.com/questions/107946/is-there-a-counterpart-of-dev-random-and-dev-urandom-in-windows-nt-systems
- bcrypt https://learn.microsoft.com/en-us/windows/win32/api/bcrypt/nf-bcrypt-bcryptgenrandom
- randomize number in range using urandom https://www.geeksforgeeks.org/generating-random-number-range-c/
- download using libcurl https://stackoverflow.com/questions/1636333/download-file-using-libcurl-in-c-c

## Roadmap
- [x] making build.zig
- [x] test hello world raylib in linux 
- [x] test hello world raylib in windows
- [x] check if json file exist
- [ ] [HARD] if json doesnt exist, copy json from internet to empty file ,optional: maybe there is also need  to check if json already valid!, for now lets pretend its valid, linux done , windows left
- [x] searching json library for c
- [x] randomizer using urandom in linux and Bcrypt in windows 
- [ ] optional : change all [INFO] to raylib log
- [ ] optional : might want to use stdint.h for precise int
- [ ] read json file that contain arabic and translation
- [ ] [HARD] build raylib using zig make it fully static, this zig build is still so suck 
- [ ] time for first refactor
