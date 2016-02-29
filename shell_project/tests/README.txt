This is the accompanying documentation for the test suite, describing what
each test validates. 

--------------------------------------------------------------------------

Test #1 and #2 validate proper start-up.

	TEST 1: No input.
	TEST 2: Whitespace as a command.

Tests #3 to #8: validate the execution of a command.

	TEST 3: Single word commands.
	TEST 4: Two-word commands and case- / numeric-sensitivity.
	TEST 5: Multiple-word commands.
	TEST 6: Multiple-word command and extra whitespace.
	TEST 7: "Quit" command.
	TEST 8: Multiple statements.

Tests #9 to #19 verify the manipulation and storage of variables.

	TEST 9: Variable assignments.
	TEST 10: Multiple assignments.
	TEST 11: Variable substitution.
	TEST 12: Multiple substitution, different lines.
	TEST 13: Multiple substitution, same line.
	TEST 14: Multiple substitution, same line, no whitespace.
	TEST 15: Substitution of a command name.
	TEST 16: ERROR: No opening bracket. 
	TEST 17: ERROR: No closing bracket.
	TEST 18: ERROR: Invalid characters in assignment / subst.
	TEST 19: ERROR: No spaces allowed.

Tests #20 to #23 verify the background functionality.

	TEST 20: Running a command in the background.
	TEST 21: Running multiple commands in the background.
	TEST 22: Running a command in the background, followed by usual.
	TEST 23: Running multiple combinations of the above.

Test #X24 to #X31 are NOT automated. They are still available for testing,
but they must be entered manually. They validate the pipe functionality.
	
	TEST X24: One pipe.
	TEST X25: Multiple pipes.
	TEST X26: One pipe after another.
	TEST X27: Piped and in the background.
	TEST X28: ERROR: Pipe syntax.
	TEST X29: ERROR: Invalid program name.
	TEST X30: ERROR: Invalid program name within a pipe.
	TEST X31: ERROR: **fails** Complicated premature exit from pipe.
