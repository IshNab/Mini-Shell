#!/bin/bash

# ============================================================================
# MINISHELL CROSS-PLATFORM TEST SUITE
# Compatible with: macOS and Linux (including GitHub Codespaces)
# Tests: Pipes, Redirections, Builtins, External Commands, Variables, Signals
# ============================================================================

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Detect OS
OS_TYPE="unknown"
if [[ "$OSTYPE" == "darwin"* ]]; then
    OS_TYPE="macOS"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS_TYPE="Linux"
fi

echo -e "${BLUE}Detected OS: $OS_TYPE${NC}\n"

# Platform-specific configurations
if [ "$OS_TYPE" == "macOS" ]; then
    # macOS uses BSD utilities
    TIMEOUT_CMD="gtimeout"  # Requires: brew install coreutils
    if ! command -v gtimeout &> /dev/null; then
        echo -e "${YELLOW}Warning: gtimeout not found. Using bash timeout workaround.${NC}"
        echo -e "${YELLOW}For better results, install: brew install coreutils${NC}\n"
        TIMEOUT_CMD="timeout_fallback"
    fi
else
    # Linux has timeout built-in
    TIMEOUT_CMD="timeout"
fi

# Fallback timeout function for macOS without coreutils
timeout_fallback() {
    local time=$1
    shift
    ( "$@" ) &
    local pid=$!
    ( sleep $time && kill -TERM $pid 2>/dev/null ) &
    local killer=$!
    wait $pid 2>/dev/null
    local result=$?
    kill -TERM $killer 2>/dev/null
    wait $killer 2>/dev/null
    return $result
}

# Counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Minishell executable
MINISHELL="./minishell"

# Temp files for comparison
MINISHELL_OUT="/tmp/minishell_out_$$.txt"
BASH_OUT="/tmp/bash_out_$$.txt"
MINISHELL_ERR="/tmp/minishell_err_$$.txt"
BASH_ERR="/tmp/bash_err_$$.txt"

# Platform-specific test file
TEST_INPUT="/tmp/test_input_$$.txt"

# ============================================================================
# HELPER FUNCTIONS
# ============================================================================

print_header() {
    echo -e "\n${BLUE}═══════════════════════════════════════════════════════════${NC}"
    echo -e "${BLUE}  $1${NC}"
    echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}\n"
}

print_test() {
    echo -e "${YELLOW}Testing:${NC} $1"
}

run_minishell() {
    local cmd="$1"

    # Create a temporary script file
    local script_file="/tmp/mini_script_$$.sh"
    echo "$cmd" > "$script_file"

    if [ "$TIMEOUT_CMD" == "timeout_fallback" ]; then
        timeout_fallback 2 $MINISHELL < "$script_file" 2>/dev/null | sed '/^minishell\$/d; /^exit$/d' > "$MINISHELL_OUT"
    else
        $TIMEOUT_CMD 2 $MINISHELL < "$script_file" 2>/dev/null | sed '/^minishell\$/d; /^exit$/d' > "$MINISHELL_OUT"
    fi

    rm -f "$script_file"
    return 0
}

run_bash() {
    local cmd="$1"
    if [ "$TIMEOUT_CMD" == "timeout_fallback" ]; then
        echo "$cmd" | timeout_fallback 2 bash --posix > "$BASH_OUT" 2> "$BASH_ERR"
    else
        echo "$cmd" | $TIMEOUT_CMD 2 bash --posix > "$BASH_OUT" 2> "$BASH_ERR"
    fi
    return $?
}

# Normalize output (remove platform-specific differences)
normalize_output() {
    local file="$1"
    # Remove trailing whitespace and empty lines at the end
    sed -i.bak 's/[[:space:]]*$//' "$file" 2>/dev/null || sed -i '' 's/[[:space:]]*$//' "$file" 2>/dev/null
    # Remove the backup file
    rm -f "${file}.bak"
}

compare_output() {
    local test_name="$1"
    local cmd="$2"

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    print_test "$test_name"
    echo -e "  Command: ${BLUE}$cmd${NC}"

    run_minishell "$cmd"
    local mini_exit=$?

    run_bash "$cmd"
    local bash_exit=$?

    # Normalize outputs for comparison
    normalize_output "$MINISHELL_OUT"
    normalize_output "$BASH_OUT"

    # Compare outputs
    if diff -q "$MINISHELL_OUT" "$BASH_OUT" > /dev/null 2>&1; then
        # For commands that should succeed, check exit status
        # For commands expected to fail, we're more lenient
        if [[ "$cmd" == *"nonexistent"* ]] || [[ "$cmd" == *"shadow"* ]] || [[ "$cmd" == *"UNDEFINED"* ]]; then
            # These are expected to fail, just check if both failed
            if [ $mini_exit -ne 0 ] && [ $bash_exit -ne 0 ]; then
                echo -e "  ${GREEN}✓ PASSED${NC} (both failed as expected)"
                PASSED_TESTS=$((PASSED_TESTS + 1))
            else
                echo -e "  ${RED}✗ FAILED${NC} (exit status mismatch: mini=$mini_exit, bash=$bash_exit)"
                FAILED_TESTS=$((FAILED_TESTS + 1))
            fi
        else
            # Normal success cases
            echo -e "  ${GREEN}✓ PASSED${NC}"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        fi
    else
        echo -e "  ${RED}✗ FAILED${NC} (output mismatch)"
        echo -e "  ${YELLOW}Expected (bash):${NC}"
        head -3 "$BASH_OUT" | sed 's/^/    /'
        echo -e "  ${YELLOW}Got (minishell):${NC}"
        head -3 "$MINISHELL_OUT" | sed 's/^/    /'
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
}

