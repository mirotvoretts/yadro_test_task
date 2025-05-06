#!/bin/bash
set -euo pipefail
[ "$#" -ge 1 ] || (echo "Expected at least one argument: command to run" && exit 1)
COMMAND=("$@")
TIMEOUT=15s

FAIL=0
TEST_DIR="test-data"

function test_ok {
    local test_input="$1"
    local test_output="${test_input%.in}.out"
    local test_expected="${test_input%.in}.sol"

    timeout -k 0.1s "$TIMEOUT" "${COMMAND[@]}" "$test_input" > "$test_output"
    EXITCODE=${PIPESTATUS[0]}

    case "$EXITCODE" in
    0)
        if diff -q "$test_expected" "$test_output" >/dev/null; then
            echo "PASS"
        else
            echo -e "\033[31;1mWA\033[0m"
            FAIL=1
        fi
        ;;
    124)
        echo -e "\033[31;1mTL\033[0m (timeout or exit code $EXITCODE)"
        FAIL=1
        return 1
        ;;
    *)
        echo -e "\033[31;1mRTE\033[0m (exit code $EXITCODE)"
        FAIL=1
        ;;
    esac
    return 0
}

echo "===== Running tests ====="
for test_input in "$TEST_DIR"/*.in; do
    test_name=$(basename "$test_input")
    echo "===== $test_name ====="
    if ! test_ok "$test_input"; then
        break
    fi
done

if [[ "$FAIL" == "0" ]]; then
    echo -e "===== \033[32;1mALL PASS\033[0m ====="
else
    echo -e "===== \033[31;1mSOME FAIL\033[0m ====="
fi
exit $FAIL