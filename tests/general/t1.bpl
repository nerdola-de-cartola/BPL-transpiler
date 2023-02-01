function f1 pi1 pi2 pa3

def
var vi1
enddef

if pi1
vi1 = call f2 pi2 pa3
endif

if pi1
return vi1
endif

return pi2

end

function f2 pi1 pa2

def
var vi1
var vi2
enddef

vi1 = pi1

get pa1 index ci0 to vi2
vi1 = vi1 + vi2

get pa1 index ci1 to vi2
vi1 = vi1 + vi2

get pa1 index ci2 to vi2
vi1 = vi1 + vi2

return vi1

end