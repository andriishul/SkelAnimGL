# GPU Skeletal Animation

This project demonstrates skeletal animation where **bone matrix calculations are performed on the GPU** using an OpenGL compute shader and SSBO.  
The CPU selects the relevant keyframes each frame, and the GPU interpolates them and calculates final bone matrices.

The scene includes a freely movable camera so you can view the animated model from any angle.

---

### Features

- Skeletal animation with GPU compute shader
- Bone matrix calculations offloaded to the GPU
- Keyframe interpolation:
  - Linear interpolation (LERP) for position and scale
  - Spherical linear interpolation (SLERP) for rotation
- Hierarchical bone transformations
- Free camera movement

---

### Controls

| Key | Action |
|-----|------|
| W | Move forward |
| S | Move backward |
| A | Move left |
| D | Move right |
| Arrow Keys | Rotate camera |

---

### Libraries / Dependencies

- **OpenGL** graphics API  
- **GLFW** window creation and input handling  
- **GLAD** OpenGL function loader  
- **GLM** math library  
- **Assimp** model and animation loading  
- **stb_image** image loading (`stb_image.h`)  
- **fmt** formatting library  

---

### How It Works

1. The CPU uploads the static skeleton data once and updates the dynamic animation nodes each frame according to the current animation time, sending them to the GPU buffers.
2. Each compute shader invocation calculates the transformation of a single bone:
   - Interpolates position, rotation, and scale keyframes
   - Builds the local transformation matrix
   - Traverses the parent hierarchy to compute the global transformation
   - Applies the bone offset
3. The resulting **final bone matrices** are written to a buffer used by the vertex shader for skinning.

> Note: Each bone is computed in parallel on the GPU compute shader, while the CPU prepares and uploads animation data sequentially each frame.
---

### Purpose

The goal of this project is to demonstrate how skeletal animation can be **offloaded to the GPU** for efficient parallel computation of bone matrices, reducing CPU workload and enabling smoother animations for complex rigs.
