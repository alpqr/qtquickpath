attribute vec4 vertexCoord;
attribute vec4 vertexColor;

uniform mat4 matrix;
uniform vec2 viewportSize;
uniform vec2 gradVec;

varying float gradTabIndex;

void main()
{
    gl_Position = matrix * vertexCoord;
    vec2 viewportCoords = (gl_Position.xy + 1.0) * viewportSize * vec2(0.5);
    gradTabIndex = dot(gradVec.xy, viewportCoords) / (gradVec.x * gradVec.x + gradVec.y * gradVec.y);
}
