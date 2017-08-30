; Example file for errors for maman 14
;   (mostly around the first passage)
; ------------------------------------

; 1. this line is ok
.extern DATA4

; 2. Error: invalid destination operand (immediate)
add  #5,#6

; 3. this line is ok (immediate target operand allowed for cmp)
    Next:   cmp  #5,#6

; 4. Error: invalid destination operand (immediate)
inc  #50

; 5. Error: invalid word (and)
and  r1,r2

; 6. Error: invalid word (jSr - instructions are case sensitive)
jSr  Start

; 7. this line is ok (r9 is a label declared later)
add  r1,r9

; 8. Error: invalid commas (,r2)
cmp  r1,,r2

; 9. Error: invalid word (,r3)
add  #5,r1,r3

; 10. Error: invalid destination operand (123)
inc  123

; 11. Error: invalid source operand (xyz)
cmp  #xyz,r1

; 12. Error: label previously declared
Next:   clr  r2

; 13. Error: invalid source operand (register)
lea  r3,r1

; 14. Error: invalid source operand (immediate)
lea  #3,r5

; 15. Error: invalid destination operand is not expected (Start)
stop Start

; 16. Error: invalid data parameter (400)
    .data   200 400

; 17. this line is ok (arbitrary whitespace between operands)
DATA1:  .data   100,   200 ,  -500, 300

; 18. Error: invalid commas (,3)
    .data   1, ,3

; 19. Error: invalid commas (,)
    .data   4,

; 20. Error: invalid data parameter (#123)
.data   #123

; 21. Error: invalid data parameter (.4)
    .data   12.4

; 22. Error: invalid data parameter (-5)
    .data   --5

; 23. this line is ok (case sensitive labels)
Data1:  .data   100, +200, -300

; 24. Error: invalid label (cannot be an instruction)
mov:    .data   5

; 25. Error: invalid label (cannot be a register)
r1:     .data   200,300

; 26. Error: label previously declared
DATA1:  .data   300

; 27. invalid label(non-alphabetic first character)
1DATA:  .data   300

; 28. this line is ok (r9 is not the name of a register)
r9:     .data   200

; 29. Error: duplicate label declaration (DATA4)
.extern DATA4

; 30. this line is ok (label Y declaration ignored)
Y:      .extern DATA8

; 31. this line is ok (STR1 is declared later)
.entry STR1

; 32. Error: Invalid word (DATA2 is not a label declaration)
DATA2   .data   4

; 33. Error: Invalid word (.data is case sensitivity)
.DATA   5

; 34. This line is ok (it is ok to designate the same entry more than once)
.entry  STR1

; 35. Error: invalid string parameter (blabla is not a string)
.string blabla

; 36. Error: invalid string parameter (blabla is redundent)
    .string "abcdefg" blabla

; 37. this line is ok (comma within string is not a separator)
STR1:   .string "abc, ,defg"

; 38. Error: invalid label (too long)
SuperCalifragilisticExpiAlidocious: .data	4

; 39. Error: data missing argument
    .data

; 40. Error: expected label
    .entry

; 41. Error: invalid word (.invalid)
    .invalid 85,90

; 42. this line is ok
stop