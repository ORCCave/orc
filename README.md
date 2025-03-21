# ORC

[![windows](https://github.com/ORCCave/ORC/actions/workflows/windows.yml/badge.svg)](https://github.com/ORCCave/ORC/actions/workflows/windows.yml)
[![linux](https://github.com/ORCCave/ORC/actions/workflows/linux.yml/badge.svg)](https://github.com/ORCCave/ORC/actions/workflows/linux.yml)

ORC(Object-Oriented Rendering Component)

### Prerequisites

To successfully build this project, ensure the following tools are installed on your system:

- **CMake** (version 3.25 or higher)
- **Git** (required for downloading external dependencies like SDL3)

#### Why Git is Required
This project uses CMake’s `FetchContent` module, which relies on Git to automatically download external dependencies (e.g., SDL3). If Git is not installed or not accessible in your system’s PATH, you’ll encounter `error: could not find git for clone of sdl3-populate`

#### For Linux Users: Install X11 and Xext
To compile ORC on Linux, you might need X11 and Xext development packages. Install them with:
  * Ubuntu/Debian: `sudo apt-get install libx11-dev libxext-dev`
  * Fedora: `sudo dnf install libX11-devel libXext-devel`
  * Arch: `sudo pacman -S libx11 libxext`
  * openSUSE: `sudo zypper install libX11-devel libXext-devel`

## License

MIT license
