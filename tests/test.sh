#!/bin/bash

new_line=$'\n'

echo test1:
make run < test1.ifj24 > test1.code24
./ic24int test1.code24
echo $new_line

echo test2:
make run < test2.ifj24 > test2.code24
./ic24int test2.code24
echo $new_line

echo test3:
make run < test3.ifj24 > test3.code24
./ic24int test3.code24
echo $new_line

echo test4:
make run < test4.ifj24 > test4.code24
./ic24int test4.code24 < input4.txt
echo $new_line


echo test5:
make run < test5.ifj24 > test5.code24
./ic24int test5.code24 < input5.txt
echo $new_line


echo test6:
make run < test6.ifj24 > test6.code24
./ic24int test6.code24 
echo $new_line


echo test7:
make run < test7.ifj24 > test7.code24
./ic24int test7.code24 
echo $new_line


echo test8:
make run < test8.ifj24 > test8.code24
./ic24int test8.code24 < input8.txt
echo $new_line


echo test9:
make run < test9.ifj24 > test9.code24
./ic24int test9.code24 
echo $new_line


echo test10:
make run < test10.ifj24 > test10.code24
./ic24int test10.code24 < input10.txt
echo $new_line


echo test11:
make run < test11.ifj24 > test11.code24
./ic24int test11.code24 < input11.txt
echo $new_line


echo test12:
make run < test12.ifj24 > test12.code24
./ic24int test12.code24
echo $new_line


echo test13:
make run < test13.ifj24 > test13.code24
./ic24int test13.code24 < input13.txt
echo $new_line


echo test14:
make run < test14.ifj24 > test14.code24
./ic24int test14.code24 < input14.txt
echo $new_line


echo test15:
make run < test15.ifj24 > test15.code24
./ic24int test15.code24 < input15.txt
echo $new_line

#echo test16:
#make run < test16.ifj24 > test16.code24
#./ic24int test16.code24 
#echo $new_line

echo $new_line
echo test17:
make run < test17.ifj24 > test17.code24
./ic24int test17.code24 
echo $new_line


echo test18:
make run < test18.ifj24 > test18.code24
./ic24int test18.code24 
echo $new_line


echo test19:
make run < test19.ifj24 > test19.code24
./ic24int test19.code24 
echo $new_line

echo $new_line
echo test20:
make run < test20.ifj24 > test20.code24
./ic24int test20.code24 
echo $new_line




