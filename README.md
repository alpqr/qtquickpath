Qt experiments for GL_NV_path_rendering, targeting both OpenGL 4.3+ and OpenGL
ES 3.1+. This is important since writing cross-GL-GLES code is non-trivial. For
instance, the standard NVPR examples using the fixed pipeline are a
non-starter, obviously.

We use a few functions from Direct State Access and more importantly, Separate
Shader Objects. The latter is essential to function with fragment shader-only
programs on OpenGL ES as well.

Verified on Windows (OpenGL 4.5) and the Drive CX platform (OpenGL ES 3.2).

Tested with Qt 5.8 (dev branch), but should function with Qt 5.6 and up (we use
QOpenGLExtraFunctions to get a proper, cross-platform & cross-API wrapper for
the ES 3.1-level functions).
