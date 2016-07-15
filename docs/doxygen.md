Doxygen
-------
We use Doxygen to automatically generate documentation of our C++ code.
We also use the doxygen generated xml in building our documentation with Sphinx.
However, when writing doxygen docstrings, it might be easier to look at the doxygen html output directly.

To run doxygen from eclipse, follow the following steps:

1. Install doxygen
2. Install eclox in eclipse
3. Restart eclipse
4. Click the doxygen button (it is a blue @)
5. Point the dialog to the Doxyfile in the docs dir

To enable autocomplete for doxygen style comments:

In Window->Preferences->C/C++->Editor, look for "Documentation tool comments", and set the workspace default to doxygen.

Now you can type /** above a function and press return.

    /**
    int foo(int bar);

becomes

    /**
     * 
     * @param bar
     * @return
     */
    int foo(int bar);
    
Note: when doxygen runs before sphinx, the _build directory might not exist causing an error. Simply run sphinx first in the docs dir. Instructions can be found in the docs dir.

