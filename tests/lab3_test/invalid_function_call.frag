{
    int a = 1;
    float b = 1.0;
    ivec3 c = ivec3(1,2,3);
    ivec4 d = ivec4(1,2,3,4);
    vec4 e = vec4(1.0,2.0,3.0,4.0);
    vec4 f = vec4(1.0,2.0,3.0,4.0);
    vec3 g = vec3(1.0,2.0,3.0);
    vec2 h = vec2(1.0,1.0);
    
    a = dp3(c,c);  /*pass*/
    a = dp3(d,d);  /* pass*/
    b = dp3(e,e);  /*pass */
    b = dp3(f,f); /*pass*/
    b = dp3(c,c); /*fail int assign to float*/
    a = dp3(e,e); /*fail float assign to int*/

    f = lit(f); /*pass*/
    h = lit(h); /*fail no over load for vec2, assign vec4 to vec2*/
   
    b = rsq(b); /*pass*/
    b = rsq(a); /*pass*/
    a = rsq(a); /*fail assign float to int*/



}
