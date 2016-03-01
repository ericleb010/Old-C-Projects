
"HTML Link Validation Tool"
Submitted by Eric Leblanc

This Python script allows a user to check a URL for any broken links.
The script will print the page checked and whether or not that page
was accessed successfully or not. A Python interpreter is required 
for running, and it must be able to interpret Python 2.4.3.

----------------------------------------------------------------------
		linkCheck [options] <URL>

   [-d] <#>      - changes the default depth to which the program will
	           go into the link hierarchy. Default is two links.
   [-o] <file>   - changes the output stream to point to a FILE object.
   [-h]/[--help] - displays help for usage.
----------------------------------------------------------------------

For example, if the user enters
	
	$ linkCheck http://mta.ca

the program will process the source code at 'http://mta.ca/' and  
print out any data associated with <a href> tags. Then it will
recursively attempt to process those links and repeat until the 
requested depth has been reached. The URL is required. 

The program will NOT recurse into a page if it has already been 
checked. It will also purposefully avoid .pdf, .php, and .exe files
due to their conflicts with the internal parser.

If the user is looking to change the depth to which the program
will run, 

	$ linkCheck -d 3 http://mta.ca

will, for example, initiate the program to recurse up to four layers
in depth. This means that it will take 'http://mta.ca', print and attempt
to follow each of the links it finds there, and do that twice more. The
results of these tests (including errors) can be redirected to the output 
FILE object of the user's choice, by using

	$ linkCheck -o ./dir/hello.txt http://mta.ca

There is no particular order in which the options need to be specified,
although the '-h' option will terminate the program and interrupt any 
further option interpretation. As for the URL argument, it is required
that it is specified at the end of the command.
