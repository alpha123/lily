###
SyntaxError: A class cannot inherit from an incomplete class.
Where: File "test/fail/inherit_entered_class.ly" at line 7
###

class First(integer value) {
    class Second(integer value2) < First(value) {
# This is disabled because it makes determining parameter name clashes really,
# really harder. So...no.
    }
}
