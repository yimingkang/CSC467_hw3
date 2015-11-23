{
    int a = 1;
    float b = 1.0;
    ivec3 c = ivec3(1,2,3);
    ivec4 d = 1vec4(1,2,3,4);
    vec3 e = vec4(1.0,2.0,3.0);
    vec4 f = vec4(1.0,2.0,3.0,4.0);
    vec2 g = vec2(1.0,2.0);
    bool g = false;
    b = rsq(a) ;
    b = rsq(b);
    
    b = dp3(c,c);
    b = dp3(d,d);
    b = dp3(e,e);
    b = dp3(f,f);

    f = lit(f);
   
    b = rsq(h);
    b  = dp3(g);
    f = lit (g);



}
