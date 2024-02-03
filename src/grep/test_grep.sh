#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
"s tests/test_0_grep.txt VAR"
"for s21_grep.c s21_grep.h Makefile VAR"
"for s21_grep.c VAR"
"-e for -e ^int s21_grep.c s21_grep.h Makefile VAR"
"-e for -e ^int s21_grep.c VAR"
)

declare -a extra=(
"-n for tests/test_1_grep.txt tests/test_2_grep.txt"
"-n for tests/test_1_grep.txt"
"-n -e ^\} tests/test_1_grep.txt"
"-c -e /\ tests/test_1_grep.txt"
"-ce ^int tests/test_1_grep.txt tests/test_2_grep.txt"
"-e ^int tests/test_1_grep.txt"
"-nivh = tests/test_1_grep.txt tests/test_2_grep.txt"
"-e"
"-ie INT tests/test_5_grep.txt"
"-echar tests/test_1_grep.txt tests/test_2_grep.txt"
"-ne = -e out tests/test_5_grep.txt"
"-iv int tests/test_5_grep.txt"
"-in int tests/test_5_grep.txt"
"-c -l aboba tests/test_1_grep.txt tests/test_5_grep.txt"
"-v tests/test_1_grep.txt -e ank"
"-ne ) tests/test_5_grep.txt"
"-l for tests/test_1_grep.txt tests/test_2_grep.txt"
"-e = -e out tests/test_5_grep.txt"
"-ne ing -e as -e the -e not -e is tests/test_6_grep.txt"
"-e ing -e as -e the -e not -e is tests/test_6_grep.txt"
"-c -e . tests/test_1_grep.txt -e '.'"
"-l for no_file.txt tests/test_2_grep.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep $t > test_s21_grep.log
    grep $t > test_sys_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
      echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m grep $t"
    else
      (( FAIL++ ))
      echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m grep $t"
    fi
    rm test_s21_grep.log test_sys_grep.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in v c l n h
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 2 параметра
for var1 in v c l n h
do
    for var2 in v c l n h
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

# 3 параметра
for var1 in v c l n h
do
    for var2 in v c l n h
    do
        for var3 in v c l n h
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done

# 2 сдвоенных параметра
for var1 in v c l n h
do
    for var2 in v c l n h
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                testing $i
            done
        fi
    done
done

# 3 строенных параметра
for var1 in v c l n h
do
    for var2 in v c l n h
    do
        for var3 in v c l n h
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3"
                    testing $i
                done
            fi
        done
    done
done

echo "\033[31mFAIL: $FAIL\033[0m"
echo "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"
