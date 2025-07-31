# GameEngine DirectX12 AI Agent Instructions

## Project Architecture

This is a DirectX12-based game engine (`NimaFramework`) built as a static library with a component-based architecture using dependency injection.

### Core Components

- **Engine**: Static library (`Engine.vcxproj`) providing all framework functionality
- **SampleProject**: Executable that demonstrates engine usage
- **Framework Entry Point**: Use `CREATE_APPLICATION(YourClass)` macro in `main.cpp` to bootstrap your application class that inherits from `NimaFramework`

### Key Architectural Patterns

1. **Framework Pattern**: All applications inherit from `NimaFramework` and override `Initialize()`, `Update()`, `Draw()`, `Finalize()`
2. **Scene System**: Games use `SceneBase`-derived scenes managed by `SceneManager` with factory pattern (`ISceneFactory`)
3. **Component Systems**: Major features (Audio, Input, Sprite, Object3d, Particle) are initialized in `NimaFramework` and accessible globally
4. **Dependency Injection**: `DIContainer` manages service instances with type-based registration/resolution
5. **Object System**: Graphics objects inherit from `ObjectSystemBaseMT` for multi-threaded DirectX12 command list management

## Critical Developer Workflows

### Building
- **Debug**: Standard Visual Studio build (F5) with engine as static lib dependency
- **Debug_ASAN**: Special build configuration using `ASAN.props` for Address Sanitizer
- **Project Structure**: Engine builds to `Engine/Bin/Debug/Engine.lib`, projects link against it

### Project Setup
```cpp
// main.cpp - Standard pattern for all projects
#include <Framework/NimaFramework.h>
#include "YourProgram/YourProgram.h"

CREATE_APPLICATION(YourProgram)
```

### Scene Implementation
```cpp
// Scene pattern - all scenes follow this structure
class YourScene : public SceneBase {
public:
    YourScene(ISceneArgs* args) : SceneBase(args) {}
    void Initialize() override;
    void Update() override;
    void Draw() override;        // Graphics rendering
    void DrawTexts() override;   // Text rendering (separate pass)
    void Finalize() override;
};
```

## Project-Specific Conventions

### Include Patterns
- **Angle brackets**: For engine includes `<Framework/NimaFramework.h>`
- **Headers**: All engine headers use forward declarations and minimal includes
- **Directory structure**: Includes mirror physical folder structure from Engine root

### Resource Management
- **Models**: Use `ModelManager`, `ModelStorage`, and `IModelLoader` pattern
- **Textures**: Managed through `TextureManager` and `SRVManager`
- **Shaders**: Located in `EngineResources/Shaders/` with `.hlsl` extension

### Error Handling
- **HRESULT**: Use `HRESULT_ASSERT` macro for DirectX12 calls
- **Debug**: Comprehensive logging through `Logger` system
- **Memory**: AddressSanitizer support via Debug_ASAN configuration

### Multi-threading
- **Graphics Objects**: Use `ObjectSystemBaseMT` base class for thread-safe DirectX12 operations
- **Command Lists**: Each graphics system gets its own command list and allocator
- **Resource States**: Managed by `ResourceStateTracker` for proper synchronization

## Integration Points

### External Dependencies
- **ImGui**: Integrated via `ImGuiManager` for debug UI
- **Assimp**: Model loading through abstracted `IModelLoader` interface
- **DirectXTex**: Texture processing utilities
- **nlohmann/json**: Configuration and serialization
- **VectorMatrix**: Custom math library (prefer over DirectXMath)

### Key Manager Systems
- `ConfigManager`: JSON-based configuration
- `AudioManager`: Audio playback and management
- `Input`: Keyboard/mouse/gamepad input handling
- `SceneTransitionManager`: Scene switching with transitions
- `ParticleSystem`: Component-based particle effects

### Platform Specifics
- **Windows-only**: Uses Win32 API through `WinSystem`
- **DirectX12**: Primary graphics API with DirectWrite for text
- **x64**: 64-bit target platform only
- **Unicode**: All string handling expects Unicode

## Development Guidelines

- **Scene Args**: Use `ISceneArgs` to pass data between scenes, accessed via `pArgs_` member
- **Singletons**: Engine systems use singleton pattern (e.g., `SceneManager::GetInstance()`)
- **Resource Paths**: Use `PathResolver` for cross-platform path handling
- **Debug Tools**: Leverage `DebugManager`, `EventTimer`, and `Logger` for development
- **Naming**: Hungarian notation for member variables (prefix with underscore), camelCase for functions
