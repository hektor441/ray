
def typeof(x):
    if type(x) in [list, tuple]: return "Group"
    elif x.isupper() or x[0] == "?": return "Variable"
    elif x.replace(":_:_:","").isdigit() != x.isdigit(): return "Variable"
    else: return "Keyword"


def match(a, b):
    if typeof(a) == "Variable":
        return True
    