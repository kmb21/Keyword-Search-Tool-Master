#!/usr/bin/env python3

import os
import subprocess
import sys

def page_symbol(page_number):
    return "\n$$@@$$PAGE: {}\n".format(page_number)

def main():
    # Get command-line arguments
    if len(sys.argv) != 3:
        print("You must provide exactly two arguments: the input filename (PDF) and the output")
        print("filename (text).")
        sys.exit(1)
    # Do some eager validation to produce nice error messages
    if not os.path.exists(sys.argv[1]):
        print("The provided input file {} does not exist.".format(sys.argv[1]))
        sys.exit(1)
    try:
        with open(sys.argv[2], 'w') as f: pass
    except:
        print("Cannot open file {} for writing.".format(sys.argv[2]))
        sys.exit(1)
    os.remove(sys.argv[2])
    # Convert the PDF file to a text document
    try:
        subprocess.check_call(["pdftotext", sys.argv[1], sys.argv[2]])
    except subprocess.CalledProcessError as e:
        print("Cannot convert input file {} to text; is it a PDF file?".format(sys.argv[1]))
        sys.exit(1)
    # Begin post-processing of text
    with open(sys.argv[2]) as f:
        contents = f.read()
    # Decapitalize, remove non-ASCII characters, and convert form feeds to page
    # number signals
    page_number = 0
    page_symbol_pending = True
    with open(sys.argv[2], 'w') as f:
        for c in contents:
            if c in 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_ \n\t':
                # If we've started a new page and haven't written a page symbol
                # yet, do it now before we write anything else.
                if page_symbol_pending:
                    page_symbol_pending = False
                    page_number += 1
                    f.write(page_symbol(page_number))
                # Now write the character to the page.
                c = c.lower()
                f.write(c)
            elif c == '\f':
                if page_symbol_pending:
                    # That page was blank, but let's not forget the page symbol
                    # we haven't written yet.
                    page_number += 1
                    f.write(page_symbol(page_number))
                else:
                    # Don't write the page symbol yet; there's a form feed at
                    # the bottom of the document.  Instead, wait for the first
                    # character on the page.
                    page_symbol_pending = True
            else:
                # Probably a smart quote or something similar.  Delete it.
                pass

main()
