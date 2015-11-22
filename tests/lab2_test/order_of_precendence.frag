{
    a = 1 + 2 - 3 + 4 - 5;  /* left associative rule +,- equal precedence, should be ((((1+2)-3)+4)-5)*/
    b = 1 * 2 * 3 /4/5;     /* left associative rule, *,/ has equal precedence, should be ((((1*2)*3)/4)/5)*/
    c = 1 * 2 + 3;          /* * has precedence over +, should be (1*2)+3 */
    d = 1 + 2 * 3;          /* * has precedence over +, should be 1-(2*3)*/
    e = 1 - 2 / 3;          /* / has precedence over +, should be 1 - (2/3)*/
    f = 1 ^ 2 ^ 3;          /* right associative, should be 1 ^ (2^3) */
    g = 1 + 2 ^ 3;          /* ^ has precedence over +, should be 1+(2^3) */
    h = 1 * 2 ^ 3;          /* ^ has precedence over *, should be 1*(2^3) */
    i = !1 - 2;             /* ! is right associative and has precedence over -, should be (!1) -2 */
    j = 2 - !1;             /* ! is right associative and has precedence over -, should be 2- (!1) */
    k = 2 - -1;             /* negative is right associative and has precedence over -, should be 2 - (-1) */
    l = -1 -1;              /* negative is right associative and has precedence over -, should be (-1) - 1 */
    m = 2* -1;              /* negative is right associative and has precedence over * , should be 2* (-1)*/
    n = (1 + 2) * 3;        /* () captures expression and has highest precedence */
    o = 1+(2 + (3 + 4));    /* () captures expression and has highest precedence */
    p = (1 + 2) + (3+4);    /* () captures expression and has highest precedence */
    q = (1 + 2) * (3+4);    /* () captures expression and has highest precedence */
    r = 1 ^ (2 ^3);         /* () captures expression and has highest precedence */
    t = 1 ^ (2+3);          /* () captures expression and has highest precedence */
    u = 1* !(2+3);          /* () captures expression and has highest precedence */
    v = 1 < 2 && 3 <= 4;     /* comparision has higher precedence than &&, should be (1<2) && (3<=4) */
    w = 1>2 || 3>=4;        /* comparision has higher precedence than ||, should be (1>2) || (3>=4) */
    x = 1+ 2 >= 3+4;       /* addition has higher precedence than comparision, should be (1+2) >= (3+4) */
    y = 1 +2 || 1-1;      /* addition has higher precedence than ||, should be (1+2) || (1-1)*/
    z = a && b || c&&d;  /* &&  has higher precedence than ||, should be (a &&b) || (c&&d)*/

    /* tests for dangling else problem, the else statement should go with the innermost if*/
    /* both types of syntex should work with the grammer*/
    /* note, the parser output of the follow if statement should start in line 235 in order_of_precedence.frag.target
       or please trace through the entire the file to find the parser output of the following statement */
    if (a > b)
        if ( c>d)
            i = 0;
        else
            i = 1;
    else
        i = 2;

    if (a > b){
        if ( c>d){
            i = 0;
        }else{
            i = 1;
        }
    }else{
        i = 2;
    }

    if (a>b)
        if (c>d)
            i =0;
        else
            i =1;
    /* use brackets to force ELSE to associate with the outer IF, not the default inner IF*/    
    if (a>b){
        if (c>d)
            i = 0;
    }else{
        i = 0;
    }
}
