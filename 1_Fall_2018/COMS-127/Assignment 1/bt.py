# Alexander Harms
# Functions for convert to and from balanced ternary

def negate(n):
    '''
    Simply iterates through the string using a dictionary 
    to build a new string of the opposite values.
    '''
    neg = ""
    reverse = {"+" : "-", "-" : "+", "0" : "0"}
    if is_valid_balanced_ternary(n):
        for c in n:
            neg += reverse[c]
    return neg

def is_valid_balanced_ternary(n):
    '''
    Iterates through the input comparing the values to 
    valid digits as soon as it hits a bad one returns False.
    '''
    validity = False
    if type(n) == type(str()):
        validity = True
        if n == "":
            validity = False
        for c in n:
            if c != "+" and c != "-" and c != "0":
                validity = False
    return validity

def convert_to_decimal(n):
    '''
    Checks validity,
    Iterates through the string,
    The exponent is figured out and stored,
    Final value is updated on each loop with the calculated value times three to the exponent.
    '''
    dec = 0
    values = {"0" : 0, "+" : 1, "-" : -1}
    if is_valid_balanced_ternary(n):
        for l in range(0, len(n)):
            exponent = (len(n)-(l+1))
            dec += values[n[l]]*(3**exponent)
    else:
        print("ERROR! Not valid balanced ternary!")
    return dec
    
def convert_to_balanced_ternary(n):
    '''
    First checks to see if the input is an interger,
    Second checks to see if its negative and if so inverses it,
    Third loops to convert and build the converted string,
    Fourth negates the string if necessary,
    Finally return string.
    '''
    isneg = False
    bt = "0"
    values = {0 : "0", 1 : "+", 2 : "-"}
    if type(n) != type(int()):
        bt = ""
    elif n != 0:
        bt = ""
        if n < 0:
            isneg = True
            n *= -1
        while n > 0:
            bt = values[n % 3] + bt
            if n % 3 == 2:
                n += 1
            n //= 3
        if isneg:
            bt = negate(bt)
    return bt