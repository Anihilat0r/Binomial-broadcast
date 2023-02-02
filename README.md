# MPI Broadcast using Binomial tree

**void binomial_bcast(** void* **data**, int **count**, MPI_Datatype **datatype**, int **root**, MPI_Comm **communicator)**

Sends a message containing **count** integer number of **dayatype** elements that are present in contigious
memory addresses starting from the address pointed to by **data**. At first only the **root** process has access
to these specific elements. By the time Broadcast exits every process in the specified **communicator** will have
received the same message containing the elements root has sent. The processes that are to receive the data call
Broadcast passing the same arguments as root, with **data** being the first address of the local buffer used to
store the incoming elements.

Broadcast implementations in the MPI standard rely on different algorithms for different communicator and message sizes. For 
smaller distributed systems the use of a binomial tree algorithm is one of the most time effective solutions.

### How it works

At first all the processes get a new temporary rank relative to the root which gains the rank 0. These "relative ranks" 
will be used to figure out which process will send the message and the one that receives it. In the first send-receive
round only the root holds the data we desire to share, so it sends a message containing the data to the process with 
relative rank 1. In the second round both root and process 1 possess the data, so we are able to send messages to the
next two processes and so on. In every round we are able to send as many messages as the processes that possess the data
we wish to share across the communicator of our choice.

**Below is an example of the aforementioned implementation in a communicator containing 14 processes :**
![Binom_Bcst_14](https://user-images.githubusercontent.com/87810236/216342446-261f8a71-d6cf-4657-8a39-cec8af47b3d8.jpg)