test_command() {
    compare_output "$1" "$2"
}

# ============================================================================
# CHECK PREREQUISITES
# ============================================================================

if [ ! -f "$MINISHELL" ]; then
    echo -e "${RED}Error: minishell executable not found!${NC}"
    echo "Please compile your minishell first with: make"
    exit 1
fi

echo -e "${GREEN}Starting Minishell Test Suite${NC}"
echo -e "Executable: $MINISHELL"
echo -e "Platform: $OS_TYPE\n"

# ============================================================================
# 1. BASIC COMMANDS
# ============================================================================

print_header "1. BASIC EXTERNAL COMMANDS"

test_command "Simple echo" "echo hello"
test_command "echo multiple args" "echo hello world"
test_command "pwd" "pwd"

# Platform-specific tests
if [ "$OS_TYPE" == "Linux" ]; then
    test_command "ls" "ls"
    test_command "ls with flag" "ls -l"
    test_command "cat /etc/hosts" "cat /etc/hosts"
else
    # macOS - use safer alternatives
    test_command "ls current dir" "ls -a"
    test_command "cat test file" "echo 'test' | cat"
fi

test_command "grep simple" "echo 'hello world' | grep hello"
test_command "wc word count" "echo 'hello world' | wc -w"
test_command "cat pipe" "echo 'test' | cat"

# ============================================================================
# 2. BUILTIN COMMANDS
# ============================================================================

print_header "2. BUILTIN COMMANDS"

test_command "echo no args" "echo"
test_command "echo -n flag" "echo -n hello"
test_command "echo -n with space" "echo -n hello world"
test_command "echo -nnnn multiple n" "echo -nnnn test"
test_command "echo -n -n multiple flags" "echo -n -n test"
test_command "echo with quotes" "echo 'hello world'"
test_command "echo with double quotes" 'echo "hello world"'
test_command "pwd builtin" "pwd"

# ENV test (platform-independent)
print_test "env builtin"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
echo "env" | $MINISHELL > "$MINISHELL_OUT" 2>&1
if [ -s "$MINISHELL_OUT" ]; then
    echo -e "  ${GREEN}✓ PASSED${NC}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "  ${RED}✗ FAILED${NC}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi

# CD tests
print_test "cd to /tmp"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
echo -e "cd /tmp\npwd" | $MINISHELL > "$MINISHELL_OUT" 2>&1
if grep -q "/tmp" "$MINISHELL_OUT"; then
    echo -e "  ${GREEN}✓ PASSED${NC}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "  ${RED}✗ FAILED${NC}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi

print_test "cd to home"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
echo -e "cd\npwd" | $MINISHELL > "$MINISHELL_OUT" 2>&1
if [ -s "$MINISHELL_OUT" ]; then
    echo -e "  ${GREEN}✓ PASSED${NC}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "  ${RED}✗ FAILED${NC}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi

# ============================================================================
# 3. PIPES
# ============================================================================

print_header "3. PIPE TESTS"

test_command "Simple pipe" "echo hello | cat"
test_command "Pipe with grep" "echo 'hello world' | grep hello"
test_command "Pipe with wc" "echo 'one two three' | wc -w"
test_command "Multiple pipes" "echo 'hello world' | cat | cat | cat"
test_command "echo pipe head" "echo -e 'line1\nline2\nline3' | head -1"
test_command "Complex pipe chain" "echo 'test' | cat | cat | grep test"

# Platform-safe pipe test
if [ "$OS_TYPE" == "Linux" ]; then
    test_command "Pipe with sort" "echo -e '3\n1\n2' | sort"
else
    test_command "Pipe with sort" "printf '3\n1\n2\n' | sort"
fi

test_command "Triple pipe" "echo hello | cat | cat | wc -c"

# ============================================================================
# 4. REDIRECTIONS
# ============================================================================

print_header "4. REDIRECTION TESTS"

# Create test files with unique names
echo "test content" > "$TEST_INPUT"

