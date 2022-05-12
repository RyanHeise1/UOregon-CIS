"""Lab coding done Feb 24 2021"""

import re

"""
Cannot start with a digit.
Should only contain one @ symbol delimiting the username from the domain.
Only accepted domains should end with '.edu' or '.com'.
"""
def validate_email(email: str) -> bool:
    if re.match(r"\D[^@]*@[^@]*((\.com)|(\.edu))$", email):
        return True
    return False


"""
Must contain at least 10 characters
Must contain at least 1 digit
Must contain at least one special symbol (non-alphanumeric)
"""
def validate_password(password: str) -> bool:
    if len(password) >= 10 and re.match(r'(.*\d.*\W.*)|(.*\W.*\d.*)', password):
        return True

def main():
    email = input("Provide an email address: ")
    while not validate_email(email):
        email = input("Invalid email, provide a different one: ")
    password = input("Input your password: ")
    while not validate_password(password):
        password = input("Invalid password, provide a different one:")
    print("Sign up successful")



if __name__ == '__main__':
    main()