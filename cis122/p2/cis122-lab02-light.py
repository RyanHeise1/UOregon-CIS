'''
CIS 122 Winter 2020 Lab 2
Author: Ryan Heise
Partner: None
Description: Calculate the average amount of time in seconds light will take to reach the surface of the Earth and Pluto. 
'''

# The speed of light is 186,282 miles per second (mps) in a vacuum.
# The average distance of the sun from the Earth is 93,000,000 miles.
# The average distance of the sun from Pluto is 3,670,050,000 miles. 


import math

earth_d = 93000000
pluto_d = 3670050000

# Use a fruitful function to calculate and return the average time light takes to travel
def avg_light_travel_seconds(distance_miles):
# Output your solution using the round() function to set the number of decimal places to 2
    time_second = round(distance_miles / 186282, 2)
    return time_second

# Use a void function print_results(planetary_object, avg_light_travel_seconds) to output the results
def print_results(planetary_object, avg_light_travel_seconds):
     print("Light travels from the sun to",planetary_object, "on an average of",avg_light_travel_seconds, "seconds")

print()
time_earth = avg_light_travel_seconds(earth_d)
print_results("earth", time_earth)
time_pluto = avg_light_travel_seconds(pluto_d)
print_results("pluto",time_pluto)
print()
