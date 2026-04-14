#!/bin/bash

# =============================================================================
# Philosophers - Evaluation Tests
# Usage: ./test.sh [--leaks] [--races]
# =============================================================================

PHILO="./philo"
PASS=0
FAIL=0
TIMEOUT_CMD="timeout"

GREEN="\033[0;32m"
RED="\033[0;31m"
YELLOW="\033[0;33m"
RESET="\033[0m"

print_result()
{
	if [ "$1" -eq 0 ]; then
		echo -e "  ${GREEN}[OK]${RESET} $2"
		PASS=$((PASS + 1))
	else
		echo -e "  ${RED}[KO]${RESET} $2"
		FAIL=$((FAIL + 1))
	fi
}

# -- Compilation ---------------------------------------------------------------

echo "============================================"
echo " COMPILATION"
echo "============================================"
make re > /dev/null 2>&1
if [ $? -eq 0 ]; then
	print_result 0 "make re compiles without errors"
else
	print_result 1 "make re compiles without errors"
	exit 1
fi
echo ""

# -- Error handling ------------------------------------------------------------

echo "============================================"
echo " ERROR HANDLING"
echo "============================================"

OUT=$(./philo 2>&1)
[ $? -ne 0 ] && print_result 0 "no args -> error" || print_result 1 "no args -> error"

OUT=$(./philo 5 800 200 2>&1)
[ $? -ne 0 ] && print_result 0 "too few args -> error" || print_result 1 "too few args -> error"

OUT=$(./philo -1 800 200 200 2>&1)
[ $? -ne 0 ] && print_result 0 "negative value -> error" || print_result 1 "negative value -> error"

OUT=$(./philo 5 800 200 200 abc 2>&1)
[ $? -ne 0 ] && print_result 0 "non-numeric arg -> error" || print_result 1 "non-numeric arg -> error"

OUT=$(./philo 0 800 200 200 2>&1)
[ $? -ne 0 ] && print_result 0 "0 philosophers -> error" || print_result 1 "0 philosophers -> error"

echo ""

# -- Must die tests ------------------------------------------------------------

echo "============================================"
echo " MUST DIE"
echo "============================================"

# Test: 1 800 200 200 - lone philosopher dies
OUT=$($TIMEOUT_CMD 5 $PHILO 1 800 200 200 2>&1)
DIED=$(echo "$OUT" | grep "died" | head -1)
if [ -n "$DIED" ]; then
	TS=$(echo "$DIED" | awk '{print $1}')
	if [ "$TS" -le 810 ]; then
		print_result 0 "1 800 200 200 -> died at ${TS}ms (<=810)"
	else
		print_result 1 "1 800 200 200 -> died at ${TS}ms (>810, too late)"
	fi
else
	print_result 1 "1 800 200 200 -> no death detected"
fi

# Test: 4 310 200 100 - should die
OUT=$($TIMEOUT_CMD 5 $PHILO 4 310 200 100 2>&1)
DIED=$(echo "$OUT" | grep "died" | head -1)
if [ -n "$DIED" ]; then
	TS=$(echo "$DIED" | awk '{print $1}')
	if [ "$TS" -le 320 ]; then
		print_result 0 "4 310 200 100 -> died at ${TS}ms (<=320)"
	else
		print_result 1 "4 310 200 100 -> died at ${TS}ms (>320, too late)"
	fi
else
	print_result 1 "4 310 200 100 -> no death detected"
fi

echo ""

# -- Must survive tests --------------------------------------------------------

echo "============================================"
echo " MUST SURVIVE (8s each)"
echo "============================================"

survive_test()
{
	ARGS="$1"
	LABEL="$2"
	OUT=$($TIMEOUT_CMD 8 $PHILO $ARGS 2>&1)
	DIED=$(echo "$OUT" | grep "died")
	if [ -z "$DIED" ]; then
		print_result 0 "$LABEL -> no death"
	else
		print_result 1 "$LABEL -> $DIED"
	fi
}

