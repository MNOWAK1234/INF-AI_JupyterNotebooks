import re

#enter path to your folder where the newly made script will be saved
#remember to change '\' for '/' or place 'r' (means: raw) in front of a path
path='C:/Users/mikol/Desktop/Mikolaj'

#open file
f = open(path+'/zapis.txt', 'w')#w stands for write and a for append
f.write("#ifndef TEST_H\n")
f.write("#define TEST_H\n\n")

# Using readlines()
odczyt = open(path+'/figury.txt', 'r')
Lines = odczyt.readlines()

vertices=0
textures=0
normals=0
faces=0
vertices_in_file=0

V = []
VT = []
VN = []
sides = []

for line in Lines:
    if(line.strip()[0:2]=='v '):
        #V.append(line.strip()[2:])
        vertices+=1
        values = line.strip()[2:].split()
        numbers = ' '.join(map(str, map(float, values)))
        V.append(numbers)
    elif(line.strip()[0:2]=='vt'):
        #VT.append(line.strip()[3:])
        textures+=1
        values = line.strip()[2:].split()
        numbers = ' '.join(map(str, map(float, values)))
        VT.append(numbers)
    elif(line.strip()[0:2]=='vn'):
        #VN.append(line.strip()[3:])
        normals+=1
        values = line.strip()[2:].split()
        numbers = ' '.join(map(str, map(float, values)))
        VN.append(numbers)
    elif(line.strip()[0:2]=='f '):
        faces+=1
        #Use regular expressions
        face = re.split(r'\s+|/', line.strip()[2:])
        face = [int(num) for num in face if num.strip()]
        vertices_in_file+=(len(face)-6)
        sides.append(face)

print(vertices)
print(textures)
print(normals)
print(faces)

f.write("int TestVertexCount = "+str(vertices_in_file)+";\n\n")
f.write("float TestVertices[]={\n")

for face in sides:
    for i in range(3,len(face)-3,3):
        f.write("\t"+(V[face[0]-1]).replace(" ","f,")+"f,1.0f,\n") #first
        f.write("\t"+(V[face[i]-1]).replace(" ","f,")+"f,1.0f,\n") #first from next pair
        f.write("\t"+(V[face[i+3]-1]).replace(" ","f,")+"f,1.0f,\n") #second from next pair

f.write("};\n\n")
f.write("float tex[] = {\n")

for face in sides:
    for i in range(3,len(face)-3,3):
        f.write("\t"+(VT[face[1]-1]).replace(" ","f,")+"f,\n") #first
        f.write("\t"+(VT[face[i+1]-1]).replace(" ","f,")+"f,\n") #first from next pair
        f.write("\t"+(VT[face[i+3+1]-1]).replace(" ","f,")+"f,\n") #second from next pair

f.write("};\n\n")
f.write("float VertexNormals[] = {\n")

for face in sides:
    for i in range(3,len(face)-3,3):
        f.write("\t"+(VN[face[2]-1]).replace(" ","f,")+"f,0.0f,\n") #first
        f.write("\t"+(VN[face[i+2]-1]).replace(" ","f,")+"f,0.0f,\n") #first from next pair
        f.write("\t"+(VN[face[i+3+2]-1]).replace(" ","f,")+"f,0.0f,\n") #second from next pair

f.write("};\n")
f.write("#endif\n")
f.close()
