'''
CIS 122 Winter 2020 Lab 7
Author: Ryan Heise
Partner: None
Description: Check If Word Is In The File Or Not
'''

"""# Loop until nothing entered
while True:
   fin = open("/Users/ryanheise/Documents/122/Assignments/cis122-assign07/words_alpha.txt") # Open file
   entry = input("Enter a search word (blank to exit): ")
   entry = entry.strip() # Remove leading/trailing
   is_found = True
   if len(entry) == 0:
      # Exit if nothing entered
      break
   else:
      for line in fin:
         line = line.strip()
         if entry.upper() == line.upper():
            is_found = True
            break
      fin.close()
   
   if is_found == False: 
      print("Word " + entry + " found")
   else:
      print("Word " + entry + " not found")
"""


# Loop until nothing entered
while True:
   # Prompt for input
   word = input("Enter a search word (blank to exit): ")

   # Remove leading/trailing
   word = word.strip() # Remove leading/trailing
   if len(word) == 0:
      # Exit if nothing entered
      break
   else:
      # Perform search

      # Open file
      fin = open("/Users/ryanheise/Documents/122/Assignments/cis122-assign07/words_alpha.txt")

      # Search for word by looping over file

      # Initialize default search result to False
      word_found = False 
      for line in fin:

         # Remove file line leading/trailing white space and line endings
         line = line.strip()

         # Test for match changing to same case
         if word.upper() == line.upper():

            # Set result status and exit loop
            word_found = True
            break

   # Close file
   fin.close()

   # Print results
   if word_found:
      print("Word " + word + " found")
   else:
      print("Word " + word + " not found")