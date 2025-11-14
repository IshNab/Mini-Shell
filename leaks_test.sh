#!/bin/bash

# ============================================================================
# MINISHELL VALGRIND MEMORY LEAK TEST SUITE
# Comprehensive memory leak detection for Ubuntu/Linux
# Focuses on: Memory leaks, Invalid reads/writes, File descriptor leaks
# ============================================================================

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m'

# Configuration
MINISHELL="./minishell"
VALGRIND_LOG="/tmp/valgrind_minishell_$$.log"
VALGRIND_SUMMARY="/tmp/valgrind_summary_$$.txt"
TEST_RESULTS="/tmp/test_results_$$.txt"

# Counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0
LEAK_TESTS=0
NO_LEAK_TESTS=0

# Valgrind options
VALGRIND_OPTS="--leak-check=full \
--show-leak-kinds=all \
--track-origins=yes \
--track-fds=yes \
--trace-children=yes \
--suppressions=/dev/null"

# For readline - we need to suppress some known leaks
READLINE_SUPPRESSION="/tmp/readline_$$.supp"

# ============================================================================
# HELPER FUNCTIONS
# ============================================================================

print_header() {
    echo -e "\n${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${CYAN}  $1${NC}"
    echo -e "${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}\n"
}

print_test() {
    echo -e "\n${YELLOW}╔══════════════════════════════════════════════════════════╗${NC}"
    echo -e "${YELLOW}║${NC} Test #$TOTAL_TESTS: $1"
    echo -e "${YELLOW}╚══════════════════════════════════════════════════════════╝${NC}"
}

print_subtest() {
    echo -e "${BLUE}  ➜ $1${NC}"
}

# Create readline suppression file
create_suppressions() {
    cat > "$READLINE_SUPPRESSION" << 'EOF'
{
   readline_leak_1
   Memcheck:Leak
   ...
   fun:readline
}
{
   readline_leak_2
   Memcheck:Leak
   ...
   obj:*/libreadline.so*
}
{
   add_history_leak
   Memcheck:Leak
   ...
   fun:add_history
}
EOF
}

