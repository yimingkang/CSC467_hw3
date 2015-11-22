{
        /*test declarations and variables using constants, and constructor*/
        int i_a = 1234;
        bvec2 b_vec2 = bvec2(true, false);
        float f_a =1.0;
        float f_b = 1.0;
        float f_c;
        float f_d;
        
        /* test argument list that has expressions*/
        vec4 f_vec4 = vec4(1.3 +1, 1.3*1, true&&true,dp3(vec4(1,2,3,4)));
        const bool b_a = true;
       
        /*test declaration without assignment*/ 
        float f_b;

        /* test statements with  expressions and operators*/
        float f_d = f_a * f_b;
        float f_d = (f_a - f_b)/f_a;
        float f_d = 2 - 4*f_b;

        /* test statements with if and while constructs*/
        if (f_a > 0){
            f_d = f_a * f_b + f_c / f_d;
        }else{
            f_d = 0;

}


