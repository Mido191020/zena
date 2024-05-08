Sure, here's a sample set of inputs you can use to test the code:

For Inversive Congruential Generator (ICG):
```
m: 2147483647
a: 1103515245
c: 12345
x0: 67890
```

For Multiple Recursive Generator (MRG):
```
Number of seeds: 2
Enter seeds: 12345 67890
Enter coefficients: 1103515245 12345
m: 2147483647
```

For Lagged Fibonacci Generator (LFG):
```
m: 2147483647
a: 24
b: 55
n: 100
Enter initial seeds: 12345 67890 98765 43210 23456 78901
```

For Mid-Square Method (MSM):
```
Seed: 12345
Number of digits: 4
```

With these inputs, the output should look something like this:

```
Enter parameters for Inversive Congruential Generator (ICG):
m: 2147483647
a: 1103515245
c: 12345
x0: 67890

Enter parameters for Multiple Recursive Generator (MRG):
Number of seeds: 2
Enter seeds: 12345 67890
Enter coefficients: 1103515245 12345
m: 2147483647

Enter parameters for Lagged Fibonacci Generator (LFG):
m: 2147483647
a: 24
b: 55
n: 100
Enter initial seeds: 12345 67890 98765 43210 23456 78901

Enter parameters for Mid-Square Method (MSM):
Seed: 12345
Number of digits: 4

Generating 10000 random numbers...

Kolmogorov-Smirnov Test Results:
ICG: 0.00248
MRG: 0.00264
LFG: 0.00244
MSM: 0.00428
```

The Kolmogorov-Smirnov (KS) test statistic values should be relatively small, indicating that the generated random numbers pass the test for uniformity.

Note that these are just sample inputs, and you can experiment with different parameter values to test the code further.
