#!/bin/bash

# ============================================================================
#                       MINISHELL FULL TEST SUITE
# ============================================================================

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Counters
TOTAL=0
PASSED=0
FAILED=0

# Test result function
test_result() {
    TOTAL=$((TOTAL + 1))
    if [ $1 -eq 0 ]; then
        echo -e "${GREEN}✓ PASS${NC} - $2"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗ FAIL${NC} - $2"
        FAILED=$((FAILED + 1))
    fi
}

echo -e "${CYAN}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║           MINISHELL FULL TEST SUITE                       ║${NC}"
echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}\n"

# ============================================================================
# 1. BASIC COMMANDS
# ============================================================================
echo -e "${BLUE}[1/10] Testing Basic Commands...${NC}"

echo "ls" | ./minishell > /tmp/test_ls.txt 2>&1
test_result $? "ls command"

echo "pwd" | ./minishell > /tmp/test_pwd.txt 2>&1
grep -q "minishell" /tmp/test_pwd.txt
test_result $? "pwd command"

echo "echo hello world" | ./minishell > /tmp/test_echo.txt 2>&1
grep -q "hello world" /tmp/test_echo.txt
test_result $? "echo command"

echo "env | head -1" | ./minishell > /tmp/test_env.txt 2>&1
[ -s /tmp/test_env.txt ]
test_result $? "env command"

# ============================================================================
# 2. PIPES
# ============================================================================
echo -e "\n${BLUE}[2/10] Testing Pipes...${NC}"

echo "ls | grep Makefile" | ./minishell > /tmp/test_pipe1.txt 2>&1
grep -q "Makefile" /tmp/test_pipe1.txt
test_result $? "Simple pipe (ls | grep)"

echo "cat Makefile | grep NAME | head -1" | ./minishell > /tmp/test_pipe2.txt 2>&1
grep -q "NAME" /tmp/test_pipe2.txt
test_result $? "Multiple pipes (3 commands)"

echo "ls | wc -l" | ./minishell > /tmp/test_pipe3.txt 2>&1
[ -s /tmp/test_pipe3.txt ]
test_result $? "Pipe with wc"

echo "echo test | cat | cat | cat" | ./minishell > /tmp/test_pipe4.txt 2>&1
grep -q "test" /tmp/test_pipe4.txt
test_result $? "Multiple pipes (4 commands)"

# ============================================================================
# 3. REDIRECTIONS
# ============================================================================
echo -e "\n${BLUE}[3/10] Testing Redirections...${NC}"

echo "echo test > /tmp/test_redir_out.txt" | ./minishell > /dev/null 2>&1
grep -q "test" /tmp/test_redir_out.txt
test_result $? "Output redirection (>)"

echo "echo line1 > /tmp/test_append.txt" | ./minishell > /dev/null 2>&1
echo "echo line2 >> /tmp/test_append.txt" | ./minishell > /dev/null 2>&1
[ $(wc -l < /tmp/test_append.txt) -eq 2 ]
test_result $? "Append redirection (>>)"

echo "cat < Makefile | head -1" | ./minishell > /tmp/test_redir_in.txt 2>&1
[ -s /tmp/test_redir_in.txt ]
test_result $? "Input redirection (<)"

echo "cat < Makefile > /tmp/test_both.txt" | ./minishell > /dev/null 2>&1
[ -s /tmp/test_both.txt ]
test_result $? "Input & Output redirection"

# ============================================================================
# 4. QUOTES
# ============================================================================
echo -e "\n${BLUE}[4/10] Testing Quotes...${NC}"

echo "echo 'hello world'" | ./minishell > /tmp/test_sq.txt 2>&1
grep -q "hello world" /tmp/test_sq.txt
test_result $? "Single quotes"

echo 'echo "hello world"' | ./minishell > /tmp/test_dq.txt 2>&1
grep -q "hello world" /tmp/test_dq.txt
test_result $? "Double quotes"

echo "echo 'test1' \"test2\" test3" | ./minishell > /tmp/test_mixed.txt 2>&1
grep -q "test1 test2 test3" /tmp/test_mixed.txt
test_result $? "Mixed quotes"

# ============================================================================
# 5. PATHS (ABSOLUTE & RELATIVE)
# ============================================================================
echo -e "\n${BLUE}[5/10] Testing Paths...${NC}"

echo "/bin/ls" | ./minishell > /tmp/test_abs.txt 2>&1
[ -s /tmp/test_abs.txt ]
test_result $? "Absolute path (/bin/ls)"

echo "/usr/bin/env | head -1" | ./minishell > /tmp/test_abs2.txt 2>&1
[ -s /tmp/test_abs2.txt ]
test_result $? "Absolute path (/usr/bin/env)"

echo "./minishell" | timeout 2 ./minishell > /tmp/test_rel.txt 2>&1
test_result $? "Relative path (./minishell)"

