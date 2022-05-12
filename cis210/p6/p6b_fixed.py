import doctest
from p5b_printcal import print_cal

def bigSalesBug(sales_list):
    '''(list) -> number

    Returns sum of all sales for amounts at or over $40,000.
    sales_list has the record of all the sales.

    >>> bigSalesBug([40_000, 45.67, 19_000.0, 25000, 100_000])
    140000.0
    >>> bigSalesBug([32,000, -99999, 92, 39999.99, 1000000000000])
    1000000000000.0
    >>> bigSalesBug([])
    0.0
    >>> bigSalesBug([0, 0, 0, 1, 0])
    0.0
    '''
    total = 0.00
    for sales in sales_list: # invalid syntax, ":" was missing
        if sales >= 40000: # amount is at or over $40,000, so ">=" is needed
            total = total + sales # variable name "total" misspelled
    return total


def findRangeBug(salesli):
    '''(list) -> tuple

    Returns largest and smallest number in non-empty salesli.
    (Note that Python has built in funcs max and min
    to do this, but not using them here, so we can
    work with the list directly.)

    >>> findRangeBug([40000, 45.67, 19000.0, 25000, 100000])
    (45.67, 100000.0)
    >>> findRangeBug([])
    Traceback (most recent call last):
    ...
    IndexError: list index out of range
    >>> findRangeBug([0, 0])
    (0.0, 0.0)
    >>> findRangeBug([-1, -9999])
    (-9999.0, -1.0)
    '''
    salesli = sorted(salesli)
    ''' .sort() method is in place and returns None
    so, we will be use sorted() method to sort the list
    '''
    low = float(salesli[0])
    high = float(salesli[-1])
    return low, high

def salesReportBug(salesli):
    '''(list) --> None

    Prints report of sales totals for each day of week (salesli)
    and range of per-day sales. salesli is non-empty - 0 sales
    for any day are reported as 0.

    ### format the expected output as expected
    >>> salesReportBug([40000, 45.67, 19000.0, 25000, 100000])
    Weekly Range: $45.67 - $100,000.00
    <BLANKLINE>
    Mon Tue Wed Thu Fri   
    $40,000.00 $45.67 $19,000.00 $25,000.00 $100,000.00
    >>> salesReportBug([])
    Traceback (most recent call last):
    ...
    IndexError: list index out of range
    >>> salesReportBug([0, 0, 0, 0, 0])
    Weekly Range: $0.00 - $0.00
    <BLANKLINE>
    Mon Tue Wed Thu Fri   
    $0.00 $0.00 $0.00 $0.00 $0.00
    '''
    #calculate and report low and high sales
    low, high = findRangeBug(salesli)
    print(f'Weekly Range: ${low:,.2f} - ${high:,.2f}\n')

    #print daily report header
    fw = 12
    print(f"{'Mon':<{fw}} {'Tue':<{fw}} {'Wed':<{fw}} {'Thu':<{fw}} {'Fri':<{fw}}")

    #report on sales per day from list data
    for sales in salesli:
        print(f'${float(sales):<{fw},.2f}', end='')
      
    return None

def test_calendar():
    print_cal(6, 2000)
    print_cal(1, 1)

if __name__ == '__main__':
    main()

#print(doctest.testmod())
