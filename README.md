# project-01-f23-IshanAgg125

Compile and Run:

You can execute the program by typing make in terminal, and then ./MYps
The makefile provided makes the job of running the program easy.

Design section:
The design of the code is based on the arguments the user provides. 
1. I begin by checking if the user provides -p in their argument. 
   a) If they do provide it then the code program heads inside getAllTheInformation(PIDpath, argc, argv). This function takes the parameter PID, the number of arguments, and also all the arguments. This function returns a proc with all the updated values based on the argv For example:
   -s then stateInformation
   -S then systemTime
   -v then virtual memory
   if not provide -c then command line
   if not provide -U then the user time
   Next, I call the printElementsOfProc(struct proc) which prints all the elements in the proc which are not -1 or "\0". 

   b) If they do not provide the -p argument. Then I head inside the struct proc informationOfSameUserID(char *PID, int argc, char **argv) function. The function contains all the PID which has the same userID as mine. Next if the argument (argv) is only 1 then I am printing the userTime and commandLine. based on the arguments provided I am updating the proc. For example:
   -s then stateInformation
   -S then systemTime
   -v then virtual memory
   if not provide -c then command line
   if not provide -U then the user time
   In the end I am putting all the values based on the argument in a strcut proc and then returning it. Next, I call the printElementsOfProc(struct proc) which prints all the elements in the proc which are not -1 or "\0". 

Testing plan:

1. I had to test each argument like finding user time, or system time, or commandline. 
2. I tested the program by repeatedly changing the arguments (argv) to ensure I am getting the 
rigth values. If the output would be incorrect i would use the C debugger to see where I am getting
an error. 
3. I had same cases of failed memory allocation, and memory leaks which literally took forever to debug. Regardless, I used printf(), put(), statements to check where the memory leak or failed memory allocation is happening. 
4. A funny bug I came across was when i was trying to save the information of the PID after -p. If I tried doing it after the while loop (I was looping through the arguments), the location of the PID would change, and I was like bruh! how did that happen? So that was one bug which took me forever to debug. 


