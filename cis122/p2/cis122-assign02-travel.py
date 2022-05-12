'''
CIS 122 Winter 2020 assignment 2
Author: Ryan Heise
Partner: None
Description: Create two functions to help calculate the travel time in hours, minutes and seconds and to display the results.
'''

# Calculate travel time in minutes given the distance in miles and the speed in mph
def calc_travel_time( distance, speed ):
    # Calculate time
    time = distance/speed
    # convert time to seconds
    time = time * 60
    return time

# Output the travel time hours, minutes and seconds given distance and speed
def print_travel_time( distance, speed ):
    # declare that time = the distance and speed we will enter in later
    time = calc_travel_time(distance, speed)
    # change minutes to int
    minutes = int(time)
    # find seconds by multiplying t - m by 60 and round it to the 0th decimal
    seconds = int(round(60*(time - minutes), 0))
    # find hours by division that results into whole number
    hours = minutes // 60
    # find minutes by the reamander
    minutes = minutes % 60
    print("To travel", distance, "miles at",speed, "MPH will take",hours, "hr,", minutes, "min and", seconds, "sec")

print()
print_travel_time(120, 55)
print_travel_time(120, 70)
print_travel_time(5, 25)
print_travel_time(5, 35)
print()
