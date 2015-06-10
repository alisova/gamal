%module bint
%{
/* Includes the header in the wrapper code */
#include "bint.h"
%}

%include "std_vector.i"
%include "std_string.i"

%rename(st) operator string;
%rename(_in) operator >>;
%rename(_out) operator <<;

%nestedworkaround bint::divzero;

/* Parse the header file to generate wrappers */
%include "bint.h"