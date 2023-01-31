function f1 pa1

def
var vi1
var vi2
enddef

get pa1 index ci0 to vi1
vi2 = vi1

get pa1 index ci1 to vi1
vi2 = vi2 + vi1

get pa1 index ci2 to vi1
vi2 = vi2 + vi1

get pa1 index ci3 to vi1
vi2 = vi2 + vi1

get pa1 index ci4 to vi1
vi2 = vi2 + vi1

return vi2

end

function f2 pa1

def
var vi1
var vi2
vet va3 size ci30
enddef

set va3 index ci0 with ci0
set va3 index ci1 with ci1
set va3 index ci2 with ci2
set va3 index ci3 with ci4
set va3 index ci4 with ci8
# sum = 15 

vi1 = call f1 va3                # vi2 = f1(vi1, &va3)
vi2 = call f1 pa1                # vi2 = f1(5, pa1)
vi1 = vi1 + vi2
# 15 + 28 = 
return vi1

end