# Run valgrind with a command
run_valgrind_test() {
    local test_name="$1"
    local command="$2"
    local expect_leaks="${3:-no}"  # 'yes' if we expect leaks, 'no' otherwise

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    print_test "$test_name"
    print_subtest "Command: ${MAGENTA}$command${NC}"

    # Run with valgrind
    echo "$command" | timeout 5 valgrind $VALGRIND_OPTS \
        --log-file="$VALGRIND_LOG" \
        --suppressions="$READLINE_SUPPRESSION" \
        $MINISHELL > /dev/null 2>&1

    local exit_code=$?

    # Check if valgrind ran successfully
    if [ ! -f "$VALGRIND_LOG" ]; then
        echo -e "  ${RED}✗ FAILED${NC} - Valgrind log not created"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return 1
    fi

     # Parse valgrind output
    local definitely_lost=$(grep "definitely lost:" "$VALGRIND_LOG" | awk '{print $4}' | tr -d ',' | head -1)
    local indirectly_lost=$(grep "indirectly lost:" "$VALGRIND_LOG" | awk '{print $4}' | tr -d ',' | head -1)
    local possibly_lost=$(grep "possibly lost:" "$VALGRIND_LOG" | awk '{print $4}' | tr -d ',' | head -1)
    local still_reachable=$(grep "still reachable:" "$VALGRIND_LOG" | awk '{print $4}' | tr -d ',' | head -1)
    local invalid_reads=$(grep "Invalid read" "$VALGRIND_LOG" | wc -l)
    local invalid_writes=$(grep "Invalid write" "$VALGRIND_LOG" | wc -l)

    # More robust FD leak detection - try multiple patterns
    local fd_leaks=$(grep "FILE DESCRIPTOR" "$VALGRIND_LOG" | grep -o "open at exit: [0-9]*" | grep -o "[0-9]*" | head -1)

    # If that didn't work, try alternative pattern
    if [ -z "$fd_leaks" ]; then
        fd_leaks=$(grep -E "Open file descriptor.*at exit" "$VALGRIND_LOG" | wc -l)
    fi

    # Default to 0 if empty - simpler approach
    definitely_lost="${definitely_lost:-0}"
    indirectly_lost="${indirectly_lost:-0}"
    possibly_lost="${possibly_lost:-0}"
    still_reachable="${still_reachable:-0}"
    fd_leaks="${fd_leaks:-0}"

    # Clean any remaining non-numeric characters
    definitely_lost=$(echo "$definitely_lost" | sed 's/[^0-9]//g')
    indirectly_lost=$(echo "$indirectly_lost" | sed 's/[^0-9]//g')
    possibly_lost=$(echo "$possibly_lost" | sed 's/[^0-9]//g')
    still_reachable=$(echo "$still_reachable" | sed 's/[^0-9]//g')
    fd_leaks=$(echo "$fd_leaks" | sed 's/[^0-9]//g')

    # Final safety check - ensure they're not empty after cleaning
    definitely_lost="${definitely_lost:-0}"
    indirectly_lost="${indirectly_lost:-0}"
    possibly_lost="${possibly_lost:-0}"
    still_reachable="${still_reachable:-0}"
    fd_leaks="${fd_leaks:-0}"

    # Calculate total leaks
    local total_lost=$((definitely_lost + indirectly_lost))

    # Print detailed results
    echo ""
    print_subtest "Memory Analysis:"
    echo -e "    ${CYAN}├─${NC} Definitely lost:  ${definitely_lost} bytes"
    echo -e "    ${CYAN}├─${NC} Indirectly lost:  ${indirectly_lost} bytes"
    echo -e "    ${CYAN}├─${NC} Possibly lost:    ${possibly_lost} bytes"
    echo -e "    ${CYAN}├─${NC} Still reachable:  ${still_reachable} bytes"
    echo -e "    ${CYAN}├─${NC} Invalid reads:    ${invalid_reads}"
    echo -e "    ${CYAN}├─${NC} Invalid writes:   ${invalid_writes}"
    echo -e "    ${CYAN}└─${NC} Open FDs at exit: ${fd_leaks}"
    echo ""

    # Determine if test passed
    local has_critical_leaks=0

    # Critical: definitely lost or indirectly lost
    if [ $definitely_lost -gt 0 ] || [ $indirectly_lost -gt 0 ]; then
        has_critical_leaks=1
        echo -e "  ${RED}✗ MEMORY LEAK DETECTED${NC}"
        echo -e "    ${RED}Critical: $total_lost bytes lost${NC}"
        LEAK_TESTS=$((LEAK_TESTS + 1))
        FAILED_TESTS=$((FAILED_TESTS + 1))

        # Show where leaks occurred
        echo -e "\n  ${YELLOW}Leak locations:${NC}"
        grep -A 5 "definitely lost\|indirectly lost" "$VALGRIND_LOG" | head -20 | sed 's/^/    /'

    elif [ $invalid_reads -gt 0 ] || [ $invalid_writes -gt 0 ]; then
        echo -e "  ${RED}✗ INVALID MEMORY ACCESS${NC}"
        echo -e "    ${RED}Invalid reads: $invalid_reads, Invalid writes: $invalid_writes${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))

        # Show invalid access details
        echo -e "\n  ${YELLOW}Invalid access details:${NC}"
        grep -B 2 -A 5 "Invalid read\|Invalid write" "$VALGRIND_LOG" | head -15 | sed 's/^/    /'

    elif [ $fd_leaks -gt 4 ]; then  # 0,1,2 + valgrind's own FDs
        echo -e "  ${YELLOW}⚠ WARNING: File descriptor leak${NC}"
        echo -e "    ${YELLOW}$fd_leaks file descriptors open at exit${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))

    elif [ $possibly_lost -gt 1000 ]; then
        echo -e "  ${YELLOW}⚠ WARNING: Possibly lost memory${NC}"
        echo -e "    ${YELLOW}$possibly_lost bytes possibly lost${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        NO_LEAK_TESTS=$((NO_LEAK_TESTS + 1))

    else
        echo -e "  ${GREEN}✓ NO LEAKS DETECTED${NC}"
        if [ $still_reachable -gt 0 ]; then
            echo -e "    ${CYAN}Note: $still_reachable bytes still reachable (not a leak)${NC}"
        fi
        PASSED_TESTS=$((PASSED_TESTS + 1))
        NO_LEAK_TESTS=$((NO_LEAK_TESTS + 1))
    fi

    # Save summary
    echo "Test: $test_name | Command: $command | Result: Lost=$total_lost Invalid_R=$invalid_reads Invalid_W=$invalid_writes FDs=$fd_leaks" >> "$TEST_RESULTS"

    # Cleanup
    rm -f "$VALGRIND_LOG"
}

