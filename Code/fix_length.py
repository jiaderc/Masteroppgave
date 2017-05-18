def fix_length(data_sequence):
    fixed_length = 125
    total_length = len(data_sequence)  
	
    #Total amount data to be removed
    total_cut = total_length-fixed_length
	
    #In case the sequence length is odd
    rest = total_cut%2

    #Calculate the start and end position
    start_pos = total_cut/2 
    end_pos = total_length - (total_cut/2)+rest 
	
    #Retrieve fixed length of sequense data 
    fixed_sequence = data_sequence[start_pos:end_pos] 
    return fixed_sequence