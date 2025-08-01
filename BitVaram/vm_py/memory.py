from limits import *
from BitVaram.vm_py.types.variable import Variable


def is_valid_address(address: int) -> bool:
    return (address > 0) and (address < MEMORY_LIMIT)


class Memory:

    def __init__(self):
        self.limit = MEMORY_LIMIT
        self.table = set()  # writen fields


    def malloc(self, cls: type, value) -> Variable:
        if issubclass(type, Variable):
            for address in range(self.limit):
                if not address in self.table:
                    self.table.add(address)
                    return cls(value, "", address)
            raise MemoryError("Memory::malloc::no free addresses")
        else:
            raise TypeError("Memory::malloc::undefined required type")


    def dealloc(self, address):
        if address in self.table:
            self.table.remove(address)
        else:
            raise MemoryError("Memory::dealloc::required address is free")