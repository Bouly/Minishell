#!/bin/bash

# ============================================================================
#                    MINISHELL STRESS TEST SUITE
# ============================================================================

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m' # No Color

# Options
QUICK_MODE=0
VALGRIND_MODE=0
VERBOSE=0

# Parse arguments
for arg in "$@"; do
    case $arg in
        --quick)
            QUICK_MODE=1
            ;;
        --valgrind)
            VALGRIND_MODE=1
            ;;
        --verbose)
            VERBOSE=1
            ;;
    esac
done

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
        if [ $VERBOSE -eq 1 ] && [ -n "$3" ]; then
            echo -e "${YELLOW}  Details: $3${NC}"
        fi
    fi
}

echo -e "${CYAN}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║         MINISHELL STRESS TEST SUITE                       ║${NC}"
echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}"
if [ $QUICK_MODE -eq 1 ]; then
    echo -e "${YELLOW}Mode: Quick Tests${NC}\n"
elif [ $VALGRIND_MODE -eq 1 ]; then
    echo -e "${MAGENTA}Mode: Valgrind Memory Check${NC}\n"
else
    echo -e "${GREEN}Mode: Full Stress Tests${NC}\n"
fi

# ============================================================================
# 1. BASIC COMMANDS
# ============================================================================
echo -e "${BLUE}[1/13] Testing Basic Commands...${NC}"

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
echo -e "\n${BLUE}[2/13] Testing Pipes...${NC}"

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
echo -e "\n${BLUE}[3/13] Testing Redirections...${NC}"

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

# Multiple redirections (should fail or take last one)
echo "echo test > /tmp/test_multi1.txt > /tmp/test_multi2.txt" | ./minishell > /dev/null 2>&1
test_result $? "Multiple output redirections"

# Heredoc (<<)
if [ $QUICK_MODE -eq 0 ]; then
    cat <<'EOF' | ./minishell > /tmp/test_heredoc.txt 2>&1
cat << END
line1
line2
END
EOF
    grep -q "line1" /tmp/test_heredoc.txt && grep -q "line2" /tmp/test_heredoc.txt
    test_result $? "Heredoc (<<)"
fi

# ============================================================================
# 4. QUOTES
# ============================================================================
echo -e "\n${BLUE}[4/13] Testing Quotes...${NC}"

# Single quotes
echo "echo 'hello world'" | ./minishell > /tmp/test_sq.txt 2>&1
grep -q "hello world" /tmp/test_sq.txt
test_result $? "Single quotes"

echo "echo ''" | ./minishell > /tmp/test_sq_empty.txt 2>&1
test_result $? "Single quotes (empty)"

echo "echo 'cat lol.c | cat > lol.c'" | ./minishell > /tmp/test_sq_pipe.txt 2>&1
grep -q "cat lol.c | cat > lol.c" /tmp/test_sq_pipe.txt
test_result $? "Single quotes (pipes not interpreted)"

echo "echo '\$USER'" | ./minishell > /tmp/test_sq_var.txt 2>&1
grep -q '\$USER' /tmp/test_sq_var.txt
test_result $? "Single quotes (variables not interpreted)"

# Double quotes
echo 'echo "hello world"' | ./minishell > /tmp/test_dq.txt 2>&1
grep -q "hello world" /tmp/test_dq.txt
test_result $? "Double quotes"

echo 'echo "cat lol.c | cat > lol.c"' | ./minishell > /tmp/test_dq_pipe.txt 2>&1
grep -q "cat lol.c | cat > lol.c" /tmp/test_dq_pipe.txt
test_result $? "Double quotes (pipes not interpreted)"

echo 'echo "; ; ; ;"' | ./minishell > /tmp/test_dq_semi.txt 2>&1
grep -q "; ; ; ;" /tmp/test_dq_semi.txt
test_result $? "Double quotes (semicolons not interpreted)"

# Mixed quotes
echo "echo 'test1' \"test2\" test3" | ./minishell > /tmp/test_mixed.txt 2>&1
grep -q "test1 test2 test3" /tmp/test_mixed.txt
test_result $? "Mixed quotes"
test_result $? "Double quotes"

echo "echo 'test1' \"test2\" test3" | ./minishell > /tmp/test_mixed.txt 2>&1
grep -q "test1 test2 test3" /tmp/test_mixed.txt
test_result $? "Mixed quotes"