test_command "Output redirect" "echo hello > /tmp/mini_test_$$.txt && cat /tmp/mini_test_$$.txt"
test_command "Input redirect" "cat < $TEST_INPUT"
test_command "Append redirect" "echo line1 > /tmp/mini_append_$$.txt && echo line2 >> /tmp/mini_append_$$.txt && cat /tmp/mini_append_$$.txt"
test_command "Input and output" "cat < $TEST_INPUT > /tmp/mini_both_$$.txt && cat /tmp/mini_both_$$.txt"
test_command "Multiple redirects" "echo test > /tmp/m1_$$.txt > /tmp/m2_$$.txt && cat /tmp/m2_$$.txt"
test_command "Redirect with pipe" "echo hello | cat > /tmp/mini_pipe_redir_$$.txt && cat /tmp/mini_pipe_redir_$$.txt"

# Heredoc tests
print_test "Simple heredoc"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if [ "$TIMEOUT_CMD" == "timeout_fallback" ]; then
    echo -e "cat << EOF\nhello\nworld\nEOF" | timeout_fallback 2 $MINISHELL > "$MINISHELL_OUT" 2>&1
else
    echo -e "cat << EOF\nhello\nworld\nEOF" | $TIMEOUT_CMD 2 $MINISHELL > "$MINISHELL_OUT" 2>&1
fi
if grep -q "hello" "$MINISHELL_OUT" && grep -q "world" "$MINISHELL_OUT"; then
    echo -e "  ${GREEN}✓ PASSED${NC}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "  ${RED}✗ FAILED${NC}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi

# ============================================================================
# 5. VARIABLE EXPANSION
# ============================================================================

print_header "5. VARIABLE EXPANSION"

export TEST_VAR="test_value"
export USER_TEST="john_doe"

test_command "Simple variable" "echo \$USER"
test_command "Variable in string" "echo Hello \$USER"
test_command "Multiple variables" "echo \$USER \$HOME"
test_command "Exit status after success" "true && echo \$?"
test_command "Undefined variable" "echo \$UNDEFINED_VAR_XYZ_123"
test_command "Variable in quotes" 'echo "$USER"'
test_command "Variable not in single quotes" "echo '\$USER'"
test_command "Custom variable" "echo \$TEST_VAR"

# ============================================================================
# 6. QUOTES
# ============================================================================

print_header "6. QUOTE HANDLING"

test_command "Single quotes" "echo 'hello world'"
test_command "Double quotes" 'echo "hello world"'
test_command "Mixed quotes" "echo 'hello' \"world\""
test_command "Quote with var" 'echo "$USER"'
test_command "Single quote with var" "echo '\$USER'"
test_command "Empty quotes" 'echo ""'
test_command "Empty single quotes" "echo ''"
test_command "Quotes with spaces" 'echo "hello   world"'

# ============================================================================
# 7. LOGICAL OPERATORS
# ============================================================================

print_header "7. LOGICAL OPERATORS (AND/OR)"

test_command "AND success" "true && echo success"
test_command "AND failure" "false && echo should_not_print"
test_command "OR success" "true || echo should_not_print"
test_command "OR failure" "false || echo failure"
test_command "Multiple AND" "true && true && echo success"
test_command "Multiple OR" "false || false || echo final"
test_command "Command AND" "echo first && echo second"
test_command "Command OR" "false || echo fallback"

# ============================================================================
# 8. EDGE CASES
# ============================================================================

print_header "8. EDGE CASES"

test_command "Empty command" ""
test_command "Only spaces" "   "
test_command "Multiple spaces" "echo     hello     world"
test_command "Command not found" "nonexistentcommand12345xyz"
test_command "Very long pipe chain" "echo test | cat | cat | cat | cat | cat"
test_command "Redirect to /dev/null" "echo hidden > /dev/null && echo visible"

# ============================================================================
# 9. BUILTIN SPECIFIC TESTS
# ============================================================================

print_header "9. BUILTIN SPECIFIC TESTS"

# Export tests
print_test "export new variable"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
echo -e "export MINI_TEST=hello\necho \$MINI_TEST" | $MINISHELL > "$MINISHELL_OUT" 2>&1
if grep -q "hello" "$MINISHELL_OUT"; then
    echo -e "  ${GREEN}✓ PASSED${NC}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "  ${RED}✗ FAILED${NC}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi

# Unset tests
print_test "unset variable"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
echo -e "export MINI_UNSET=test\nunset MINI_UNSET\necho \$MINI_UNSET" | $MINISHELL > "$MINISHELL_OUT" 2>&1
if ! grep -q "test" "$MINISHELL_OUT"; then
    echo -e "  ${GREEN}✓ PASSED${NC}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "  ${RED}✗ FAILED${NC}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi

# ============================================================================
# 10. SIGNAL HANDLING
# ============================================================================

