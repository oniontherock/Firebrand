uniform sampler2D texture;
uniform float directions; // LOWER IS FASTER
uniform float quality; // LOWER IS FASTER
uniform float size;

void main() {

    vec2 resolution = vec2(textureSize(texture, 0));

    float TAU = 6.28318530718;

    vec2 radius = size / resolution;

    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = gl_FragCoord.xy / resolution;

    vec4 sum = vec4(0.0);

    // Blur calculations
    for(float d = 0.0; d < TAU; d += TAU / directions) {
		for(float i = 1.0 / quality; i <= 1.001; i += 1.0 / quality) {
			sum += texture2D(texture, uv+vec2(cos(d),sin(d))*radius*i);		
        }
    }
    
    // divide sum by the amount of pixels sampled (which is the quality * directions).
    sum /= quality * directions;

    gl_FragColor = vec4(1.0, 1.0, 1.0, sum.a);
}
