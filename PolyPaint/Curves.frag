#version 120

varying vec3 v_position;
varying vec2 v_uv;
uniform vec2 u_mousePos;
uniform mat4 u_projectionMatrix;
uniform vec2 u_points[500];
uniform int u_sizeOfPoints;
uniform vec2 u_resolution;
uniform vec3 u_color;

float drawLine(vec2 p1, vec2 p2,vec2 vuv,float Thickness) {
    vec2 uv = vuv;

    float a = abs(distance(p1, uv));
    float b = abs(distance(p2, uv));
    float c = abs(distance(p1, p2));

    if ( a >= c || b >=  c ) return 0.0;

    float p = (a + b + c) * 0.5;

    // median to (p1, p2) vector
    float h = 2 / c * sqrt( p * ( p - a) * ( p - b) * ( p - c));

    return mix(1.0, 0.0, smoothstep(0.5 * Thickness, 1.5 * Thickness, h));
}

vec3 lerp(vec3 colorone, vec3 colortwo, float value)
{
	return (colorone + value*(colortwo-colorone));
}


void main(void){
    vec4 color = vec4(1.0,1.0,1.0,1.0);

  //
    mat4 mat = mat4(
    1.0,0.0,0.0,0.0,
    0.0,1.0,0.0,0.0,
    0.0,0.0,1.0,0.0,
    0.0,0.0,0.0,1.0
    );

    float minres = min(u_resolution.x, u_resolution.y);
    vec2 resolution = u_resolution;
    vec2 uv = gl_FragCoord.xy / resolution.x;
    vec2 position = u_mousePos.xy / resolution.x;
    vec2 texuv =  gl_FragCoord.xy / resolution.xy;


    vec3 src = vec3(0.0,0.0,0.0);


    for(int i = 0; i <u_sizeOfPoints-1;i++)
    {
        vec2 pt = u_points[i].xy/resolution.x;
        vec2 pt2 = u_points[i+1].xy/resolution.x;
        src += drawLine(pt,pt2,uv,0.001) * vec3(u_color[0]/255.0,u_color[1]/255.0,u_color[2]/255.0);
    }


    gl_FragColor = vec4(src,float((src.x+src.y+src.z)>0.0));

}