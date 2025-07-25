# Complier
Implement multiple phases of compiler for custom language c--

```cpp
def intnumPrint(int num, int length)
{
    int i, j, first, temp;
    print "enter number";
    read i;
    i <- length;
    while (i > 0) {
        first <- 0;
        j <- 1;
        while (j <= i) {
            print j;
            j <- j + 1;
        }
        i <- i - 1;
    }
    ret i;
}
```
# The language contains the following elements:
Data Types: int	char <br />
Keywords: def ifelse while ret print read <br />
Arithmetic operators: 	+	- 	* 	/ <br />
Relational operators: 	<	<= 	>	>= 	== 	<> <br />
Line comments: start the line with # <br />
Multi line comments: ## enclose commentin ## <br />
Identifier: a letter followed by any number of letters or digits <br />
Numeric constants: only integers <br />
Literal constants: a letter enclosed in single quotes <br />
Parenthesis, braces, square brackets <br />
Assignment operator: <- <br />
Semi colon: ; <br />
Comma: ,
