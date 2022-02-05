%define start 0

%macro colon 2
%%some_label: 
  dq start
  db %1, 0
%2:
%define start %%some_label
%endmacro