# ============================================================================
# 5. PATHS (ABSOLUTE & RELATIVE)
# ============================================================================
echo -e "\n${BLUE}[5/13] Testing Paths...${NC}"

echo "/bin/ls" | ./minishell > /tmp/test_abs.txt 2>&1
[ -s /tmp/test_abs.txt ]
test_result $? "Absolute path (/bin/ls)"

echo "/usr/bin/env | head -1" | ./minishell > /tmp/test_abs2.txt 2>&1
[ -s /tmp/test_abs2.txt ]
test_result $? "Absolute path (/usr/bin/env)"

echo "./minishell" | timeout 2 ./minishell > /tmp/test_rel.txt 2>&1
test_result $? "Relative path (./minishell)"

# Relative path with ../..
mkdir -p /tmp/test_rel_dir/subdir
cp ./minishell /tmp/test_rel_dir/
echo "cd /tmp/test_rel_dir/subdir && ../../minishell" | timeout 2 ./minishell > /tmp/test_rel_complex.txt 2>&1
test_result $? "Relative path with ../.."

# PATH environment variable tests
echo "ls" | PATH="" ./minishell 2>&1 | grep -q "command not found"
test_result $? "Command without PATH (should fail)"

echo "echo test" | PATH="" ./minishell > /tmp/test_builtin_nopath.txt 2>&1
grep -q "test" /tmp/test_builtin_nopath.txt
test_result $? "Builtin without PATH (should work)"

# Multiple directories in PATH (left to right evaluation)
if [ $QUICK_MODE -eq 0 ]; then
    mkdir -p /tmp/path_test1 /tmp/path_test2
    echo '#!/bin/bash' > /tmp/path_test1/testcmd
    echo 'echo "from_path1"' >> /tmp/path_test1/testcmd
    echo '#!/bin/bash' > /tmp/path_test2/testcmd
    echo 'echo "from_path2"' >> /tmp/path_test2/testcmd
    chmod +x /tmp/path_test1/testcmd /tmp/path_test2/testcmd
    
    echo "testcmd" | PATH="/tmp/path_test1:/tmp/path_test2" ./minishell > /tmp/test_path_order.txt 2>&1
    grep -q "from_path1" /tmp/test_path_order.txt
    test_result $? "PATH directories evaluated left to right"
fi

# ============================================================================
# 6. BUILTINS
# ============================================================================
echo -e "\n${BLUE}[6/13] Testing Builtins...${NC}"

# PWD
echo "pwd" | ./minishell > /tmp/test_pwd_builtin.txt 2>&1
[ -s /tmp/test_pwd_builtin.txt ]
test_result $? "Builtin: pwd"

mkdir -p /tmp/test_cd_dir
echo "cd /tmp/test_cd_dir && pwd" | ./minishell > /tmp/test_pwd2.txt 2>&1
grep -q "test_cd_dir" /tmp/test_pwd2.txt
test_result $? "Builtin: pwd (after cd)"

# ECHO
echo "echo test" | ./minishell > /tmp/test_echo_builtin.txt 2>&1
grep -q "test" /tmp/test_echo_builtin.txt
test_result $? "Builtin: echo"

echo "echo -n test" | ./minishell > /tmp/test_echo_n.txt 2>&1
[ "$(cat /tmp/test_echo_n.txt)" = "test" ]
test_result $? "Builtin: echo -n (no newline)"

echo "echo -n -n -n test" | ./minishell > /tmp/test_echo_multiple_n.txt 2>&1
[ "$(cat /tmp/test_echo_multiple_n.txt)" = "test" ]
test_result $? "Builtin: echo multiple -n"

echo "echo" | ./minishell > /tmp/test_echo_empty.txt 2>&1
[ "$(cat /tmp/test_echo_empty.txt)" = "" ]
test_result $? "Builtin: echo (empty)"

# ENV
echo "env | grep PATH" | ./minishell > /tmp/test_env_builtin.txt 2>&1
grep -q "PATH" /tmp/test_env_builtin.txt
test_result $? "Builtin: env"