survive_test "5 800 200 200" "5 800 200 200"
survive_test "4 410 200 200" "4 410 200 200"
survive_test "2 800 200 200" "2 800 200 200"

echo ""

# -- Must eat N times then stop ------------------------------------------------

echo "============================================"
echo " MUST EAT N TIMES"
echo "============================================"

OUT=$($TIMEOUT_CMD 30 $PHILO 5 800 200 200 7 2>&1)
DIED=$(echo "$OUT" | grep "died")
EAT_COUNT=$(echo "$OUT" | grep -c "is eating")
if [ -z "$DIED" ] && [ "$EAT_COUNT" -ge 35 ]; then
	print_result 0 "5 800 200 200 7 -> $EAT_COUNT eatings (>=35), no death, stopped"
else
	print_result 1 "5 800 200 200 7 -> eatings=$EAT_COUNT, died=$DIED"
fi

echo ""

# -- Death timing precision ----------------------------------------------------

echo "============================================"
echo " DEATH TIMING (<= 10ms delay)"
echo "============================================"

TIMING_OK=1
for i in 1 2 3; do
	OUT=$($TIMEOUT_CMD 3 $PHILO 4 310 200 100 2>&1)
	DIED=$(echo "$OUT" | grep "died" | head -1)
	TS=$(echo "$DIED" | awk '{print $1}')
	if [ -n "$TS" ] && [ "$TS" -le 320 ]; then
		echo -e "  run $i: died at ${TS}ms ${GREEN}OK${RESET}"
	else
		echo -e "  run $i: died at ${TS}ms ${RED}KO${RESET}"
		TIMING_OK=0
	fi
done
if [ "$TIMING_OK" -eq 1 ]; then
	print_result 0 "death timing consistently <= 10ms"
else
	print_result 1 "death timing exceeded 10ms"
fi

echo ""

# -- No scrambled output -------------------------------------------------------

echo "============================================"
echo " OUTPUT FORMAT"
echo "============================================"

OUT=$($TIMEOUT_CMD 5 $PHILO 5 800 200 200 7 2>&1)
BAD_LINES=$(echo "$OUT" | grep -cvE '^[0-9]+ [0-9]+ (has taken a fork|is eating|is sleeping|is thinking|died)$')
if [ "$BAD_LINES" -eq 0 ]; then
	print_result 0 "all lines match expected format"
else
	print_result 1 "$BAD_LINES lines with bad format"
fi

echo ""

# -- Optional: memory leaks ----------------------------------------------------

if [ "$1" = "--leaks" ] && command -v valgrind > /dev/null 2>&1; then
	echo "============================================"
	echo " MEMORY LEAKS (valgrind)"
	echo "============================================"
	VOUT=$(valgrind --leak-check=full --show-leak-kinds=all \
		$TIMEOUT_CMD 15 $PHILO 5 800 200 200 7 2>&1)
	if echo "$VOUT" | grep -q "no leaks are possible"; then
		print_result 0 "no memory leaks"
	else
		print_result 1 "memory leaks detected"
		echo "$VOUT" | grep -A5 "LEAK SUMMARY"
	fi
	echo ""
fi

# -- Optional: data races ------------------------------------------------------

if [ "$1" = "--races" ] && command -v valgrind > /dev/null 2>&1; then
	echo "============================================"
	echo " DATA RACES (helgrind)"
	echo "============================================"
	HOUT=$(valgrind --tool=helgrind \
		$TIMEOUT_CMD 15 $PHILO 5 800 200 200 7 2>&1)
	ERRORS=$(echo "$HOUT" | grep "ERROR SUMMARY" | awk '{print $4}')
	if [ "$ERRORS" = "0" ]; then
		print_result 0 "no data races"
	else
		print_result 1 "$ERRORS data race errors"
	fi
	echo ""
fi

# -- Summary -------------------------------------------------------------------

echo "============================================"
TOTAL=$((PASS + FAIL))
echo -e " RESULTS: ${GREEN}$PASS${RESET}/${TOTAL} passed, ${RED}$FAIL${RESET} failed"
echo "============================================"

exit $FAIL
