
class Function:

    def __init__(self, identifier: str, signature: int, instructions: dict):
        self.name = identifier
        self.signature = signature
        self.block = instructions

    # EXECUTOR
    def __call__(self, symbols) -> int:
        pass