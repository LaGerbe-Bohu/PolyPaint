#version 120

varying vec3 v_position;
varying vec2 v_uv;
uniform vec2 u_mousePos;
uniform mat4 u_projectionMatrix;
uniform vec2 u_points[500];
uniform int u_sizeOfPoints;
uniform int u_indexSelectedPoint;

uniform vec2 u_resolution;

uniform vec2 u_listofpixel[100];
uniform int u_sizePixel;

uniform vec2 u_fenetre[500];
uniform int u_sizeOfFenetre;
uniform sampler2D float_texture;

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

 //   vec2 position =  (u_mousePos.xy - .5*u_resolution.xy) / minres;

    vec3 src = vec3(0.0,0.0,0.0);
    float dist = 0.0;


    for(int i = 0; i <u_sizeOfPoints-1;i++)
    {
        vec2 pt = u_points[i].xy/resolution.x;
        vec2 pt2 = u_points[i+1].xy/resolution.x;
        src += drawLine(pt,pt2,uv,0.001) * vec3(1.0,1.0,1.0);
    }

    if(u_sizeOfPoints >= 3){
        src += drawLine(u_points[0]/resolution.x,u_points[u_sizeOfPoints-1]/resolution.x,uv,0.001) * vec3(1.0,1.0,1.0);
    }



    for(int i = 0; i <u_sizeOfFenetre-1;i++)
    {
        vec2 pt = u_fenetre[i].xy/resolution.x;
        vec2 pt2 = u_fenetre[i+1].xy/resolution.x;
         
        src += drawLine(pt,pt2,uv,0.001) * vec3(144.0/255.0,18.0/255.0,208.0/255.0);
    }

    if(u_sizeOfFenetre >= 3){
           
        src += drawLine(u_fenetre[0]/resolution.x,u_fenetre[u_sizeOfFenetre-1]/resolution.x,uv,0.001) * vec3(144.0/255.0,18.0/255.0,208.0/255.0);
    }



    vec3 PointF;
    vec3 PointP;

    src = lerp(lerp(src, vec3(144.0/255.0,18.0/255.0,208.0/255.0),PointF.x),vec3(0.0,1.0,128.0/255.0),PointP.x);

    for(int i = 0; i <u_sizeOfPoints;i++)
    {
        vec2 pt = u_points[i].xy/resolution.x;

        float v = (sqrt( pow(pt.x - uv.x,2)+ (pow( pt.y - uv.y ,2) )) ) ; 
        
        if(i == u_indexSelectedPoint){
            PointP = vec3(smoothstep(0.01-0.001,0.009-0.001,v));
            src += PointP*vec3(1,0,0);
          
        }
        else{
          PointP = vec3(smoothstep(0.0068-0.001,0.0055-0.001,v));
            src += PointP*vec3(0.0,1.0,128.0/255.0);
        }
        
    }


    for(int i = 0; i <u_sizeOfFenetre;i++)
    {
        vec2 f = u_fenetre[i].xy/resolution.x;
        
        float v =  (sqrt( pow(f.x - uv.x,2)+ (pow( f.y - uv.y ,2)) )) ; 
        PointF += smoothstep(0.0068 -0.001,0.0055-0.001,v);
    }


  



    src = lerp(lerp(src, vec3(144.0/255.0,18.0/255.0,208.0/255.0),PointF.x),vec3(0.0,1.0,128.0/255.0),PointP.x);
    
    
   

   //float v = (sqrt( pow(position.x - uv.x,2)+ (pow( position.y - uv.y ,2)) )) ; 
    //Point += smoothstep(0.0056,0.0055,v);

    float float_texel = float( texture2D(float_texture, vec2(texuv.x, 1-texuv.y)));

    src += float_texel * vec3(76/255.0, 81/255.0, 230/255.0);
    

    gl_FragColor = vec4(src,float((src.x+src.y+src.z)>0.0));

}