def est_tax(income, exemptions):    			
    '''												
    Generates an estimate for federal income tax.
    Prints result.
    Example from class.

    >>> est_tax(20000, 1)
    1870.0      
    '''    									 
    # Set values needed to generate estimate    	 
    STD_EXEMPT = 4150    					 
    STD_DEDUCT = 6500    
    TAX_RATE = .20     

    # Calculate federal tax by adjusting    
    # reported income and applying tax rate    
    taxable_income = income - STD_DEDUCT    	
    exempt_adjust = STD_EXEMPT * exemptions
    taxable_income = taxable_income - exempt_adjust    
    estimated_tax = taxable_income * TAX_RATE              

    print('Estimated tax is:', estimated_tax)
    
    return

est_tax(20000,1)