# EXPORT
echo -e "export TEST_VAR=hello\nenv | grep TEST_VAR" | ./minishell > /tmp/test_export.txt 2>&1
grep -q "TEST_VAR=hello" /tmp/test_export.txt
test_result $? "Builtin: export (new variable)"

echo -e "export PATH=/new/path\necho \$PATH" | ./minishell > /tmp/test_export_replace.txt 2>&1
grep -q "/new/path" /tmp/test_export_replace.txt
test_result $? "Builtin: export (replace existing)"

# UNSET
echo -e "export TEST_VAR=hello\nunset TEST_VAR\nenv | grep TEST_VAR" | ./minishell > /tmp/test_unset.txt 2>&1
! grep -q "TEST_VAR" /tmp/test_unset.txt
test_result $? "Builtin: unset"

# CD
echo "cd /tmp && pwd" | ./minishell > /tmp/test_cd_abs.txt 2>&1
grep -q "/tmp" /tmp/test_cd_abs.txt
test_result $? "Builtin: cd (absolute path)"

echo "cd . && pwd" | ./minishell > /tmp/test_cd_dot.txt 2>&1
[ -s /tmp/test_cd_dot.txt ]
test_result $? "Builtin: cd . (current dir)"

echo "cd .. && pwd" | ./minishell > /tmp/test_cd_dotdot.txt 2>&1
[ -s /tmp/test_cd_dotdot.txt ]
test_result $? "Builtin: cd .. (parent dir)"

echo "cd /nonexistent 2>&1" | ./minishell 2>&1 | grep -q -e "No such file" -e "cannot access"
test_result $? "Builtin: cd (error handling)"

# EXIT
if [ $QUICK_MODE -eq 0 ]; then
    echo "exit" | timeout 1 ./minishell > /tmp/test_exit.txt 2>&1
    test_result $? "Builtin: exit"

    echo "exit 42" | timeout 1 ./minishell > /tmp/test_exit_code.txt 2>&1
    EXIT_CODE=$?
    [ $EXIT_CODE -eq 42 ]
    test_result $? "Builtin: exit with code"
fi

# ============================================================================
# 7. BUILTIN WITH REDIRECTIONS
# ============================================================================
echo -e "\n${BLUE}[7/13] Testing Builtins with Redirections...${NC}"

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
echo -e "\n${BLUE}[8/13] Testing Complex Pipelines...${NC}"

echo "cat Makefile | grep CC | wc -l" | ./minishell > /tmp/test_complex1.txt 2>&1
[ -s /tmp/test_complex1.txt ]
test_result $? "3-stage pipeline"

echo "ls -la | grep src | cat | wc -l" | ./minishell > /tmp/test_complex2.txt 2>&1
[ -s /tmp/test_complex2.txt ]
test_result $? "4-stage pipeline"

echo "echo test | cat | cat | cat | cat" | ./minishell > /tmp/test_complex3.txt 2>&1
grep -q "test" /tmp/test_complex3.txt
test_result $? "5-stage pipeline"

# Pipes with failing commands
echo "ls nonexistent | grep bla | more" | ./minishell > /tmp/test_pipe_fail.txt 2>&1
test_result $? "Pipe with failing command"

# Mix pipes and redirections
echo "cat < Makefile | grep NAME > /tmp/test_pipe_redir.txt" | ./minishell > /dev/null 2>&1
[ -s /tmp/test_pipe_redir.txt ]
test_result $? "Pipes mixed with redirections"

echo "ls | cat > /tmp/test_pipe_out.txt" | ./minishell > /dev/null 2>&1
[ -s /tmp/test_pipe_out.txt ]
test_result $? "Pipe output to file"

# cat | cat | ls should work
echo "cat Makefile | cat | ls" | ./minishell > /tmp/test_cat_cat_ls.txt 2>&1
[ -s /tmp/test_cat_cat_ls.txt ]
test_result $? "cat | cat | ls"

# ============================================================================
# 9. ERROR HANDLING
# ============================================================================
echo -e "\n${BLUE}[9/13] Testing Error Handling...${NC}"

echo "commandnotfound" | ./minishell 2>&1 | grep -q "command not found"
test_result $? "Command not found error"

echo "cat nonexistentfile" | ./minishell 2>&1 | grep -q -e "No such file" -e "cannot open"
test_result $? "File not found error"

echo "ls | grep" | ./minishell > /dev/null 2>&1
test_result $? "Incomplete pipe handling"

