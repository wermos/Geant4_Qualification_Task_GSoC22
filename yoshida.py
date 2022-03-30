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
