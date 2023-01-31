function f1 pi1 pa2

def
var vi1
enddef

vi1 = call f2 pi1 pa2

return vi1

end


function f2 pi1 pa2

def
var vi1
enddef

set pa2 index ci5 with pi1
get pa2 index ci5 to vi1

#      5  +  5
vi1 = vi1 + pi1

return vi1

end