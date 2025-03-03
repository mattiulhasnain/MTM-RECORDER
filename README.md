good # MTM Recorder 🎥🔥

A powerful Windows screen recorder with advanced features like DRM bypass, multi-monitor support, and high-quality recording.

## Features
- Capture entire screen, specific window, or region
- Multi-monitor support
- High-quality recording up to 4K at 60 FPS
- Audio recording options (system audio, microphone, or both)
- Hardware acceleration (NVENC, AMD VCE, Intel QSV)
- Real-time compression
- Multiple output formats (MP4, AVI, MKV, WebM, GIF)
- Hotkey support
- Mouse cursor options
- Annotation & drawing tools
- Scheduled recording
- Auto-split recordings
- Frame rate control
- Watermark support
- GIF export
- Instant replay mode
- Text overlay & timestamps
- System tray mode
- Auto-recording on app launch
- Post-recording editing
- Bypass DRM and protected window capture
- Stealth & privacy features
- Low CPU usage mode
- Adaptive bitrate
- Minimalistic and intuitive UI
- Customizable layouts
- Dark & light mode
- Floating controls
- Live preview
- Drag & drop files
- Quick access toolbar
- Custom themes

## Developer
FIVE FEBS

## Tech Stack
- C++ / Python (for low-level screen capture)
- OBS Studio API / FFmpeg (for encoding and recording)
- GDI+ / DirectX / Vulkan (for screen capture)
- Nvidia NVENC / AMD VCE / Intel QSV (for GPU-accelerated recording)
- Qt / Electron / WPF (for UI development)

## System Requirements
- Windows 10 or later
- Visual Studio 2022 or later
- Windows SDK
- CMake 3.10 or later

## Installation

### Prerequisites
1. Install Visual Studio 2022 with C++ development tools
2. Install Windows SDK
3. Install CMake (version 3.10 or higher)
4. Install FFmpeg libraries:
   - Download FFmpeg from https://ffmpeg.org/download.html
   - Extract to a directory (e.g., C:\ffmpeg)
   - Add FFmpeg bin directory to system PATH
   - Install pkg-config for Windows
5. Install NSIS for creating the installer

### Building from Source
1. Clone the repository:
   ```bash
   git clone https://github.com/mattiulhasnain/MTM-RECORDER.git
   cd MTM-RECORDER
   ```

2. Set up FFmpeg environment:
   ```bash
   set FFMPEG_DIR=C:\path\to\ffmpeg
   set PKG_CONFIG_PATH=%FFMPEG_DIR%\lib\pkgconfig
   ```

3. Create a build directory and configure the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

4. Build the project:
   ```bash
   cmake --build .
   ```

5. Create the installer:
   ```bash
   makensis ../installer.nsi
   ```

### Running the Application
1. After installation, launch MTM Recorder from the Start Menu
2. Configure recording settings:
   - Select capture area (full screen, window, or region)
   - Set frame rate (default: 30 FPS)
   - Choose output format (MP4, AVI, MKV)
3. Start recording using the Record button or hotkey
4. Stop recording when finished
5. Find your recordings in the specified output directory

### Running the Application
After installation, you can run the application from the Start Menu or by executing the installed executable.

### Uninstalling
To uninstall, use the uninstaller from the Start Menu or run:
```bash
/path/to/installation/Uninstall.exe
```

## Contributing
Contributions are welcome! Please read the [CONTRIBUTING.md](CONTRIBUTING.md) file for details.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
