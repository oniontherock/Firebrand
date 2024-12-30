uniform sampler2D texture;
uniform float lerp;
uniform float divider;

void main() {
		
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	float avgColor = (pixel.r * 0.2126) + (pixel.g * 0.7152) + (pixel.b * 0.0722);

	vec4 grayColor = vec4(avgColor, avgColor, avgColor, pixel.a);

	vec4 colorDist = grayColor - pixel;

	gl_FragColor = (pixel + (colorDist * lerp)) / divider;
}