print_header "10. SIGNAL HANDLING (Manual Tests)"

echo -e "${YELLOW}The following tests require manual verification:${NC}"
echo ""
echo "1. Start minishell and press Ctrl+C"
echo "   → Should display new prompt without exiting"
echo ""
echo "2. Start minishell and press Ctrl+\\"
echo "   → Should do nothing (ignore SIGQUIT)"
echo ""
echo "3. Start minishell and press Ctrl+D"
echo "   → Should exit gracefully"
echo ""
echo "4. Run: cat (then press Ctrl+C)"
echo "   → Should interrupt cat and return to prompt"
echo ""

# ============================================================================
# 11. MEMORY LEAK TESTS
# ============================================================================

print_header "11. MEMORY LEAK TESTS"

if ! command -v valgrind &> /dev/null; then
    echo -e "${YELLOW}Valgrind not found. Skipping memory leak tests.${NC}"
    if [ "$OS_TYPE" == "macOS" ]; then
        echo -e "${YELLOW}Note: Valgrind has limited support on recent macOS versions.${NC}"
        echo -e "${YELLOW}Consider using 'leaks' command on macOS:${NC}"
        echo -e "  MallocStackLogging=1 ./minishell"
        echo -e "  Then in another terminal: leaks minishell${NC}"
    fi
else
    echo -e "${YELLOW}Running valgrind tests (this may take a while)...${NC}\n"

    print_test "Valgrind: Simple command"
    echo "echo hello" | valgrind --leak-check=full --error-exitcode=1 --quiet $MINISHELL > /dev/null 2> /tmp/valgrind_$$.log
    if [ $? -eq 0 ]; then
        echo -e "  ${GREEN}✓ NO LEAKS${NC}"
    else
        echo -e "  ${RED}✗ LEAKS DETECTED${NC}"
        echo "  See /tmp/valgrind_$$.log for details"
    fi

    print_test "Valgrind: Pipes"
    echo "echo hello | cat | cat" | valgrind --leak-check=full --error-exitcode=1 --quiet $MINISHELL > /dev/null 2> /tmp/valgrind2_$$.log
    if [ $? -eq 0 ]; then
        echo -e "  ${GREEN}✓ NO LEAKS${NC}"
    else
        echo -e "  ${RED}✗ LEAKS DETECTED${NC}"
        echo "  See /tmp/valgrind2_$$.log for details"
    fi
fi

# ============================================================================
# PLATFORM-SPECIFIC NOTES
# ============================================================================

print_header "PLATFORM-SPECIFIC NOTES"

if [ "$OS_TYPE" == "macOS" ]; then
    echo -e "${YELLOW}macOS Detected:${NC}"
    echo "  • Some system calls behave differently on macOS vs Linux"
    echo "  • Test your minishell on the school's Linux machines before submitting"
    echo "  • readline behavior may differ between platforms"
    echo ""
    echo -e "${YELLOW}macOS-specific commands:${NC}"
    echo "  • For memory leaks: use 'leaks' instead of valgrind"
    echo "  • Command: MallocStackLogging=1 ./minishell"
    echo "  • Then: leaks minishell (in another terminal)"
else
    echo -e "${GREEN}Linux Detected:${NC}"
    echo "  • This matches the 42 school evaluation environment"
    echo "  • Results should be consistent with moulinette"
fi

# ============================================================================
# CLEANUP
# ============================================================================

rm -f /tmp/mini_*_$$.txt /tmp/m1_$$.txt /tmp/m2_$$.txt /tmp/s*_$$.txt 2>/dev/null
rm -f "$MINISHELL_OUT" "$BASH_OUT" "$MINISHELL_ERR" "$BASH_ERR" "$TEST_INPUT" 2>/dev/null
rm -f /tmp/valgrind*_$$.log 2>/dev/null

# ============================================================================
# RESULTS SUMMARY
# ============================================================================

print_header "TEST RESULTS SUMMARY"

echo -e "Platform:     ${BLUE}$OS_TYPE${NC}"
echo -e "Total Tests:  ${BLUE}$TOTAL_TESTS${NC}"
echo -e "Passed:       ${GREEN}$PASSED_TESTS${NC}"
echo -e "Failed:       ${RED}$FAILED_TESTS${NC}"

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "\n${GREEN}🎉 ALL TESTS PASSED! 🎉${NC}"
    echo -e "\n${YELLOW}⚠ Important:${NC} Test on both platforms before final submission!"
    exit 0
else
    PASS_RATE=$((PASSED_TESTS * 100 / TOTAL_TESTS))
    echo -e "\nPass Rate:    ${YELLOW}${PASS_RATE}%${NC}"
    echo -e "\n${YELLOW}⚠ Some tests failed. Review the output above.${NC}"
    echo -e "${YELLOW}⚠ Remember to test on the evaluation platform!${NC}\n"
    exit 1
fi
