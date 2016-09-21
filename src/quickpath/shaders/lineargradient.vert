attribute vec4 vertexCoord;
attribute vec4 vertexColor;

uniform mat4 matrix;
uniform vec2 viewportSize;
uniform vec2 gradStart;
uniform vec2 gradEnd;

varying float gradTabIndex;

void main()
{
    vec4 pos = matrix * vertexCoord;
    vec2 v = pos.xy / pos.w;
    v.x = 1.0 + v.x * 0.5 * viewportSize.x;
    v.y = 1.0 + v.y * 0.5 * viewportSize.y;
    vec2 gradVec = gradEnd - gradStart;
    // project v onto gradVec to get a color table index
    gradTabIndex = dot(gradVec, v) / (gradVec.x * gradVec.x + gradVec.y * gradVec.y);
    gl_Position = pos;
}
