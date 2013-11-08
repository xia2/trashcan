import turtle

def draw(itrn):
  sqr = 'FRFRFRFR'
  for j in range(itrn):
    sqr = sqr.replace('F', 'FLFRFRFLF')
    
  for s in sqr:
    if s == 'F':
      turtle.forward(3 ** (4 - itrn))
    elif s == 'L':
      turtle.left(90)
    elif s == 'R':
      turtle.right(90)

for j in range(5):
  draw(j)
      

      
  
