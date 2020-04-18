void test_open_creat()  { 
    M_TRACE("--------- Begin test_open_creat() ---------\n");
    My_open("./test1",O_RDWR); // 一个存在的文件 
    My_open("./no_such_file1",O_RDWR); // 一个不存在的文件 
    My_open_with_mode("./test2",O_RDWR,S_IRUSR|S_IWUSR); // 一个存在的文件 
    My_open_with_mode("./no_such_file2",O_RDWR,S_IRUSR|S_IWUSR); // 一个不存在的文件
    My_open_with_mode("./test3",O_RDWR|O_CREAT,S_IRUSR|S_IWUSR); // 一个存在的文件 ，带 O_CREAT 标志 
    My_open_with_mode("./no_such_file3",O_RDWR|O_CREAT,S_IRUSR|S_IWUSR); // 一个不存在的文件 ，带 O_CREAT 标志 
    My_open_with_mode("./test4",O_RDWR|O_CREAT|O_EXCL,S_IRUSR|S_IWUSR); // 一个存在的文件 ，带 O_CREAT|O_EXCL 标志 
    My_open_with_mode("./no_such_file4",O_RDWR|O_CREAT|O_EXCL,S_IRUSR|S_IWUSR); // 一个不存在的文件，带 O_CREAT|O_EXCL 标志 
    My_creat("./test5",S_IRUSR|S_IWUSR); // 一个存在的文件 
    My_creat("./no_such_file5",S_IRUSR|S_IWUSR); // 一个不存在的文件
    M_TRACE("--------- End test_open_creat() ---------\n\n"); 
 } 