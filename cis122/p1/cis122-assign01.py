''' 
CIS 122 Winter 2020 Assignment 1 
Author: Ryan H. Student 
Credit: Ryan H. Oregon 
Description: Introduction to programming problem set uses Python numeric data types and 
operations to solve a variety of small problems. 
''' 

#Question 1 (semantic error)

import math
total_hats = 64                                  # 5

cost_red = 10                                    # 2

# Part B bug fix: added a "round" element that rounds decimal answers down
total_red = math.floor(total_hats / 2)           # 8    

total_red_cost = total_red * cost_red            # 1

total_blue = total_hats - total_red              # 3

cost_blue = 5                                    # 4

total_blue_cost = total_blue * cost_blue         # 6

total_cost = total_red_cost + total_blue_cost    # 7

print()
print("*** Question 1: Calculate total hat cost ***")
print()
print("Total Hat Cost:", total_cost)
print()
print()
print("*** Question 2: Calculator to find steps per min, hr, and sec ***")
daily_steps= 10000

# Displays daily steps
print()
print("Daily Steps: ", daily_steps)

# Displays the ammount of steps per second
per_sec = (daily_steps/24)*(1/60)*(1/60)
print("Per Second: ", per_sec)

# Dispans the ammount of steps per minute
per_min = (daily_steps/24)*(1/60)
print("Per Minute: ", per_min)

# Display the ammount of steps per hour
per_hr = (daily_steps/24)
print("Per Hour: ", per_hr)
print()
print()
print("*** Question 3: finding out if your friend has walked over 100 miles ***")
print()
pi = 3.14
radius = 400
area = 2 * pi * radius**2
twice_a_day = area * 2
weekly_ft = twice_a_day * 6
print("Weekly Distance (ft): ", weekly_ft)
weekly_mi = weekly_ft * 0.000189394
print("Weekly Distance (mi): ", weekly_mi)
print()
