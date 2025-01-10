# ass2srt converter

Convert ASS subtitles to SRT format easily keeping readability as much as possible.

This tool is especially useful for players that have poor ASS subtitle support (e.g., built-in subtitle rendering on Samsung Tizen).
If you're encountering issues with ASS subtitles not displaying correctly, this converter will help you convert them to the more widely supported SRT format.

---

## Usage

The most common scenario is simply running the following command to convert an ASS subtitle file to SRT format:

```bash
ass2srt -i /path/to/input/file.ass -o /path/to/output/file.srt
```

### Additional options

There are a few extra options available that might be useful if the output is still too noisy:

* `-s style1,style2,style3`
Specify a list of ASS styles to include in the output file. All other styles will be ignored.
* `-e style1,style2,style3`
Specify a list of ASS styles to additionally **EXCLUDE** from the output file.
* `-x`
Attempts to detect "Signs"-like styles, which are often used for background signs translation and not for dialogues.
_Note: This functionality is still experimental and may not work perfectly for everyday use._

# Build and install

## Requirements

To build the converter from source, you will need:

* GCC (version 12.2 or higher, with support for the gnu++23 standard)
* CMake (version 3.25.0 or higher)

## Building and Installing from Source

1. Clone the repository:
```bash
git checkout https://github.com/kawashiro/ass2srt.git
```
2. Navigate into the project directory:
```bash
cd ass2srt/
```
3. Generate the build files using CMake:
```bash
cmake -B build/ -S .
```
You can specify a custom installation destination during this step, like so:
```bash
cmake -B build/ -S . -DCMAKE_INSTALL_PREFIX=/your/prefix
```
4. Build and install the tool:
```bash
cmake --build build/ --target install
```
5. And that's it! You can now run ass2srt from your shell to use the converter.

---

Enjoy converting your subtitles to a more readable format with ass2srt!

This tool was created for personal use, but you are free to contribute in any way you want.
