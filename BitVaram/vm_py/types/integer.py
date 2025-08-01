from BitVaram.vm_py.types.variable import Variable
from ..memory import is_valid_address

class Integer(Variable):

    def __init__(self, value: int, identifier: str, address: int):
        if is_valid_address(address):
            self.value = value
            self.address = address
            self.name = identifier
        else:
            raise ValueError("Integer::__init__::invalid address")

    def get_value(self) -> int:
        return self.value

    def set_value(self, value: int):
        self.value = value

    def get_name(self) -> str:
        return self.name

    def set_name(self, name: str):
        self.name = name

    def get_address(self) -> int:
        return self.address

    # OPERATORS vvv

    def __add__(self, other):
        if isinstance(other, Integer):
            return Integer(self.value + other.value, self.name, self.address)
        return NotImplemented

    def __sub__(self, other):
        if isinstance(other, Integer):
            return Integer(self.value - other.value, self.name, self.address)
        return NotImplemented

    def __mul__(self, other):
        if isinstance(other, Integer):
            return Integer(self.value * other.value, self.name, self.address)
        return NotImplemented

    def __truediv__(self, other):
        if isinstance(other, Integer):
            if other.value == 0:
                raise ZeroDivisionError("Integer::__truediv__::division by zero")
            return Integer(self.value // other.value, self.name, self.address)
        return NotImplemented

    # UTF CHAR TABLE TRANSFORM
    def __str__(self):
        return chr(self.value)