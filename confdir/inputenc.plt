%%% -*- latex -*-    
\ProvidesPackage{inputenc}[2008/03/19 v1.2 Input encoding file]
%% $Id: inputenc.plt,v 2.7 2008/03/19 17:54:46 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)


%% Documentation at end of file

\def\IeC{%
  \ifx\protect\@typeset@protect
    \expandafter\@firstofone
  \else
    \noexpand\IeC
  \fi
}

\newtoks\inpenc@prehook
\newtoks\inpenc@posthook
\def\inputencoding#1{%
  \the\inpenc@prehook 
  \edef\inputencodingname{#1}%
  \input@encoding=\encoding@value{\inputencodingname}%
  \the\inpenc@posthook
}



\let\inputencodingname\relax
\newif\if@ie@needed \@ie@neededfalse
\newif\if@ie@needed@simple \@ie@needed@simplefalse
\def\encoding@value#1{\csname ie@#1\endcsname}
\def\encoding@def#1{\expandafter\def\csname ie@#1\endcsname}

\def\ie@newencoding#1#2{\encoding@def{#1}{#2}}
\def\ie@use#1{\def\inputencodingname{#1}}
\def\io@enc{2}
\def\DeclareInputText#1#2{\input@encoding@val\io@enc\space #1=#2\relax}

\ie@newencoding{utf8}{0}
\ie@newencoding{latin1}{1}
\ie@newencoding{ascii}{2}
\ie@newencoding{ansinew}{3}
\ie@newencoding{applemac}{4}
\ie@newencoding{cp1250}{5}
\ie@newencoding{cp1252}{6}
\ie@newencoding{cp1257}{7}
\ie@newencoding{cp437}{8}
\ie@newencoding{cp437de}{9}
\ie@newencoding{cp865}{10}
\ie@newencoding{cp850}{11}
\ie@newencoding{cp852}{12}
\ie@newencoding{cp858}{13}
\ie@newencoding{decmulti}{14}
\ie@newencoding{latin10}{15}
\ie@newencoding{latin2}{16}
\ie@newencoding{latin3}{17}
\ie@newencoding{latin4}{18}
\ie@newencoding{latin5}{19}
\ie@newencoding{latin9}{20}
\ie@newencoding{macce}{21}
\ie@newencoding{next}{22}


\DeclareOption{utf8}{\ie@use{utf8}}
\DeclareOption{utf8x}{\ie@use{utf8}}
\DeclareOption{latin1}{\ie@use{latin1}}
\DeclareOption{ascii}{\ie@use{ascii}}
\DeclareOption{ansinew}{\@ie@neededtrue\ie@use{ansinew}}
\DeclareOption{applemac}{\@ie@neededtrue\ie@use{applemac}}
\DeclareOption{cp1250}{\@ie@neededtrue\ie@use{cp1250}}
\DeclareOption{cp1252}{\@ie@neededtrue\ie@use{cp1252}}
\DeclareOption{cp1257}{\@ie@neededtrue\ie@use{cp1257}}
\DeclareOption{cp437}{\@ie@neededtrue\ie@use{cp437}}
\DeclareOption{cp437de}{\@ie@neededtrue\ie@use{cp437de}}
\DeclareOption{cp865}{\@ie@neededtrue\ie@use{cp865}}
\DeclareOption{cp850}{\@ie@neededtrue\ie@use{cp850}}
\DeclareOption{cp852}{\@ie@neededtrue\ie@use{cp852}}
\DeclareOption{cp858}{\@ie@neededtrue\ie@use{cp858}}
\DeclareOption{decmulti}{\@ie@neededtrue\ie@use{decmulti}}
\DeclareOption{latin10}{\@ie@neededtrue\ie@use{latin10}}
\DeclareOption{latin2}{\@ie@neededtrue\ie@use{latin2}}
\DeclareOption{latin3}{\@ie@neededtrue\ie@use{latin3}}
\DeclareOption{latin4}{\@ie@neededtrue\ie@use{latin4}}
\DeclareOption{latin5}{\@ie@neededtrue\ie@use{latin5}}
\DeclareOption{latin9}{\ie@use{latin9}}
\DeclareOption{macce}{\@ie@neededtrue\ie@use{macce}}
\DeclareOption{next}{\@ie@neededtrue\ie@use{next}}


\ProcessOptions \relax

\ifx\inputencodingname\relax\else
  \input@encoding@default\encoding@value{\inputencodingname}%
  \edef\@tempa{%
  \AtBeginDocument{\noexpand\inputencoding{\inputencodingname}}}
  \@tempa
  \let\inputencodingname\relax
\fi