# ============================================================================
# 6. BUILTINS
# ============================================================================
echo -e "\n${BLUE}[6/10] Testing Builtins...${NC}"

echo "pwd" | ./minishell > /tmp/test_pwd_builtin.txt 2>&1
[ -s /tmp/test_pwd_builtin.txt ]
test_result $? "Builtin: pwd"

echo "echo test" | ./minishell > /tmp/test_echo_builtin.txt 2>&1
grep -q "test" /tmp/test_echo_builtin.txt
test_result $? "Builtin: echo"

echo "env | grep PATH" | ./minishell > /tmp/test_env_builtin.txt 2>&1
grep -q "PATH" /tmp/test_env_builtin.txt
test_result $? "Builtin: env"

# ============================================================================
# 7. BUILTIN WITH REDIRECTIONS
# ============================================================================
echo -e "\n${BLUE}[7/10] Testing Builtins with Redirections...${NC}"

echo "echo test > /tmp/test_echo_redir.txt" | ./minishell > /dev/null 2>&1
grep -q "test" /tmp/test_echo_redir.txt
test_result $? "echo with output redirection"

echo "pwd > /tmp/test_pwd_redir.txt" | ./minishell > /dev/null 2>&1
[ -s /tmp/test_pwd_redir.txt ]
test_result $? "pwd with output redirection"

echo "env | grep USER > /tmp/test_env_redir.txt" | ./minishell > /dev/null 2>&1
[ -s /tmp/test_env_redir.txt ]
test_result $? "env with pipe and redirection"

# ============================================================================
# 8. COMPLEX PIPELINES
# ============================================================================
echo -e "\n${BLUE}[8/10] Testing Complex Pipelines...${NC}"

echo "cat Makefile | grep CC | wc -l" | ./minishell > /tmp/test_complex1.txt 2>&1
[ -s /tmp/test_complex1.txt ]
test_result $? "3-stage pipeline"

echo "ls -la | grep src | cat | wc -l" | ./minishell > /tmp/test_complex2.txt 2>&1
[ -s /tmp/test_complex2.txt ]
test_result $? "4-stage pipeline"

echo "echo test | cat | cat | cat | cat" | ./minishell > /tmp/test_complex3.txt 2>&1
grep -q "test" /tmp/test_complex3.txt
test_result $? "5-stage pipeline"

# ============================================================================
# 9. ERROR HANDLING
# ============================================================================
echo -e "\n${BLUE}[9/10] Testing Error Handling...${NC}"

echo "commandnotfound" | ./minishell 2>&1 | grep -q "command not found"
test_result $? "Command not found error"

echo "cat nonexistentfile" | ./minishell 2>&1 | grep -q -e "No such file" -e "cannot open"
test_result $? "File not found error"

echo "ls | grep" | ./minishell > /dev/null 2>&1
test_result $? "Incomplete pipe handling"

# ============================================================================
# 10. MEMORY LEAKS (VALGRIND)
# ============================================================================
echo -e "\n${BLUE}[10/10] Testing Memory Leaks (Valgrind)...${NC}"

echo "ls" | valgrind --leak-check=full --show-leak-kinds=definite,possible --error-exitcode=1 ./minishell > /dev/null 2>&1
test_result $? "Simple command - no leaks"

echo "ls | grep src" | valgrind --leak-check=full --show-leak-kinds=definite,possible --error-exitcode=1 ./minishell > /dev/null 2>&1
test_result $? "Pipe - no leaks"

echo "cat < Makefile > /tmp/test_valgrind.txt" | valgrind --leak-check=full --show-leak-kinds=definite,possible --error-exitcode=1 ./minishell > /dev/null 2>&1
test_result $? "Redirections - no leaks"

# ============================================================================
# CLEANUP
# ============================================================================
rm -f /tmp/test_*.txt

# ============================================================================
# FINAL REPORT
# ============================================================================
echo -e "\n${CYAN}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║                    TEST SUMMARY                           ║${NC}"
echo -e "${CYAN}╠════════════════════════════════════════════════════════════╣${NC}"
echo -e "${CYAN}║${NC} Total Tests:  ${YELLOW}$TOTAL${NC}"
echo -e "${CYAN}║${NC} Passed:       ${GREEN}$PASSED${NC}"
echo -e "${CYAN}║${NC} Failed:       ${RED}$FAILED${NC}"
echo -e "${CYAN}╠════════════════════════════════════════════════════════════╣${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "${CYAN}║${NC} ${GREEN}✓ ALL TESTS PASSED!${NC} 🎉"
    echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}\n"
    exit 0
else
    PERCENTAGE=$((PASSED * 100 / TOTAL))
    echo -e "${CYAN}║${NC} ${YELLOW}Success Rate: $PERCENTAGE%${NC}"
    echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}\n"
    exit 1
fi
