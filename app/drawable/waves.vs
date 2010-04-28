
// osg predefined unifoms:
//   int osg_FrameNumber; 
//   float osg_FrameTime; 
//   float osg_DeltaFrameTime;
//   mat4 osg_ViewMatrix;
//   mat4 osg_InverseViewMatrix; 



uniform float osg_FrameTime;
uniform vec2 gridSize;

/**
 * Uniforms are defined in VertexHeightUpdater.h.
 */
uniform sampler2D heightMap;

/**
 * Compute the height deformation offset from height map texture
 * at the given location in texture coordinates.
 * @param heightMapCoord location to sample height in texture coordinates.
 * @return height displacement amount.
 */
float sampleHeight(vec2 heightMapCoord) {
    // HELP: How do we get the signed floating point number out of the
    // texel cell?? A vec4 isn't what we want here.
	vec4 heightTexl = texture2D(heightMap, heightMapCoord);
    
    // Calling length gives us something to work with, but we loose
    // the sign. Not ideal
    // float height = length(heightTexl);
    // height = heightTexl.s + heightTexl.t + heightTexl.p + heightTexl.q
    

    float height = heightTexl.x;

    return height;
}

/**
 * Vertex program responsible for displacing verticies based on 
 * heightmap and creating an updated vertex normal for the fragment shader.
 */
varying vec3 normal, lightDir, eyeVec;
void main(void) {
    // First take care of passing on regular color info to fs
    gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
    
    // Get the desired height value at the current vertex.
    float height = sampleHeight(gl_TexCoord[0].st);
        
    // Exaggerate height a bit....
    height *= 15.0;
    
    // Add hight offset to vertex and provide
    vec4 newVertex = gl_Vertex;
    newVertex.z += height;
    gl_Position = gl_ModelViewProjectionMatrix * newVertex;
    
    // Compute lighting vectors for fs.
    vec4 origVertex = gl_ModelViewMatrix * gl_Vertex;
	lightDir = gl_LightSource[0].position.xyz - origVertex.xyz;
	eyeVec = -origVertex.xyz;
    
    // Now start process of computing new normal based on height values
    // from discrete samples around the current vertex. Basically approximates
    // local curvature.
    // offset is in texture coordinates
	const float offset = 0.005; 
    
	// HACK: Use matrix as a cheap array of vectors to set up
	// offsets around current texture coordinate. Current MacOS drivers
    // don't seem to allow vector array local variables.
    // Last (w) coordinate is ignored.
	mat4 neighbors = mat4(
        vec3(0, offset, 0), 0,
        vec3(0, -offset, 0), 0,
        vec3(-offset, 0, 0), 0,
        vec3(offset, 0, 0), 0
    );
    
    // Compute height for samples.
	for(int i = 0; i < 4; i++) {
		// Set the neighbors z component to the magnitude of the neighbor's texel.
        neighbors[i].z = sampleHeight(gl_TexCoord[0].st + neighbors[i].st);
	}
    
    // Recalculate normal by creating vectors from dz/dx and dz/dy
    // and then taking their cross product.
    vec3 dir1 = neighbors[0].xyz - neighbors[1].xyz;
    vec3 dir2 = neighbors[2].xyz - neighbors[3].xyz;
    vec3 newNorm = normalize(cross(dir1,dir2));
    
//    float d = dot(newNorm, gl_Normal);
//    newNorm = (d < 0.1) ? gl_Normal : newNorm;
    
    // Assign value to varying normal for fs
	normal = gl_NormalMatrix  * newNorm;
}
