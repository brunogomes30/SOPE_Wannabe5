## TO DO

- [ ] send struct via public FIFO                                 
- [ ] create private FIFO                                         
- [ ] read server response                                        
- [ ] (active) wait for server to read from public FIFO           
- [ ] detect that the server closed the FIFO && act accordingly   
- [ ] logs                                                        
- [ ] free memory                                                 
- [ ] report    


## Suggestions

:grey_question: use atexit for the last log - [info](https://www.tutorialspoint.com/c_standard_library/c_function_atexit.htm)\
:grey_question: use unlink to delete pipe - [info](https://linux.die.net/man/2/unlink)\
:grey_question: send struct via public FIFO:
1. convert struct to string 
2. write in binary

:grey_question: detect closure of pipe
* if writter can't open FIFO

