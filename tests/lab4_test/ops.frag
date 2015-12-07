{
    int a = 0;
    int b = 10;
    int c = a + b + 10;

    bvec4 first  = {true, true, false, false};
    bvec4 second = {false, false, true, true}

    bvec4 and_result = first && second;
    bvec4 or_result  = first || second;

}
