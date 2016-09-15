attribute highp vec4 vertexCoord;
attribute highp vec4 vertexColor;

uniform highp mat4 matrix;
uniform mediump vec2 halfViewportSize;
uniform highp vec3 linearData;

varying mediump float gradTabIndex;

void main()
{
    gl_Position = matrix * vertexCoord;
    mediump vec2 viewportCoords = (gl_Position.xy + 1.0) * halfViewportSize;
    gradTabIndex = dot(linearData.xy, viewportCoords) * linearData.z;
}
