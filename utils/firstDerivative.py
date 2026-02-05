#!/bin/python3

from sympy import Symbol as sym, simplify
from sympy import Rational as frac

order = 4

print("Order =", order)


def display(M):
    print("[")
    for i in M:
        for j in i:
            print(j, end='\t')
        print()
    print("]\n")


# Creating matrix
M = [[frac(1, 1)*sym("a")]*2 + [frac(0, 1)]*(order-1) for _ in range(order)]

# Initialising matrix

for i in range(order):
    M[i][0] = sym(f"f_{order - i}") - sym("f_0")
    M[i][1] = sym("f'_0") * sym("h") * frac(i-order, 1)

for i in range(order):
    for j in range(2, order+1):
        M[i][j] = frac((i - order) ** (j), 1)

print("Initial Matrix")
display(M)

for i in range(order-1):
    for j in range(order+1):
        if j == i+2:
            continue
        M[i][j] = M[i][j] / M[i][i+2]
    M[i][i+2] = frac(1, 1)
    display(M)
    for j in range(i+1, order):
        for k in range(0, order+1):
            if k == i+2:
                continue
            M[j][k] = M[j][k] - M[i][k]*M[j][i+2]
        M[j][i+2] = frac(0, 1)
    display(M)

print('\n')
print(simplify(M[order-1][0] / M[order-1][1] * sym("f'_0")))
print('\n\n')
