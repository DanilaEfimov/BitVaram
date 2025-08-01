
class SymbolTable:

    # vvv TableLine
    class TableLine:

        def __init__(self, identifier: str, is_global: bool):
            self.name = identifier
            self.is_global = is_global

    # ^^^ TableLine / SymbolTable vvv

    def __init__(self, core: dict=None):
        self.core = core if not core is None else dict()


    def get(self, identifier: str) -> TableLine:
        return self.core[identifier]


    def add(self, line: TableLine):
        self.core[line.name] = line
