#version 120

attribute vec3 a_position;
attribute vec2 a_uv;
attribute vec2 u_mousePos;
attribute vec2 a_points;

uniform mat4 u_projectionMatrix;


attribute mat4 a_projectionMatrix;

varying vec2 v_uv;
varying vec3 v_position;
varying float float_texel;

uniform sampler2D float_texture;

vec2 mousePos;

void main(void){

    mat4 mat = mat4(
    1.0,0.0,0.0,0.0,
    0.0,1.0,0.0,0.0,
    0.0,0.0,1.0,0.0,
    0.0,0.0,0.0,1.0
    );

   vec4 position =  mat*vec4(a_position.x,a_position.y,0.0,1.0);
    v_position = vec3(position.x,position.y,position.z);
    v_uv = a_uv;

   //float_texel = float( texture2D(float_texture, vec2(position.x,position.y)));

 
    gl_Position = position;

}