# ============================================================================
# CHECK PREREQUISITES
# ============================================================================

print_header "VALGRIND MEMORY LEAK TEST SUITE"

echo -e "${BLUE}Checking prerequisites...${NC}\n"

# Check if minishell exists
if [ ! -f "$MINISHELL" ]; then
    echo -e "${RED}✗ Error: minishell executable not found!${NC}"
    echo "  Please compile your minishell first: make"
    exit 1
fi
echo -e "${GREEN}✓${NC} Found minishell executable"

# Check if valgrind is installed
if ! command -v valgrind &> /dev/null; then
    echo -e "${RED}✗ Error: valgrind is not installed!${NC}"
    echo "  Install with: sudo apt-get install valgrind"
    exit 1
fi
echo -e "${GREEN}✓${NC} Valgrind is installed: $(valgrind --version)"

# Check valgrind version
VALGRIND_VERSION=$(valgrind --version | grep -oP '\d+\.\d+' | head -1)
echo -e "${GREEN}✓${NC} Valgrind version: $VALGRIND_VERSION"

# Check if timeout is available
if ! command -v timeout &> /dev/null; then
    echo -e "${RED}✗ Error: timeout command not found!${NC}"
    exit 1
fi
echo -e "${GREEN}✓${NC} Timeout command available"

echo ""
echo -e "${CYAN}Configuration:${NC}"
echo -e "  • Minishell: $MINISHELL"
echo -e "  • Track file descriptors: Yes"
echo -e "  • Track origins: Yes"
echo -e "  • Suppress readline leaks: Yes"

# Create suppression file
create_suppressions
echo -e "  • Suppression file: $READLINE_SUPPRESSION"

rm -f "$TEST_RESULTS"

# ============================================================================
# CATEGORY 1: BASIC COMMANDS
# ============================================================================

print_header "CATEGORY 1: BASIC COMMAND EXECUTION"

run_valgrind_test "Simple echo" "echo hello"
run_valgrind_test "Echo with arguments" "echo hello world test"
run_valgrind_test "Empty echo" "echo"
run_valgrind_test "PWD builtin" "pwd"
run_valgrind_test "ENV builtin" "env"
run_valgrind_test "External command - ls" "ls"
run_valgrind_test "External command - cat" "cat /etc/hosts"
run_valgrind_test "Command with multiple args" "ls -la /tmp"

# ============================================================================
# CATEGORY 2: PIPES
# ============================================================================

print_header "CATEGORY 2: PIPE OPERATIONS"

run_valgrind_test "Simple pipe" "echo hello | cat"
run_valgrind_test "Pipe with grep" "echo 'hello world' | grep hello"
run_valgrind_test "Pipe with wc" "echo 'one two three' | wc -w"
run_valgrind_test "Double pipe" "echo test | cat | cat"
run_valgrind_test "Triple pipe" "echo test | cat | cat | cat"
run_valgrind_test "Long pipe chain" "echo test | cat | cat | cat | cat | cat | cat"
run_valgrind_test "Pipe with builtin" "echo test | cat"
run_valgrind_test "Complex pipe" "ls -la | grep minishell | wc -l"

# ============================================================================
# CATEGORY 3: REDIRECTIONS
# ============================================================================

print_header "CATEGORY 3: REDIRECTIONS"

# Create test input file
echo "test input content" > /tmp/leak_test_input.txt

run_valgrind_test "Output redirect" "echo hello > /tmp/leak_out.txt"
run_valgrind_test "Input redirect" "cat < /tmp/leak_test_input.txt"
run_valgrind_test "Append redirect" "echo line1 >> /tmp/leak_append.txt"
run_valgrind_test "Multiple redirects" "echo test > /tmp/leak1.txt > /tmp/leak2.txt"
run_valgrind_test "Input and output" "cat < /tmp/leak_test_input.txt > /tmp/leak_both.txt"
run_valgrind_test "Redirect with pipe" "echo hello | cat > /tmp/leak_pipe.txt"
run_valgrind_test "Redirect to /dev/null" "echo hidden > /dev/null"