# ============================================================================
# 10. ENVIRONMENT VARIABLES
# ============================================================================
echo -e "\n${BLUE}[10/13] Testing Environment Variables...${NC}"

echo 'echo $HOME' | ./minishell > /tmp/test_home.txt 2>&1
grep -q "/" /tmp/test_home.txt
test_result $? "Variable: \$HOME"

echo 'echo $USER' | ./minishell > /tmp/test_user.txt 2>&1
[ -s /tmp/test_user.txt ]
test_result $? "Variable: \$USER"

echo 'echo $PATH | grep bin' | ./minishell > /tmp/test_path.txt 2>&1
grep -q "bin" /tmp/test_path.txt
test_result $? "Variable: \$PATH"

echo 'echo $PWD | grep minishell' | ./minishell > /tmp/test_pwd_var.txt 2>&1
grep -q "minishell" /tmp/test_pwd_var.txt
test_result $? "Variable: \$PWD"

echo 'echo $NONEXISTENT' | ./minishell > /tmp/test_novar.txt 2>&1
[ ! -s /tmp/test_novar.txt ] || [ "$(cat /tmp/test_novar.txt | tr -d '\n')" = "" ]
test_result $? "Non-existent variable"

echo 'echo $HOME $USER' | ./minishell > /tmp/test_multi_var.txt 2>&1
[ -s /tmp/test_multi_var.txt ]
test_result $? "Multiple variables"

echo 'echo $USER | cat' | ./minishell > /tmp/test_var_pipe.txt 2>&1
[ -s /tmp/test_var_pipe.txt ]
test_result $? "Variable with pipe"

echo 'echo $HOME > /tmp/test_var_redir.txt' | ./minishell > /dev/null 2>&1
grep -q "/" /tmp/test_var_redir.txt
test_result $? "Variable with redirection"

# ============================================================================
# 11. EXIT STATUS ($?)
# ============================================================================
echo -e "\n${BLUE}[11/13] Testing Exit Status \$?...${NC}"

echo -e 'ls\necho $?' | ./minishell > /tmp/test_exitok.txt 2>&1
grep -q "0" /tmp/test_exitok.txt
test_result $? "Exit status after success"

echo -e 'commandnotfound\necho $?' | ./minishell > /tmp/test_exiterr.txt 2>&1
grep -q "127" /tmp/test_exiterr.txt
test_result $? "Exit status after command not found"

echo -e 'cat nonexistent\necho $?' | ./minishell > /tmp/test_exitfile.txt 2>&1
grep -q "1" /tmp/test_exitfile.txt
test_result $? "Exit status after file error"

echo -e 'ls | grep test\necho $?' | ./minishell > /tmp/test_exitpipe.txt 2>&1
[ -s /tmp/test_exitpipe.txt ]
test_result $? "Exit status after pipe"

if [ $QUICK_MODE -eq 0 ]; then
    echo -e '/bin/false\necho $?' | ./minishell > /tmp/test_exitfalse.txt 2>&1
    grep -q "1" /tmp/test_exitfalse.txt
    test_result $? "Exit status with /bin/false"

    echo -e '/bin/true\necho $?' | ./minishell > /tmp/test_exittrue.txt 2>&1
    grep -q "0" /tmp/test_exittrue.txt
    test_result $? "Exit status with /bin/true"
fi

# ============================================================================
# 12. SYNTAX ERRORS
# ============================================================================
echo -e "\n${BLUE}[12/13] Testing Syntax Errors...${NC}"

echo "| ls" | ./minishell 2>&1 | grep -q -e "syntax error" -e "parse error"
test_result $? "Pipe at beginning"

echo "ls |" | ./minishell 2>&1 | grep -q -e "syntax error" -e "parse error"
test_result $? "Pipe at end"

echo "ls | | cat" | ./minishell 2>&1 | grep -q -e "syntax error" -e "parse error"
test_result $? "Double pipe"

echo "ls >" | ./minishell 2>&1 | grep -q -e "syntax error" -e "parse error"
test_result $? "Redirection without file"

echo "< >" | ./minishell 2>&1 | grep -q -e "syntax error" -e "parse error"
test_result $? "Empty redirections"

