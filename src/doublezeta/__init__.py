from enum import Enum
from .zetacore import (
    CpyMatrix
    )

class MatrixType(Enum):
    """
    Used to specify the data type of a CpyMatrix object.
    """
    BOOL = 0
    FLOAT = 1
    INT = 2