# Heredoc test (special handling)
print_test "Heredoc test"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
print_subtest "Command: ${MAGENTA}cat << EOF (with content)${NC}"
echo -e "cat << EOF\nhello\nworld\nEOF" | timeout 5 valgrind $VALGRIND_OPTS \
    --log-file="$VALGRIND_LOG" \
    --suppressions="$READLINE_SUPPRESSION" \
    $MINISHELL > /dev/null 2>&1

if [ -f "$VALGRIND_LOG" ]; then
    definitely_lost=$(grep "definitely lost:" "$VALGRIND_LOG" | awk '{print $4}' | tr -d ',')
    definitely_lost=${definitely_lost:-0}
    if [ $definitely_lost -eq 0 ]; then
        echo -e "  ${GREEN}✓ NO LEAKS DETECTED${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        NO_LEAK_TESTS=$((NO_LEAK_TESTS + 1))
    else
        echo -e "  ${RED}✗ MEMORY LEAK DETECTED${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        LEAK_TESTS=$((LEAK_TESTS + 1))
    fi
else
    echo -e "  ${RED}✗ FAILED - Valgrind log not created${NC}"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi
rm -f "$VALGRIND_LOG"

# ============================================================================
# CATEGORY 4: VARIABLE EXPANSION
# ============================================================================

print_header "CATEGORY 4: VARIABLE EXPANSION"

export TEST_VAR="test_value"
export USER_VAR="john"

run_valgrind_test "Simple variable" "echo \$USER"
run_valgrind_test "Variable in string" "echo Hello \$USER"
run_valgrind_test "Multiple variables" "echo \$USER \$HOME \$PATH"
run_valgrind_test "Custom variable" "echo \$TEST_VAR"
run_valgrind_test "Exit status" "true && echo \$?"
run_valgrind_test "Undefined variable" "echo \$UNDEFINED_VAR_XYZ"
run_valgrind_test "Variable with text" "echo \${USER}test"
run_valgrind_test "Variable in quotes" 'echo "$USER"'

# ============================================================================
# CATEGORY 5: QUOTES
# ============================================================================

print_header "CATEGORY 5: QUOTE HANDLING"

run_valgrind_test "Single quotes" "echo 'hello world'"
run_valgrind_test "Double quotes" 'echo "hello world"'
run_valgrind_test "Empty quotes" 'echo ""'
run_valgrind_test "Quotes with spaces" 'echo "hello   world"'
run_valgrind_test "Mixed quotes" "echo 'hello' \"world\""
run_valgrind_test "Nested scenarios" "echo \"It's working\""
run_valgrind_test "Variable in quotes" 'echo "User: $USER"'

# ============================================================================
# CATEGORY 6: LOGICAL OPERATORS
# ============================================================================

print_header "CATEGORY 6: LOGICAL OPERATORS"

run_valgrind_test "AND success" "true && echo success"
run_valgrind_test "AND failure" "false && echo fail"
run_valgrind_test "OR success" "true || echo no"
run_valgrind_test "OR failure" "false || echo yes"
run_valgrind_test "Multiple AND" "true && true && echo ok"
run_valgrind_test "Multiple OR" "false || false || echo ok"
run_valgrind_test "Mixed operators" "true && false || echo mixed"
run_valgrind_test "Commands with AND" "echo first && echo second"

# ============================================================================
# CATEGORY 7: BUILTINS
# ============================================================================

print_header "CATEGORY 7: BUILTIN COMMANDS"

run_valgrind_test "Echo -n flag" "echo -n hello"
run_valgrind_test "CD to /tmp" "cd /tmp"
run_valgrind_test "CD to home" "cd"
run_valgrind_test "Export variable" "export NEW_VAR=value"
run_valgrind_test "Unset variable" "unset PATH"
run_valgrind_test "Multiple exports" "export A=1 && export B=2 && export C=3"

# ============================================================================
# CATEGORY 8: EDGE CASES
# ============================================================================

print_header "CATEGORY 8: EDGE CASES AND STRESS TESTS"