echo ">>" | ./minishell 2>&1 | grep -q -e "syntax error" -e "parse error"
test_result $? "Append without file"

if [ $QUICK_MODE -eq 0 ]; then
    timeout 2 bash -c "echo \"echo 'unclosed\" | ./minishell" > /tmp/test_unclosed_sq.txt 2>&1
    [ $? -eq 124 ] || grep -q -e "syntax" -e "error" -e ">" /tmp/test_unclosed_sq.txt
    test_result $? "Unclosed single quote"

    timeout 2 bash -c 'echo "echo \"unclosed" | ./minishell' > /tmp/test_unclosed_dq.txt 2>&1
    [ $? -eq 124 ] || grep -q -e "syntax" -e "error" -e ">" /tmp/test_unclosed_dq.txt
    test_result $? "Unclosed double quote"
fi

# ============================================================================
# 13. EDGE CASES & STRESS TESTS
# ============================================================================
echo -e "\n${BLUE}[13/13] Testing Edge Cases & Stress Tests...${NC}"

echo "echo    test    spaces" | ./minishell > /tmp/test_spaces.txt 2>&1
grep -q "test spaces" /tmp/test_spaces.txt
test_result $? "Multiple spaces"

echo "	echo	test	tabs" | ./minishell > /tmp/test_tabs.txt 2>&1
grep -q "test tabs" /tmp/test_tabs.txt
test_result $? "Tabs instead of spaces"

echo "" | ./minishell > /tmp/test_empty.txt 2>&1
test_result $? "Empty command"

echo "   " | ./minishell > /tmp/test_spaces_only.txt 2>&1
test_result $? "Spaces only"

echo "echo test" | PATH="" ./minishell > /tmp/test_nopath.txt 2>&1
grep -q "test" /tmp/test_nopath.txt
test_result $? "Builtin without PATH"

if [ $QUICK_MODE -eq 0 ]; then
    # Long command (stress test)
    LONG_CMD="echo "
    for i in {1..100}; do
        LONG_CMD="$LONG_CMD arg$i"
    done
    echo "$LONG_CMD" | ./minishell > /tmp/test_long.txt 2>&1
    [ -s /tmp/test_long.txt ]
    test_result $? "Very long command (100 args)"

    # Many pipes (stress test)
    echo "echo test | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat" | ./minishell > /tmp/test_manypipes.txt 2>&1
    grep -q "test" /tmp/test_manypipes.txt
    test_result $? "Many pipes (10 stages)"

    # Special characters
    echo "echo 'test@#\$%^&*()'" | ./minishell > /tmp/test_special.txt 2>&1
    grep -q "test@" /tmp/test_special.txt
    test_result $? "Special characters in quotes"

    # Newlines and semicolons (if supported)
    echo "echo line1; echo line2" | ./minishell > /tmp/test_semi.txt 2>&1
    [ -s /tmp/test_semi.txt ]
    test_result $? "Semicolon separator (if supported)"
fi

# ============================================================================
# 14. MEMORY LEAKS (VALGRIND)
# ============================================================================
if [ $VALGRIND_MODE -eq 1 ]; then
    echo -e "\n${BLUE}[14/14] Testing Memory Leaks (Valgrind)...${NC}"

    echo "ls" | valgrind --leak-check=full --show-leak-kinds=definite,possible --error-exitcode=1 ./minishell > /dev/null 2>&1
    test_result $? "Simple command - no leaks"

    echo "ls | grep src" | valgrind --leak-check=full --show-leak-kinds=definite,possible --error-exitcode=1 ./minishell > /dev/null 2>&1
    test_result $? "Pipe - no leaks"

    echo "cat < Makefile > /tmp/test_valgrind.txt" | valgrind --leak-check=full --show-leak-kinds=definite,possible --error-exitcode=1 ./minishell > /dev/null 2>&1
    test_result $? "Redirections - no leaks"

    echo 'echo $HOME' | valgrind --leak-check=full --show-leak-kinds=definite,possible --error-exitcode=1 ./minishell > /dev/null 2>&1
    test_result $? "Variables - no leaks"

    echo "commandnotfound" | valgrind --leak-check=full --show-leak-kinds=definite,possible --error-exitcode=1 ./minishell > /dev/null 2>&1
    test_result $? "Command not found - no leaks"
fi

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
