{
        /*test declarations and variables using constants, and constructor*/
        int i_a = 1234;
        float f_a = -12.01;
        bool _ = true;
        ivec2 i_vec2 = ivec2(1,1);
        bvec2 b_vec2 = bvec2(true, false);
        
        /* test argument list that has expressions*/
        vec4 f_vec4 = vec4(1.3 +1, 1.3*1, true&&true,dp3(vec4(1,2,3,4)));
        const bool b_a = true;
       
        /*test declaration without assignment*/ 
        float f_b;

        /* test statements with  expressions and operators*/
        float f_d = f_a * f_b;
        float f_e = f_a / f_b;
        float f_f = f_a + f_b;
        float f_g = (f_a - f_b)/f_a;
        float f_h = 2 - 4*f_b;
        int i_a = i_a + 2;
        float f_i = f_a ^ i_a;
        bool _a = !_; 
         _a = _ &&_;
         _a = _||true;

        /* test statements with if and while constructs*/
        if (_d == true){
            f_d = f_a * f_b + f_c / f_d;
        }else{
            _d = !_d;
        }   

        while (_d != true){

        }   
        
        while (true)
            while(true)
                i = 0;

        /* test expressions with comparators*/
        i = f_a >0; 
        i = f_a <0; 
        i = f_a >= 0;
        i = f_a <= 0;
        i = f_a != 0;
        i = _ == true;
        
        /*test array assignment*/
        i = a[1];
    
        /*test function usage*/
        i = dp3(a,b);
        i = (dp3(a,b) + lit(a)) + lit(a) * dp3(a,b);
}


