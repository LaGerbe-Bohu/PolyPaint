#version 120

varying vec3 v_position;
varying vec2 v_uv;
uniform vec2 u_mousePos;
uniform mat4 u_projectionMatrix;
uniform vec2 u_points[500];
uniform int u_sizeOfPoints;

uniform vec2 u_fenetre[500];
uniform int u_sizeOfFenetre;


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


void main(void){
    vec4 color = vec4(1.0,1.0,1.0,1.0);

  //
    mat4 mat = mat4(
    1.0,0.0,0.0,0.0,
    0.0,1.0,0.0,0.0,
    0.0,0.0,1.0,0.0,
    0.0,0.0,0.0,1.0
    );



    vec4 position = mat*vec4(u_mousePos.x,u_mousePos.y,0.0,1.0)  ;

    vec3 src = vec3(0.1,0.1,0.1);
    float dist = 0.0;


    for(int i = 0; i <u_sizeOfPoints-1;i++)
    {
        src += drawLine(u_points[i],u_points[i+1],v_uv,0.001) * vec3(1.0,1.0,1.0);
    }

    if(u_sizeOfPoints > 0){
        src += drawLine(u_points[0],u_points[u_sizeOfPoints-1],v_uv,0.001) * vec3(1.0,1.0,1.0);
    }



    for(int i = 0; i <u_sizeOfFenetre-1;i++)
    {

        src += drawLine(u_fenetre[i],u_fenetre[i+1],v_uv,0.001) * vec3(0.0,0.0,1.0);
    }

    if(u_sizeOfFenetre > 0){
        src += drawLine(u_fenetre[0],u_fenetre[u_sizeOfFenetre-1],v_uv,0.001) * vec3(0.0,0.0,1.0);
    }



    vec3 Point;


    for(int i = 0; i <u_sizeOfPoints;i++)
    {
        dist += float(sqrt( pow(u_points[i].x - v_uv.x,2)+ (pow( u_points[i].y - v_uv.y ,2) * (9.0/16.0)) )<.003) ; // Pense à le faire avec le vrai projection matrix quand tu y arriveras
        Point += dist * vec3(1.0,1.0,1.0);
    }



    for(int i = 0; i <u_sizeOfFenetre;i++)
    {
        dist += float(sqrt( pow(u_fenetre[i].x - v_uv.x,2)+ (pow( u_fenetre[i].y - v_uv.y ,2) * (9.0/16.0)) )<.003); // Pense à le faire avec le vrai projection matrix quand tu y arriveras
        Point += dist * vec3(0.0,0.0,1.0);
    }




    src += Point;






    gl_FragColor = vec4(src,1.0);

}