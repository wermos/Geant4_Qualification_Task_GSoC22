"""
I used this program to find the numerical values of each constant in the Yoshida
algorithm (a specific type of algorithm in the leapfrog family of algorithms),
to 15 digits of accuracy (which is the limit of accuracy in a C++ double on a
64 bit machine.

I then copied and pasted the numerical values into leapfrog.hpp.

I used the Wikipedia page for reference:
https://en.wikipedia.org/wiki/Leapfrog_integration#4th_order_Yoshida_integrator
"""

import sympy

cbrtTwo = sympy.cbrt(2)

w0 = - cbrtTwo / (2 - cbrtTwo)
w1 = 1 / (2 - cbrtTwo)

print(f"c1 = w1 / 2 = {(w1 / 2).n(15)}")
print(f"c4 = w1 / 2 = {(w1 / 2).n(15)}")

print(f"c2 = (w0 + w1) / 2 = {((w0 + w1) / 2).n(15)}")
print(f"c3 = (w0 + w1) / 2 = {((w0 + w1) / 2).n(15)}")

print(f"d1 = w1 = {w1.n(15)}")
print(f"d3 = w1 = {w1.n(15)}")
print(f"d2 = w0 = {w0.n(15)}")
