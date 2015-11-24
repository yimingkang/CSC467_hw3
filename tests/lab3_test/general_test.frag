{
        /*test declarations and variables using constants, and constructor*/
        int i_a = 1234;
        int b_vec2 = 1;
        float f_a =1.0;
        float f_b = 1.0;
        float f_c;
        float f_d;
        
       /*test argument list that has expressions*/
        vec4 f_vec4 = vec4(1.3 +1.0, 1.3*1.0,1.1,dp3(vec4(1.0,2.0,3.0,4.0), vec4(1.0,3.0,4.0,5.0)));
        const bool b_a = true;
       
        /*i_a = 0;*/

        /* test statements with  expressions and operators*/
      /*  f_d = f_a * f_b;
        f_d = (f_a - f_b)/f_a;
        f_d = 2.0 - 4.0*f_b;
*/
        /* test statements with if and while constructs*/
        if (f_a > 0.0){
            f_d = f_a * f_b + f_c / f_d;
        }else{
            f_d = 0.0;
        }
}


