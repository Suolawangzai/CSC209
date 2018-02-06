    #include <stdio.h>
    #include <stdlib.h>

    //function declarations
    int valid_chars_count (char *input, int size);
    int valid_char (char c);
    int check_is_island_part(char *island_start_pt);
    int find_island_size(int *island_start_pt, int length);

    int main (int argc, char *argv[]) {
        char * inputfile_name=argv[1]; 
        FILE * input_fp;
        char *input_buffer; //stores all bytes in the input file
        char *sequence; //char array holding the final cleaned DNA sequence
        
        int *valid_cpg_positions; //array of all positions in sequence
        int input_len, seq_len;
        
        int i, j;
        int r;

        int num_island = 0; //store number of CpG island
        int k;
        int current_index; // store the current index of the array working on
        char *current_start; // store the current pointer of the array working on
        int is_island_part; 
        int *cpg_start;
        int rest_len;
        int island_start_index;
        int island_end_index;
        int island_size;

        FILE * output_fp;
        output_fp = fopen("cpg_output.txt", "w");


        //open the file
        if ( !(input_fp= fopen ( inputfile_name , "rb" )) ) {
            printf("Could not open file \"%s\" for reading input lines \n", inputfile_name);
            return (-1);
        }
        
        //compute total number of characters in the input file
        fseek (input_fp, 0, SEEK_END);
        input_len=ftell (input_fp);
        rewind (input_fp); 
        printf ("total bytes in file: %d\n", input_len);
        
        //allocate an array to hold these characters
        input_buffer = (char*) malloc (input_len+1);
        
        //read file content into the input buffer
        r = fread (input_buffer, 1, input_len,  input_fp);
        printf ("read characters %d\n", r);
        if (r != input_len) {
            printf("Reading error \n");
            return (-1);
        }
        //add terminating character
        input_buffer [input_len] ='\0';

        //determine total number of valid DNA characters
        //and allocate array of chars to hold them
        seq_len = valid_chars_count (input_buffer, input_len);
        printf ("total characters %d total valid characters %d \n", input_len, seq_len);
        sequence = (char*) malloc (seq_len+1); 
        
        //transfer valid characters from raw buffer
        for (i=0, j=0; i < input_len; i++) {
            if (valid_char (input_buffer [i])) {
                sequence [j++] = input_buffer [i];
            }
        }
        sequence [seq_len] = '\0';
        
        //allocate int array for all the positions
        valid_cpg_positions = (int*) malloc (seq_len*sizeof(int));
        for (i=0; i<seq_len; i++)
            valid_cpg_positions[i] = 0;
            
        /* YOUR CpG ISLANDS DISCOVERY CODE HERE */
        current_index = 0;
        current_start = sequence;
        is_island_part = check_is_island_part(current_start);

        // Set valid_spg_positions to the correct value according
        // whether it belongs to CpG island
        while(is_island_part != -1){
            if(is_island_part == 1){
                // set the value stored in valid_cpg_positions to 1
                for(k = 0; k < 200; k++){
                    valid_cpg_positions[current_index + k] = 1;
                }
            }

            current_index++;
            current_start++;
            // Check the next 200 windows
            is_island_part = check_is_island_part(current_start);
        }

    current_index = 0;
    cpg_start = valid_cpg_positions;
    rest_len = seq_len;

    // Find each island by going through the array valid_cpg_positions
   while(current_index < (seq_len - 199) ){
        if(*cpg_start == 1){
            island_start_index = current_index;
            island_size = find_island_size(cpg_start,rest_len);
            island_end_index = island_start_index + island_size - 1;
            // Wirte it to the file
            fprintf(output_fp, "CpG island \t %d .. %d \t size=%d \n",
                    island_start_index, island_end_index, island_size);
            num_island++;
            cpg_start += island_size;
            current_index = island_end_index + 1;
            rest_len = seq_len - current_index;
        }
        else{
            current_index++;
            rest_len--;
            cpg_start++;
        }
    }

    
    // Write it to files
    fprintf(output_fp, "num_islands \t %d \n", num_island);
    fclose(output_fp);
    
        
        return 0;
    }

    int valid_chars_count (char *input, int size) {
        int i, count = 0;
        for (i=0; i<size; i++) {
            if (valid_char (input [i]) )
                count++;
        }
        return count;
    }

    int valid_char (char c) {
        if (c == 'a' || c == 'c' || c == 'g' || c == 't' ) 
            return 1;
        return 0;
    }

/* Added new helper functions */

/* This function recieve a pointer to the 200 window and
check whether it belongs to a CpG island. Return 0 iff it does
not belongs to the island; return 1 iff it belongs to a CpG island;
return -1 if there is not enough element in the data for the window.
*/
int check_is_island_part(char *island_start_pt){
    char *start_pt;
    start_pt = island_start_pt;
    int i = 0;
    double countC = 0.0,countG = 0.0, countCG = 0.0;
    double expected_CG;
    double CG_ratio;
    char current_char;
    char last_char = '0';
    while(i < 200 && *start_pt != '\0'){
        current_char = *start_pt;
        if(current_char == 'c'){
            countC++;
        }
        else if(current_char == 'g'){
            countG++;
            if(last_char == 'c'){
                countCG++;
            }
      

        }
        i++;
        start_pt++;
        last_char = current_char;
        
    }
    // Not enough data left: reach the end before 200
    if(i < 200 && *start_pt == '\0'){
        return -1;
    }
    
    
    expected_CG = (double)countC * countG / 200.0;
    // If expected_CG is 0 then return 0
    if(expected_CG == 0){
        return 0;
    }
    CG_ratio = countCG / expected_CG;

    if( (countC + countG ) >100 && CG_ratio >= 0.6 ){
        return 1;
    }
    return 0;
}


int find_island_size(int *island_start_pt, int length){
    int *start_pt;
    start_pt = island_start_pt;
    int size = 0;
    while(size < length && *start_pt == 1){
        start_pt++;
        size++;
    }
    return size;
}



