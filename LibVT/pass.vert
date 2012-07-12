uniform mat4 matViewProjection;
uniform vec2 zrange;
attribute vec4 vertex;
attribute vec3 normal;
attribute vec2 texcoord0;
varying vec2 texcoord;
varying vec3 screennorm;
varying float zheight;
void main( void )
{
    gl_Position = matViewProjection * vertex;
    texcoord    = texcoord0.xy;
	screennorm =normal;;
	zheight=vertex.z;
}
