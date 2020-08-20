# dir_enumerator
A simple root directory enumerator

PLEASE DO NOT SCAN ANYTHING YOU DO NOT OWN / HAVE EXPLICIT PERMISSION TO SCAN.
This is the source for a simple directory enumerator. Usage is printed out when the binary is first run.

*Installation*

You will need a C compiler. Simply compile enum.c and run the resulting output program. Usage will be displayed (optionally output with the flag -h also).

*Testing*

I recommend you test this by starting a web server in the test directory (test_dir). This can be done easily using PythonX:

  _Python2_: python -m SimpleHTTPServer 8000
  _Python3_: python -m http.server 8000
  
After this, the scanner should target localhost:8000 by default so you can run the output binary again and see the results.
The usage contains all the information necessary to run this on other targets/ports.

PLEASE DO NOT SCAN ANYTHING YOU DO NOT OWN / HAVE EXPLICIT PERMISSION TO SCAN.
Use responsibly.
