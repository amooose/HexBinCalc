# BasicCalculator
![image](https://user-images.githubusercontent.com/65414576/154794970-1ce1c7ce-f436-4796-94de-1bd62e1df459.png)

It uses the Shunting Yard Algorithm to convert the input infix expression to postfix notation and then evaluates it. It can also output the postfix representation of an expression.

# Examples of valid input #

- `"112+22*(2*(22-53^2*(2))-7/(7/2-1/4))" = -246159.38462`
- `"2+-1" = 1`
- `"2^0.5" = 1.413`
- `"7^7^(2/(4-532)^3)" = 7.0`
- `"2(2)"`  (=`"2*(2)"`)


# Examples of invalid input #

- `"1---1"`
- `"2*"`
- `"7 ^ 7 ^ (2/(4-532)^3)"` (No spaces are allowed when inputting directly from console)


# Current limitations #
- Range of numbers is limited by `double` data type.
- No support for trigonometric and logarithmic expressions.
- No support for expressions containing irrational numbers (eg pi, e)
- Input is not validated so all invalid inputs will cause program to crash.
