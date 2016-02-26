This `docs` directory contains the developer documentation for the BrewPi Firmware.

It is best viewed on [brewpidev.readthedocs.org](brewpidev.readthedocs.org).


# Building the documentation locally
To build the documentation locally, you will have to install some python packages.
This is best done in an isolated virtualenv

First, create a Python virtualenv using virtualenvwrapper:

For Windows:

    pip install virtualenvwrapper-win
    
For other systems:
    
    pip install virtualenvwrapper
    

Create a new virtualenv:

    mkvirtualenv docs

Now start using the virtualenv:
    
    workon docs


Install all required dependencies:

    pip install -r requirements.txt
    
You can now build the documentation with:

    make html

## Markdown vs reStructuredText
Sphinx supports both MarkDown and reStructuredText, but because Markdown is easier to write and best supported by GitHub, we chose to write the docs in MarkDown as much as possible.


## Building the documentation in Eclipse (on windows)
If you are using Eclipse to write firmware code, it can be helpful to edit and auto-build the docs from Eclipse too.
To build the documentation on resource save in Eclipse (on Windows), set up a new build configuration and use as build command:

    <path to virtualenv>/shpinx-build.exe

And as 'build on resource save'command use:

    -d _build/doctrees -b html . _build/html
    
To auto-refresh your browser as well, you can use the Chrome plugin LivePage.