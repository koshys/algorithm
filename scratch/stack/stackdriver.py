import stack

stk = stack.Stack(sz=2)
print(stk)

print(stk.pushdown('apple'))
print(stk.pushdown('orange'))
print(stk.pushdown('grapes'))

print(stk.popup())
print(stk.popup())
print(stk.popup())


def add ( x, y):
    return x + y

print(add(10,12))    

print("print example")
