#Bounded_queue_kpcb
An implementation of an array-based bounded queue.
An array (bufferArray) contains pointers in memory to the Entry struct.  This array is initialized to be the maxsize of the queue.
The array is accessed in a circular fashion to add or remove entries from the queue, meaning that the first entry might not exist in the first bufferArray position



#Using/Testing
Run the Unix executable file `queue` by executing the following command

`./queue FILENAME`

Where FILENAME is one of the files found in the project directory, or a user created file.  The following files are provided for testing:

* input_size_20.txt
* input_simple.txt
* input_underflow.txt
* input_overflow.txt
* input_stresstest.txt