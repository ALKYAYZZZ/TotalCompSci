#version 120

varying vec3 color;

void main()
{
	// Just pass the color through from vertex to pixel
	gl_FragColor = vec4(color.r, color.g, color.b, 1.0);
}