run_valgrind_test "Empty command" ""
run_valgrind_test "Only spaces" "   "
run_valgrind_test "Multiple spaces" "echo     hello     world"
run_valgrind_test "Very long command" "echo this is a very long command with many many many arguments to test memory allocation"
run_valgrind_test "Command not found" "nonexistent_command_xyz_123"
run_valgrind_test "Multiple pipes" "echo a | cat | cat | cat | cat | cat | cat | cat | cat"
run_valgrind_test "Multiple redirects" "echo a > /tmp/l1.txt > /tmp/l2.txt > /tmp/l3.txt > /tmp/l4.txt"
run_valgrind_test "Complex combination" "echo test | cat > /tmp/leak_complex.txt && cat /tmp/leak_complex.txt"

# ============================================================================
# CATEGORY 9: REPEATED OPERATIONS
# ============================================================================

print_header "CATEGORY 9: REPEATED OPERATIONS (Memory Accumulation Test)"

# Test for memory accumulation (running same command multiple times)
print_test "Repeated echo (5 times)"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
print_subtest "Testing if memory accumulates over multiple commands"

echo -e "echo test1\necho test2\necho test3\necho test4\necho test5" | timeout 5 valgrind $VALGRIND_OPTS \
    --log-file="$VALGRIND_LOG" \
    --suppressions="$READLINE_SUPPRESSION" \
    $MINISHELL > /dev/null 2>&1

if [ -f "$VALGRIND_LOG" ]; then
    definitely_lost=$(grep "definitely lost:" "$VALGRIND_LOG" | awk '{print $4}' | tr -d ',')
    definitely_lost=${definitely_lost:-0}
    echo ""
    print_subtest "Memory Analysis:"
    echo -e "    ${CYAN}└─${NC} Definitely lost: ${definitely_lost} bytes"
    echo ""
    if [ $definitely_lost -eq 0 ]; then
        echo -e "  ${GREEN}✓ NO MEMORY ACCUMULATION${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        NO_LEAK_TESTS=$((NO_LEAK_TESTS + 1))
    else
        echo -e "  ${RED}✗ MEMORY ACCUMULATES${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        LEAK_TESTS=$((LEAK_TESTS + 1))
    fi
fi
rm -f "$VALGRIND_LOG"

# ============================================================================
# CATEGORY 10: FILE DESCRIPTOR LEAKS
# ============================================================================

print_header "CATEGORY 10: FILE DESCRIPTOR LEAK TESTS"

run_valgrind_test "Multiple file operations" "cat /etc/hosts > /tmp/fd1.txt && cat /tmp/fd1.txt"
run_valgrind_test "Pipe with redirects" "echo test | cat > /tmp/fd2.txt"
run_valgrind_test "Failed file open" "cat /nonexistent/file.txt"
run_valgrind_test "Multiple redirects same cmd" "echo test > /tmp/fd3.txt >> /tmp/fd4.txt"

# ============================================================================
# CLEANUP
# ============================================================================

rm -f /tmp/leak_*.txt /tmp/fd*.txt /tmp/l*.txt 2>/dev/null
rm -f "$READLINE_SUPPRESSION" "$VALGRIND_LOG" 2>/dev/null

# ============================================================================
# FINAL RESULTS
# ============================================================================

print_header "FINAL RESULTS"

echo -e "${CYAN}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║                   TEST STATISTICS                          ║${NC}"
echo -e "${CYAN}╠════════════════════════════════════════════════════════════╣${NC}"
printf "${CYAN}║${NC} %-35s ${BLUE}%20s${NC} ${CYAN}║${NC}\n" "Total Tests Run:" "$TOTAL_TESTS"
printf "${CYAN}║${NC} %-35s ${GREEN}%20s${NC} ${CYAN}║${NC}\n" "Tests Passed (No Leaks):" "$NO_LEAK_TESTS"
printf "${CYAN}║${NC} %-35s ${RED}%20s${NC} ${CYAN}║${NC}\n" "Tests Failed (With Leaks):" "$LEAK_TESTS"
echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}"

# Calculate percentage
if [ $TOTAL_TESTS -gt 0 ]; then
    PASS_PERCENTAGE=$((NO_LEAK_TESTS * 100 / TOTAL_TESTS))
else
    PASS_PERCENTAGE=0
fi

