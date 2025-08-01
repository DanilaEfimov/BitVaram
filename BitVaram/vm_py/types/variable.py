from abc import ABC, abstractmethod

class Variable(ABC):

    @abstractmethod
    def get_value(self):
        pass

    @abstractmethod
    def set_value(self, value):
        pass

    @abstractmethod
    def get_name(self) -> str:
        pass

    @abstractmethod
    def set_name(self, name):
        pass

    @abstractmethod
    def get_address(self) -> int:
        pass

