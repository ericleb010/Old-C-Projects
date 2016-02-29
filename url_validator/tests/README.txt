Here is a summary of the tests listed herein:

Tests 1 and 2 verify that the command line is functional:
	
	TEST 1: No input.
	TEST 2: Help request.

Tests 3 to 5 verify that the program handles all relevant protocols.

	TEST 3: HTTP protocol, no recursion.
	TEST 4: FTP protocol, no recursion.
	TEST 5: FILE protocol, no recursion.

Tests 6 and 7 check the URL validation process.

	TEST 6: URL which doesn't exist.
	TEST 7: URL which is handled by server (e.g. HTTP 404)

The remaining tests verify the recursive functionality.

	TEST 8.1: HTTP protocol, depth 1, single qualified URL.
	TEST 8.2: HTTP protocol, depth 1, single unqualifed URL.
	TEST 9: FILE protocol, depth 1, single qualified URL.
	TEST 10: FILE protocol, depth 1, single unqualified URL.
	TEST 11.1: Unqualified URLs do not require leading '/'.
	TEST 11.2: Unqualified URLs and moving up directories.
	TEST 12: Doesn't recurse into other domains.
	TEST 13: Doesn't recurse into PHP.
	TEST 14: Doesn't recurse into PDF.
	TEST 15: Doesn't recurse into URLs we've checked already.
	TEST 16: External URL in a file.