echo ""
echo -e "${CYAN}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║                   LEAK-FREE RATE                           ║${NC}"
echo -e "${CYAN}╠════════════════════════════════════════════════════════════╣${NC}"

if [ $PASS_PERCENTAGE -eq 100 ]; then
    printf "${CYAN}║${NC}                     ${GREEN}%3d%% LEAK-FREE${NC}                      ${CYAN}║${NC}\n" "$PASS_PERCENTAGE"
    echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}"
    echo ""
    echo -e "${GREEN}╔════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║                                                            ║${NC}"
    echo -e "${GREEN}║          🎉  PERFECT! NO MEMORY LEAKS DETECTED! 🎉         ║${NC}"
    echo -e "${GREEN}║                                                            ║${NC}"
    echo -e "${GREEN}║         Your minishell is ready for evaluation!           ║${NC}"
    echo -e "${GREEN}║                                                            ║${NC}"
    echo -e "${GREEN}╚════════════════════════════════════════════════════════════╝${NC}"
    echo ""
elif [ $PASS_PERCENTAGE -ge 90 ]; then
    printf "${CYAN}║${NC}                     ${YELLOW}%3d%% LEAK-FREE${NC}                      ${CYAN}║${NC}\n" "$PASS_PERCENTAGE"
    echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}"
    echo ""
    echo -e "${YELLOW}╔════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${YELLOW}║  ⚠️  ALMOST THERE! Just a few leaks to fix.               ║${NC}"
    echo -e "${YELLOW}╚════════════════════════════════════════════════════════════╝${NC}"
elif [ $PASS_PERCENTAGE -ge 70 ]; then
    printf "${CYAN}║${NC}                     ${YELLOW}%3d%% LEAK-FREE${NC}                      ${CYAN}║${NC}\n" "$PASS_PERCENTAGE"
    echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}"
    echo ""
    echo -e "${YELLOW}╔════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${YELLOW}║  ⚠️  GOOD PROGRESS! Some memory leaks need attention.     ║${NC}"
    echo -e "${YELLOW}╚════════════════════════════════════════════════════════════╝${NC}"
else
    printf "${CYAN}║${NC}                     ${RED}%3d%% LEAK-FREE${NC}                      ${CYAN}║${NC}\n" "$PASS_PERCENTAGE"
    echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}"
    echo ""
    echo -e "${RED}╔════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${RED}║  ✗  CRITICAL: Multiple memory leaks detected!              ║${NC}"
    echo -e "${RED}║     Review the detailed output above.                      ║${NC}"
    echo -e "${RED}╚════════════════════════════════════════════════════════════╝${NC}"
fi

echo ""
echo -e "${CYAN}Detailed results saved to: ${NC}$TEST_RESULTS"
echo ""

# Print common leak sources
if [ $LEAK_TESTS -gt 0 ]; then
    echo -e "${YELLOW}╔════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${YELLOW}║           COMMON LEAK SOURCES IN MINISHELL                 ║${NC}"
    echo -e "${YELLOW}╠════════════════════════════════════════════════════════════╣${NC}"
    echo -e "${YELLOW}║${NC} 1. Not freeing tokens after parsing                       ${YELLOW}║${NC}"
    echo -e "${YELLOW}║${NC} 2. Not freeing AST nodes after execution                  ${YELLOW}║${NC}"
    echo -e "${YELLOW}║${NC} 3. Not closing file descriptors (pipes, redirections)     ${YELLOW}║${NC}"
    echo -e "${YELLOW}║${NC} 4. Not freeing environment variable list                  ${YELLOW}║${NC}"
    echo -e "${YELLOW}║${NC} 5. Not freeing argv arrays in command nodes               ${YELLOW}║${NC}"
    echo -e "${YELLOW}║${NC} 6. Losing readline() allocated strings                    ${YELLOW}║${NC}"
    echo -e "${YELLOW}║${NC} 7. Not freeing expanded variable strings                  ${YELLOW}║${NC}"
    echo -e "${YELLOW}║${NC} 8. Not freeing paths from PATH splitting                  ${YELLOW}║${NC}"
    echo -e "${YELLOW}╚════════════════════════════════════════════════════════════╝${NC}"
    echo ""
fi

# Exit code
if [ $LEAK_TESTS -eq 0 ]; then
    exit 0
else
    exit 1
fi
