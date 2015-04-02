import turtle
for sides in 2, 3, 4, 5, 6, 8, 12, 24, 48, 120, 360:
  for j in range(sides):  
    turtle.forward(360 / sides)
    turtle.left(360 / sides)

def shape(sides):
  for j in range(sides):  
    turtle.forward(360 / sides)
    turtle.left(360 / sides)
  
