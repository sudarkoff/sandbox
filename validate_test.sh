#!/usr/bin/env bash

gcc validate.c
if [ "$?" -ne "0" ]
then
    echo "Test terminated due to compilation error."
    exit $?
fi

PROG=./a.out

### First, test good dates
GOOD_DATES=( \
    "1973-03-31" "1583-1-1" "3999-12-31" \
    "1600-02-29" "2000-02-29" "2400-02-29" \
    )

element_count=${#GOOD_DATES[@]}
index=0
while [ "$index" -lt "$element_count" ]
do
    PARAM=`echo ${GOOD_DATES[$index]} | sed -e s#-#\ #g`
    $PROG $PARAM
    [ "$?" -eq "0" ] && echo -n "PASS " || echo -n "FAIL "
    echo -e "\t${GOOD_DATES[$index]} is valid"
    let "index+=1"
done

### Then, test bad dates
BAD_DATES=( \
    "1973-13-01" "1973-04-31" \
    "0-1-1" "1-0-1" "1-1-0" "1582-1-1" "4000-01-01" \
    "1700-02-29" "1800-02-29" "1900-02-29" "2100-02-29" \
    )

element_count=${#BAD_DATES[@]}
index=0
while [ "$index" -lt "$element_count" ]
do
    PARAM=`echo ${BAD_DATES[$index]} | sed -e s#-#\ #g`
    $PROG $PARAM
    [ "$?" -ne "0" ] && echo -n "PASS " || echo -n "FAIL "
    echo -e "\t${BAD_DATES[$index]} is NOT valid"
    let "index+=1"
done

### Finally, test comparing dates
COMPARE_DATES=( \
    "1973-03-31 1973-03-31|0" "1973-03-31 1973-04-30|255" "1973-03-31 1973-02-28|1" \
    "1600-02-29 2000-02-29|255" "2000-01-01 1999-12-31|1" "2004-12-31 2005-01-01|255" \
    )

element_count=${#COMPARE_DATES[@]}
index=0
while [ "$index" -lt "$element_count" ]
do
    PARAMS=`echo ${COMPARE_DATES[$index]%|*} | sed -e s#-#\ #g`
    RESULT=${COMPARE_DATES[$index]#*|}
    $PROG $PARAMS
    [ "$?" -eq "$RESULT" ] && echo -n "PASS " || echo -n "FAIL "
    echo -e "\t${COMPARE_DATES[$index]}"
    let "index+=1"
done
