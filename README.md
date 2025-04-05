# C++ Real-time Collaborative Editor

🚀 A minimal real-time collaborative text editor built from scratch using C++, Boost.Beast WebSocket, and JSON.

## Features

- WebSocket-based real-time communication
- Lock-based editing: only one user can edit at a time
- Simple client-server model using Boost.Asio and Boost.Beast

## Build Instructions

### Dependencies

- Boost (Asio, Beast)
- CMake
- nlohmann-json (if used)

### Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
