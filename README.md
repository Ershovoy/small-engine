# A lightweight, high-performance custom game engine and a Blobby Volley 2 clone, written entirely in C from scratch.

# This project was built to demonstrate low-level systems programming, focusing on absolute control over memory, deterministic physics, and extreme execution speed by deliberately avoiding bloated commercial engines and even the standard C library.

## 🚀 Key Engineering Features

*   **Zero-Dependency / No-CRT Architecture:** Compiled strictly without the C Runtime Library (`/NODEFAULTLIB`). Interfaces directly with the Windows OS (Kernel32, User32, Gdi32) to ensure a minimal memory footprint and zero bloat.
*   **Live Hot-Reloading:** The platform layer and core game logic are strictly decoupled. The game logic compiles into a dynamic link library (`game.dll`) which the engine automatically hot-reloads at runtime when modifications are detected, dramatically reducing development iteration time.
*   **Deterministic Physics (Fixed-Point Math):** To guarantee perfect synchronization in multiplayer across different CPU architectures, a custom mathematics library was built using fixed-point arithmetic (`fixed_point.c`). All collision detection and physics integration avoid floating-point non-determinism.
*   **Client-Server Multiplayer:** Features a custom network engine built on raw UDP sockets. Implements a tick-based State-Sync architecture with input buffering, state hashing for integrity checks, and lag compensation.
*   **Custom Memory Management:** Completely avoids `malloc`/`free`. Implements linear allocators (Memory Arenas) and Generational Object Pools that directly reserve and commit memory pages via `VirtualAlloc`. This prevents memory fragmentation and leaks by design.
*   **Virtual File System (VFS):** Includes a custom tool (`embed_assets.exe`) that compiles all game assets (textures, sounds, compiled shaders) directly into the binary, resulting in a single, self-contained executable.
*   **Dual Rendering Pipeline:** Supports hardware-accelerated rendering via **DirectX 11** with custom HLSL shaders, and features a fully custom **Software Renderer** fallback written from scratch (manipulating raw pixel buffers).
*   **Low-Latency Audio:** Integrates with the **XAudio2** API for precise sound effect playback.

## 🛠️ Technical Stack

*   **Language:** C89 (Strict `/WX` warnings as errors, `/TC` C-only compilation).
*   **Graphics:** DirectX 11 / Software Rendering.
*   **Audio:** XAudio2.
*   **Networking:** WinSock2 (UDP).
*   **Build System:** Custom Windows Batch scripts (`.bat`).

## ⚙️ How to Build and Run

You need the MSVC compiler (`cl.exe`) available in your path (usually by running the Developer Command Prompt for Visual Studio).

1. Clone the repository:
   ```cmd
   git clone [https://github.com/Ershovoy/small-engine.git](https://github.com/Ershovoy/small-engine.git)
   cd small-engine
