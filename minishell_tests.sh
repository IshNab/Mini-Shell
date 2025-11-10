#!/bin/bash

# ============================================================================
# MINISHELL COMPREHENSIVE TEST SUITE
# Tests: Pipes, Redirections, Builtins, External Commands, Variables, Signals
# ============================================================================

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Minishell executable
MINISHELL="./minishell"

# Temp files for comparison
MINISHELL_OUT="/tmp/minishell_out.txt"
BASH_OUT="/tmp/bash_out.txt"
MINISHELL_ERR="/tmp/minishell_err.txt"
BASH_ERR="/tmp/bash_err.txt"

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
    echo "$cmd" | timeout 2 $MINISHELL > "$MINISHELL_OUT" 2> "$MINISHELL_ERR"
    return $?
}

run_bash() {
    local cmd="$1"
    echo "$cmd" | timeout 2 bash --posix > "$BASH_OUT" 2> "$BASH_ERR"
    return $?
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
    
    # Compare outputs
    if diff -q "$MINISHELL_OUT" "$BASH_OUT" > /dev/null 2>&1; then
        if [ $mini_exit -eq $bash_exit ]; then
            echo -e "  ${GREEN}✓ PASSED${NC}"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "  ${RED}✗ FAILED${NC} (exit status: mini=$mini_exit, bash=$bash_exit)"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    else
        echo -e "  ${RED}✗ FAILED${NC} (output mismatch)"
        echo -e "  ${YELLOW}Expected:${NC}"
        cat "$BASH_OUT" | head -3
        echo -e "  ${YELLOW}Got:${NC}"
        cat "$MINISHELL_OUT" | head -3
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
echo -e "Executable: $MINISHELL\n"

# ============================================================================
# 1. BASIC COMMANDS
# ============================================================================

print_header "1. BASIC EXTERNAL COMMANDS"

test_command "Simple ls" "ls"
test_command "ls with flag" "ls -l"
test_command "ls with multiple flags" "ls -la"
test_command "echo simple" "echo hello"
test_command "echo multiple args" "echo hello world"
test_command "pwd" "pwd"
test_command "cat with file" "cat /etc/hosts"
test_command "grep simple" "echo 'hello world' | grep hello"
test_command "wc" "echo 'hello world' | wc -w"

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
test_command "env builtin" "env | grep USER"

# CD tests (need special handling)
print_test "cd to home"
echo -e "cd\npwd" | $MINISHELL > "$MINISHELL_OUT" 2>&1
if [ $? -eq 0 ]; then
    echo -e "  ${GREEN}✓ PASSED${NC}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "  ${RED}✗ FAILED${NC}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

print_test "cd to /tmp"
echo -e "cd /tmp\npwd" | $MINISHELL > "$MINISHELL_OUT" 2>&1
if grep -q "/tmp" "$MINISHELL_OUT"; then
    echo -e "  ${GREEN}✓ PASSED${NC}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "  ${RED}✗ FAILED${NC}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

# ============================================================================
# 3. PIPES
# ============================================================================

print_header "3. PIPE TESTS"

test_command "Simple pipe" "echo hello | cat"
test_command "Pipe with grep" "echo 'hello world' | grep hello"
test_command "Pipe with wc" "echo 'one two three' | wc -w"
test_command "Multiple pipes" "echo 'hello world' | cat | cat | cat"
test_command "ls with pipe" "ls | grep minishell"
test_command "cat pipe grep" "cat /etc/hosts | grep localhost"
test_command "echo pipe head" "echo -e 'line1\nline2\nline3' | head -1"
test_command "Complex pipe chain" "echo 'test' | cat | cat | grep test"
test_command "Pipe with sort" "echo -e '3\n1\n2' | sort"
test_command "Triple pipe" "echo hello | cat | cat | wc -c"

# ============================================================================
# 4. REDIRECTIONS
# ============================================================================

print_header "4. REDIRECTION TESTS"

# Create test files
echo "test content" > /tmp/test_input.txt
echo "line1" > /tmp/test_multiline.txt
echo "line2" >> /tmp/test_multiline.txt

test_command "Output redirect" "echo hello > /tmp/mini_test.txt && cat /tmp/mini_test.txt"
test_command "Input redirect" "cat < /tmp/test_input.txt"
test_command "Append redirect" "echo line1 > /tmp/mini_append.txt && echo line2 >> /tmp/mini_append.txt && cat /tmp/mini_append.txt"
test_command "Input and output" "cat < /tmp/test_input.txt > /tmp/mini_both.txt && cat /tmp/mini_both.txt"
test_command "Multiple redirects" "echo test > /tmp/m1.txt > /tmp/m2.txt && cat /tmp/m2.txt"
test_command "Redirect with pipe" "echo hello | cat > /tmp/mini_pipe_redir.txt && cat /tmp/mini_pipe_redir.txt"
test_command "Redirect stderr" "cat nonexistent 2> /tmp/mini_err.txt"

# Heredoc tests
print_test "Simple heredoc"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
echo -e "cat << EOF\nhello\nworld\nEOF" | timeout 2 $MINISHELL > "$MINISHELL_OUT" 2>&1
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
test_command "Variable with text" "echo \${USER}test"
test_command "Exit status" "echo test && echo \$?"
test_command "Exit status fail" "false && echo \$?"
test_command "Exit status after true" "true && echo \$?"
test_command "Undefined variable" "echo \$UNDEFINED_VAR_XYZ"
test_command "Variable in quotes" 'echo "\$USER"'
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
test_command "Nested quotes scenario" "echo \"It's working\""

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
test_command "Mixed operators" "true && false || echo mixed"
test_command "Command AND" "echo first && echo second"
test_command "Command OR" "false || echo fallback"

# ============================================================================
# 8. EDGE CASES
# ============================================================================

print_header "8. EDGE CASES"

test_command "Empty command" ""
test_command "Only spaces" "   "
test_command "Multiple spaces" "echo     hello     world"
test_command "Tabs" "echo\thello"
test_command "Multiple pipes empty" "echo hello | | cat"
test_command "Redirect nonexistent" "cat < /nonexistent/file"
test_command "Permission denied" "cat /etc/shadow"
test_command "Command not found" "nonexistentcommand123"
test_command "Very long pipe chain" "echo test | cat | cat | cat | cat | cat"
test_command "Redirect to /dev/null" "echo hidden > /dev/null"
test_command "Mixed operators complex" "echo a && echo b || echo c | cat"

# ============================================================================
# 9. SPECIAL CHARACTERS
# ============================================================================

print_header "9. SPECIAL CHARACTERS"

test_command "Asterisk expansion" "echo *"
test_command "Question mark" "echo test?"
test_command "Dollar sign escape" "echo \\\$USER"
test_command "Backslash" "echo \\\\"
test_command "Semicolon in string" "echo 'test;test'"
test_command "Pipe in string" "echo 'test|test'"
test_command "Ampersand in string" "echo 'test&test'"

# ============================================================================
# 10. BUILTIN SPECIFIC TESTS
# ============================================================================

print_header "10. BUILTIN SPECIFIC TESTS"

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

# PWD tests
test_command "pwd after cd" "cd /tmp && pwd"
test_command "pwd multiple times" "pwd && pwd"

# ============================================================================
# 11. SIGNAL HANDLING
# ============================================================================

print_header "11. SIGNAL HANDLING (Manual)"

echo -e "${YELLOW}The following tests require manual verification:${NC}"
echo ""
echo "1. Start minishell and press Ctrl+C"
echo "   → Should display new prompt without exiting"
echo ""
echo "2. Start minishell and press Ctrl+\\"
echo "   → Should do nothing (ignore SIGQUIT)"
echo ""
echo "3. Start minishell and press Ctrl+D"
echo "   → Should exit gracefully with 'exit'"
echo ""
echo "4. Run: cat (then press Ctrl+C)"
echo "   → Should interrupt cat and return to prompt"
echo ""
echo "5. Run: cat << EOF (then press Ctrl+C)"
echo "   → Should cancel heredoc and return to prompt"
echo ""

# ============================================================================
# 12. MEMORY LEAK TESTS
# ============================================================================

print_header "12. MEMORY LEAK TESTS (Valgrind)"

if ! command -v valgrind &> /dev/null; then
    echo -e "${YELLOW}Valgrind not found. Skipping memory leak tests.${NC}"
else
    echo -e "${YELLOW}Running valgrind tests (this may take a while)...${NC}\n"
    
    # Test 1: Simple command
    print_test "Valgrind: Simple command"
    echo "echo hello" | valgrind --leak-check=full --error-exitcode=1 $MINISHELL > /dev/null 2> /tmp/valgrind.log
    if [ $? -eq 0 ]; then
        echo -e "  ${GREEN}✓ NO LEAKS${NC}"
    else
        echo -e "  ${RED}✗ LEAKS DETECTED${NC}"
        echo "  See /tmp/valgrind.log for details"
    fi
    
    # Test 2: Pipes
    print_test "Valgrind: Pipes"
    echo "echo hello | cat | cat" | valgrind --leak-check=full --error-exitcode=1 $MINISHELL > /dev/null 2> /tmp/valgrind2.log
    if [ $? -eq 0 ]; then
        echo -e "  ${GREEN}✓ NO LEAKS${NC}"
    else
        echo -e "  ${RED}✗ LEAKS DETECTED${NC}"
        echo "  See /tmp/valgrind2.log for details"
    fi
    
    # Test 3: Redirections
    print_test "Valgrind: Redirections"
    echo "echo test > /tmp/vg_test.txt" | valgrind --leak-check=full --error-exitcode=1 $MINISHELL > /dev/null 2> /tmp/valgrind3.log
    if [ $? -eq 0 ]; then
        echo -e "  ${GREEN}✓ NO LEAKS${NC}"
    else
        echo -e "  ${RED}✗ LEAKS DETECTED${NC}"
        echo "  See /tmp/valgrind3.log for details"
    fi
    
    # Test 4: Variable expansion
    print_test "Valgrind: Variable expansion"
    echo "echo \$USER \$HOME" | valgrind --leak-check=full --error-exitcode=1 $MINISHELL > /dev/null 2> /tmp/valgrind4.log
    if [ $? -eq 0 ]; then
        echo -e "  ${GREEN}✓ NO LEAKS${NC}"
    else
        echo -e "  ${RED}✗ LEAKS DETECTED${NC}"
        echo "  See /tmp/valgrind4.log for details"
    fi
fi

# ============================================================================
# 13. STRESS TESTS
# ============================================================================

print_header "13. STRESS TESTS"

test_command "Long command" "echo this is a very long command with many arguments to test buffer handling and memory management"
test_command "Many pipes" "echo test | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat"
test_command "Long pipe chain" "echo test | grep test | cat | cat | wc -c | cat"
test_command "Multiple redirections" "echo test > /tmp/s1.txt > /tmp/s2.txt > /tmp/s3.txt && cat /tmp/s3.txt"

# ============================================================================
# CLEANUP
# ============================================================================

rm -f /tmp/mini_*.txt /tmp/test_*.txt /tmp/m1.txt /tmp/m2.txt /tmp/s*.txt /tmp/vg_test.txt 2>/dev/null
rm -f "$MINISHELL_OUT" "$BASH_OUT" "$MINISHELL_ERR" "$BASH_ERR" 2>/dev/null

# ============================================================================
# RESULTS SUMMARY
# ============================================================================

print_header "TEST RESULTS SUMMARY"

echo -e "Total Tests:  ${BLUE}$TOTAL_TESTS${NC}"
echo -e "Passed:       ${GREEN}$PASSED_TESTS${NC}"
echo -e "Failed:       ${RED}$FAILED_TESTS${NC}"

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "\n${GREEN}🎉 ALL TESTS PASSED! 🎉${NC}\n"
    exit 0
else
    PASS_RATE=$((PASSED_TESTS * 100 / TOTAL_TESTS))
    echo -e "\nPass Rate:    ${YELLOW}${PASS_RATE}%${NC}"
    echo -e "\n${YELLOW}⚠ Some tests failed. Review the output above.${NC}\n"
    exit 1
fi
