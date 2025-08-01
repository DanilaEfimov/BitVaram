from limits import STACK_LIMIT

class Stack:

    def __init__(self):
        self.limit = STACK_LIMIT
        self.top = 0
        self.fields = []


    def add(self, value: int):
        if self.top < self.limit:
            self.fields.append(value)
            self.top += 1
        else:
            raise MemoryError("stack overflow")


    def pop(self):
        if self.top > 0:
            value = self.fields.pop()
            self.top -= 1
            return value
        else:
            raise MemoryError("stack underflow")
