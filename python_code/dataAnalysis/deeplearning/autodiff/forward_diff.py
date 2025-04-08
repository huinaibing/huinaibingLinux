class ForwardDiff:
    def __init__(self, val, dv):
        self.val = val
        self.dv = dv

    def __repr__(self):
        return str(self.val) + str(self.dv) + "d"

    def __add__(self, other):
        return ForwardDiff(self.val + other.val, self.dv + other.dv)

    def __sub__(self, other):
        return ForwardDiff(self.val - other.val, self.dv - other.dv)

    def __mul__(self, other):
        return ForwardDiff(self.val * other.val, self.dv * other.val + self.val * other.dv)
