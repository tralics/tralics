% -*- latex -*- 
\ProvidesPackage{delarray}[2007/03/28 v1.0 Array with delimiters]

%% Note: it is impossible to change the meaning of arry
% in the current version

\newenvironment{delarray}[4][]{\global\def\r@delimiter{#4}
\left#2\begin{array}{#3}}{\end{array}\right\r@delimiter}


\endinput
