# service-template

A modern C++ service template with WebSocket support, SQLite persistence, and structured logging. Provides a production-ready foundation for building real-time backend services.

## Quick Start

```bash
make deps      # Install dependencies via vcpkg
make build     # Build the project
make test      # Run tests
```

## Features

- WebSocket server via uWebSockets
- SQLite database integration
- Structured logging with spdlog
- JSON serialization with nlohmann-json
- Google Test and Google Benchmark integration

## Requirements

- CMake 3.25+
- C++17 compiler
- vcpkg package manager

## Project Structure

```
service-template/
├── include/        # Public headers
├── src/            # Implementation files
├── test/           # Unit tests and benchmarks
├── CMakeLists.txt  # Build configuration
└── vcpkg.json      # Dependency manifest
```
