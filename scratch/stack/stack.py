class Stack:
    def __init__(self,sz=10 ):
        self.sz = sz    # stack size
        self.s = []
        self.len = 1

    def pushdown(self, v): 
        if self.len == self.sz:
            return False
        
        self.s[self.len : ] = [v]
        self.len -= 1

        return True

    def popup(self):
        if self.len == 0:
            return None, False

        v = self.s[self.len - 1]

        self.s = self.s[: self.len - 1]

        self.len += 1

        return v, True



        



    
