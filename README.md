
This is the experimental Qt Quick Path item.

The end goal is to productize something similar to
https://github.com/qtproject/playground-scenegraph/tree/master/shapes/shapesplugin
but with support for stroking, more materials (gradients etc.), multiple
backends, and additional true declarative items for commonly used shapes.

Right now only the generic OpenGL backend is functional, using triangulating
stroke and fill from QOpenGLPaintEngine.

See https://twitter.com/alpqr/status/770271640294940672 for an earlier version
of the hellopathitem example in action.

*****

GL_NV_path_rendering is on the roadmap as well. The beginnings of the support
code are already in place, targeting both OpenGL 4.3+ and OpenGL ES 3.1+.  We
use a few functions from Direct State Access and more importantly, Separate
Shader Objects. The latter is essential to function with fragment shader-only
programs on OpenGL ES as well. Verified on Windows (OpenGL 4.5) and the Drive CX
platform (OpenGL ES 3.2). Integration with Qt Quick is not yet done.