% latin9 iso-8859-15
\edef\io@enc{\encoding@value{latin9}}
\DeclareInputText{164}{"20AC}
\DeclareInputText{166}{"160}
\DeclareInputText{168}{"161}
\DeclareInputText{180}{"17D}
\DeclareInputText{184}{"17E}
\DeclareInputText{188}{"152}
\DeclareInputText{189}{"153}
\DeclareInputText{190}{"178}



\if@ie@needed\else\endinput\fi


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% ansi new
\input@encoding@val\ie@ansinew -32 128
"20AC 0 "201A "192 "201E "2026 "2020 "2021
"2038 "2030 "160 "2039 "152  0 "17D 0
0 "2018 "2019 "201C "201D "2022 "2013 "2014
"2F7 "2122 "161 "203A "153 0 "17E "178

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\input@encoding@val\ie@applemac -128 128
196 197 199 201 209 214 220 225
224 226 228 227 229 231 233 232
234 235 237 236 238 239 241 243
242 244 246 245 250 249 251 252
"2020 176 162 163 167 "2022 182 223
174 169 "2122 180 168 "2260 198 216
"221E 177 "2264 "2265 165 181 "2202 "3A3
"3A0 "3C0 "222B 170 186 "3A9 230 248
191 161 172 "221A "192 "2248 "394 171
187 "2026 160 192 195 213 "152 "153
"2013 "2014 "201C "201D "2018 "2019 247 "22C4
255 "178 47 164 "2039 "203A "FB01 "FB02
"2021 183 "201A "201E "2030 194 202 193
203 200 205 206 207 204 211 212
"F8FF 210 218 219 217 "131 "2038 "2F7
175 "306 "307 "30A 184 "30B "328 "30C


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\input@encoding@val \encoding@value{cp1250} -128 128
"20AC 0     "201A 0     "201E "2026 "2020 "2021
0     "2030 "160  "2039 "15A  "164  "17D  "179
0     "2018 "2019 "201C "201D "2022 "2013 "2014
0     "2122 "161  "203A "15B  "165  "17E  "17A
160   "30C  "30   "141  164   "1CD  166    167 
168   169   "15E  171   172   173   174    "17B
176   177   "328  "142  180   181   182    283
184   "105  "15F  187   "13D  "30B  "13E   "17C
"154  193   194   "102  196   "139  "106   199   
"10C  201   "118  203   "11A  205   206    "10E
"110  "143  "147  211   212   "150  214    215
"158  "16E  218   "170  220   221   "162   223
"155  225   226   "103  228   "13A  "107   231
"10D  233   "119  235   "11B  237   238    "10F
"111  "144  "148  243   244   "151  246    247
"159  "16F  250   "171  252   253   "163   "307


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


\input@encoding@val \encoding@value{cp1252} -32 128
"20AC 0 "201A "192 "201E "2026 "2020 "2021
"2038 "2030 "160 "2039 "152 0 "17D 0 
0 "2018 "2019 "201C "201D "2022 "2013 "2014
"2F7 "2122 "161 "203A "153 157 "17E "178


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\input@encoding@val \encoding@value{cp1257} -128 128
"20AC 0 "201A 0 "201E "2026 "2020 "2021
0 "2030 0 "2039 0 168 "30C 184
0 "2018 "2019 "201C "201D "2022 "2013 "2014 
0 "2122 0 "203A 0 175 "328 0
160 161 162 163 164 0 166 167
216 169 "156 171 172 173 174 198
176 177 178 179 180 181 182 183
248 185 "157 187 188 189 190  230
"104 "12E "100 "106 196 197 "118 "112
"10C 201 "179 "116 "122 "136 "12A "13B
"160 "143 "145 211 "14C 213 214 215
"172 "141 "15A "16A 220 "17B "17D 223
"105 "12F "101 "107 228 229 "119 "113
"10D 233 "17A "117 "123 "137 "12B "13C
"161 "144 "146 243 "14D 245 246 247
"173 "142 "15B "16B 252 "17C "17E "307

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


\input@encoding@val \encoding@value{cp437} -128 128
199 252 233 226 228 224 229 231
234 235 232 239 238 236 196 197
201 230 198 244 246 242 251 249
255 214 220 162 163 165 "2A07 "192
225 237 243 250 241 209 170 186 
191 0   172 189 188 161 171 187
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
"3B1 "3B2 "393 "3C0 "3A3 "3C3 181 "3B3
"3A6 "3B8 "3A9 "394 "221E "3D5 "3B5 "2229
"2261 177 "2265 "2264 244 245 247 "2248
176 183 "2022 "221A "207F 178 "25AA 160

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\input@encoding@val \encoding@value{cp437de} -128 128
199 252 233 226 228 224 229 231
234 235 232 239 238 236 196 197
201 230 198 244 246 242 251 249
255 214 220 162 163 165 "2A07 "192
225 237 243 250 241 209 170 186 
191 0   172 189 188 161 171 187
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
"3B1 223 "393 "3C0 "3A3 "3C3 181 "3B3
"3A6 "3B8 "3A9 "394 "221E "3D5 "3B5 "2229
"2261 177 "2265 "2264 244 245 247 "2248
176 183 "2022 "221A "207F 178 "25AA 160

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\input@encoding@val \encoding@value{cp865} -128 128
199 252 233 226 228 224 229 231
234 235 232 239 238 236 196 197
201 230 198 244 246 242 251 249
255 214 220 248 163 216 "2A07 "192
225 237 243 250 241 209 170 186 
191 0   172 189 188 161 171 164
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
"3B1 "3B2 "393 "3C0 "3A3 "3C3 181 "3B3
"3A6 "3B8 "3A9 "394 "221E "3D5 "3B5 "2229
"2261 177 "2265 "2264 244 245 247 "2248
176 183 "2022 "221A "207F 178 "25AA 160

%\showthe\input@encoding@val \encoding@value{cp865} 175
%\showthe\input@encoding@val \encoding@value{cp865} 255

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\input@encoding@val \encoding@value{cp850} -128 128
199 252 233 226 228 224 229 231
234 235 232 239 238 236 196 197
201 230 198 244 246 242 251 249
255 214 220 248 163 216 215 "192
225 237 243 250 241 209 170 186
191 174 172 189 188 161 171 187
0 0 0 0 0 193 194 192
169 0 0 0 0 162 165 0
0 0 0 0 0 0 227 195
0 0 0 0 0 0 0 164
240 208 202 203 200 "131 205 206
207 0 0 0 0 166 204 0
211 223 212 210 245 213 181 254
222 218 219 217 253 221 175 180
173 177 0   189 182 167 247 184
176 168 183 185 179 178 "25AA 160

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\input@encoding@val \encoding@value{cp858} -128 128
199 252 233 226 228 224 229 231
234 235 232 239 238 236 196 197
201 230 198 244 246 242 251 249
255 214 220 248 163 216 215 "192
225 237 243 250 241 209 170 186
191 174 172 189 188 161 171 187
0 0 0 0 0 193 194 192
169 0 0 0 0 162 165 0
0 0 0 0 0 0 227 195
0 0 0 0 0 0 0 164
240 208 202 203 200 "20AC 205 206
207 0 0 0 0 166 204 0
211 223 212 210 245 213 181 254
222 218 219 217 253 221 175 180
173 177 0   189 182 167 247 184
176 168 183 185 179 178 "25AA 160

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\input@encoding@val \encoding@value{cp858} -128 128
199 252 233 226 228 "16F "107 231
"142 235 "150 "151 238 "179 196 106 
201 "139 "13A 244 246 "13D "13E "15A
"15B 214 220 "164 "165 "141 215 "10D
225 237 243 250 "104 "105 "17D "17E
"118 "119 172 "17A "10C "15F 171 187
0 0 0 0 0 193 194 "11A
"15E 0 0 0 0 "17B "17C 0
0 0 0 0 0 0 "102 "103
0 0 0 0 0 0 0 164
"111 "110 "10E 203 "10F "147 205 206
"11B 0 0 0 0 "162 "16E 0
211 223 212 "143 "144 "148 "160 "161
"154 218 "155 "170 253 221 "163 180
173 "30B "328 "30C "306 167 247 184
176 168 "307 "171 "158 "159 "25AA 160



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\input@encoding@val \encoding@value{latin10} -96 160
160 "104 "105 "141 "20AC "201E "160 167
"161 169 "218 171 "179 173 "17A "17B 
176 177 "10C "142 "17D "201D 182 183
"17E "10D "219 187 "152 "153 "178 "17C
192 193 194 "102 196 "106 198 199
200 201 202 203 204 205 206 207
"110 "143 210 211 212 "150 214 "15A
"170 217 218 219 220 "118 "21A 223
224 225 226 "103 228 "107 230 231
232 233 234 235 236 237 238 239
"111 "144 242 243 244 "151 246 "15B
"171 249 250 251 252 "119 "21B 255



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% latin2
\input@encoding@val \encoding@value{latin2} -96 160
160 "104 "306 "141 164 "13D "15A 167 
168 "160 "15E "164 "179 173 "17D "17B
176 "105 "328 "142 180 "13E "15B "30C
184 "161 "15F "165 "17A "30B "17E "17C
"154 193 194 "102 196 "139 "106 199
"10C 201 "118 203 "11A 205 206 "10E
"110 "143 "147 211 212 "150 214 215
"158 "16E 218 "170 220 221 "162 223
"155 225 226 "103 228 "13A "107 231
"10D 233 "119 235 "11B 237 238 "10F
"111 "144 "148 243 244 "151 246 247
"159 "16F 250 "171 252 253 "163 "307


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\input@encoding@val \encoding@value{latin3} -96 160
160 "126 "306 163 164 165 "124 167
168 "130 "15E "11E "134 173 174 "17B
176 "127 178 179 180 181 "125 183 
184 "131 "15F "11F "135 189 190 "17C
192 193 194 195 196 "10A "108 199
200 201 202 203 204 205 206 207
208 209 210 211 212 "120 214 215
"11C 217 218 219 220 "16C "15C 223
224 225 226 227 228 "10B "109 231
232 233 234 235 236 237 238 239
240 241 242 243 244 "121 246 247
"11D 249 250 251 252 "16D "15D "307

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% encoding table for  latin4

\input@encoding@val \encoding@value{latin4} -96 160
160  "104 "312 "156 164 "128 "13B 167
168 "160 "112 "122 "166 173 "17D 175
176 "105 "328 "157 180 "129 "13C "30C
184 "161 "113 "123 "167 "14A "17E "14B
"100 193 194 195 196 197 198 "12E
"10C 201 "118 203 "116 205 206 "12A
"110 "145 "14C "136 212 213 214 215
216 "172 218 219 220 "168 "16A 223
"101 225 226 227 228 229 230 "12F
"10D 233 "119 235 "117 237 238 "12B
"111 "146 "14D "137 244 245 246 247
248 "173 250 251 252 "169 "16B "307

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%latin5 ISO-8859-9
% like latin1 with some exceptions
\edef\io@enc{\encoding@value{latin5}}
\DeclareInputText{208}{"11E}
\DeclareInputText{221}{"130}
\DeclareInputText{222}{"15E}
\DeclareInputText{253}{"131}
\DeclareInputText{254}{"15F}



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% encoding table for macce

\input@encoding@val\ie@macce -128 128
196 0 0 201 "104 214 220 0
"105  "10C 228 "10D "106 "107 0 "179
"17A 0 0 0 0 0 0 243
0 244 246 0 0 0 0 252
"2020 176 "118 163 167 "2022 182 223
174 169 0 "119 0 0 0 0
0 0 0 0 0 0 0 0
"142 0 0 0 0 0 0 0
0 "143 0 0 "144 0 0 171
187 "2026 160 0 0 0 0 0
"2013 "2014 "201C "201D "2018 "2019 247 0
0 0 0 0 0 "2039 "203A "159 0
0 0 "201A "201E 0 "15A "15B 0
0 0 0 0 0 0 211 211 212
0 0 0 0 0 0 0 0 
0 0 0 "17B "141 "17C 0 0


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Econding table for next

\input@encoding@val\ie@next -128 128
160 192 193 194 195 196 197 199
200 201 202 203 204 205 206 207
208 209 210 211 212 213 214 217
218 219 252 221 222 181 215 247
169 161 162 163 47  165 "192 167
164 "2019 "201C 171 "2039 "203A "FB01 "FB02
174 "2013 "2020 "2021 183 166 182 "2022
"201A "201E "201D 187 "2026 "2030 172 191
185 "300 180 "2038 "2F7 175 "306 "307
168 178 "30A 184 179 "30B "328 "30C
"2014 177 188 189 189 224 225 226
227 228 229 231 232 233 234 235
236 198 237 170 238 239 240 241
"141 216 "152 186 242 243 244 245
246 230 249 250 251 "131 252 253
"142 248 "153 223 254 255 0 0

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\edef\io@enc{\encoding@value{decmulti}}
\DeclareInputText{215}{"152}
\DeclareInputText{221}{"178}
\DeclareInputText{247}{"153}
\DeclareInputText{253}{255}

\endinput

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%% This file uses features defined in Tralics 2.10.8
%% It provides \IeC (same code as in LaTeX)
%% It provides \inpenc@prehook and \inpenc@posthook, two hooks
%% It provides \inputencoding a command that changes the encoding
%% of the current file. Argument is a known encoding name
%% If you say \usepackage[foo,bar]{inputenc}
%% this defines foo and bar, uses bar.
%% If you chose an encoding other than : utf8, latin1, ascii, then all
%% encodings are loaded>

%% Declaration \input@encoding@default=17
%% sets default encoding to 17
%% Declaration \input@encoding=16
%% sets current encoding to 16
%% Declaration \input@encoding@val 15 32 48
%% defines for encoding 15 the character 32 to be Unicode U+30
%% Declaration \input@encoding@val 15 -3 48 100 110 120 
%% changes characters 48 49 and 50 to 100 110 and 120
%% All modifications are global. 
%% Use \the\foo in order to get the value of \foo
%% Encoding 0 is UTF8, encoding 1 is latin1; these are fixed

