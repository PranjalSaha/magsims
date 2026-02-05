#!/bin/python3


from sympy import Symbol as sym
from sympy import Rational as frac
import copy

order = 5

print("Order =", order)


def display(M):
    print("[")
    for i in M:
        for j in i:
            print(j, end='\t')
        print()
    print("]\n")


# Creating matrix
M = [[frac(1, 1)*sym("a")]*2 + [frac(0, 1)]*order for _ in range(order + 1)]

# Initialising matrix

for i in range(order):
    M[i][0] = sym(f"a_{order - i}")
    M[i][1] = sym("a_0")

for i in range(order+1):
    for j in range(order):
        M[i][j+2] = frac((i - order) ** (j+1), 1)
M[order][0] = frac(1, 1)*sym("a_n")
M[order][1] = sym("a_0")
M[order][2:(order+3)] = [frac(1, 1)] * order

print("Initial Matrix")
display(M)

for i in range(order):
    for j in range(order+2):
        if j == i+2:
            continue
        M[i][j] = M[i][j] / M[i][i+2]
    M[i][i+2] = frac(1, 1)
    display(M)
    for j in range(i+1, order+1):
        for k in range(0, order+2):
            if k == i+2:
                continue
            M[j][k] = M[j][k] - M[i][k]*M[j][i+2]
        M[j][i+2] = frac(0, 1)
    display(M)

print('\n')
print(M[order][1] - M[order][0] + sym("a_n"))
print('\n\n')
