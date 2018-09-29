Q1
In this question i first opened the file given in argument,checked its no. of characters using lseek,then i started a loop and took the last 10,00,000 characters and converted their case and placed them in the new file.

Q2.
I first created a symlink to the file created in Q1 using symlink function and then using stat function i checked the different permissions given to both the file and directory created in Q1.
I used the same logic of Q1 for the checking of case inversion but here i regularly compared the inverted characters with the file created in Q1.
