import sys
def main():
    if len(sys.argv) != 3:
        print("ERROR: This program needs two command line arguments to run, where first one is")
        print("the input file and the second one is the output file!")
        print("Sample run command is as follows: python3 calculator.py input.txt output.txt")
        print("Program is going to terminate!")

    try:
        input_file = open(sys.argv[1], "r")

    except FileNotFoundError:
        print(f"ERROR: There is either no such a file namely {sys.argv[1]} or this program does not have")
        print(f"permission to read it!")
        print("Program is going to terminate!")

    try:
        output_file=open(sys.argv[2],"w")
        lines = input_file.readlines()
        c= [[i for i in line.split()] for line in lines]

    except FileNotFoundError:
        print(f"ERROR: There is either no such a file namely {sys.argv[2]} or this program does not have")
        print(f"permission to read it!")
        print("Program is going to terminate!")

    for i in c:
            
        if len(i)==0:
            output_file.write("")
        elif 0<len(i)<3 or len(i)>3:
            a=" ".join(i)
            output_file.write("{}\nLine format is erroneous!\n".format(a))

        if len(i)==3:
            try:
                s=float(i[0])
                y=float(i[2])    
                try:
                    if len(i)==0:
                        output_file.write("")
                    else:
                        if len(i)==3:
                            s=float(i[0])
                            y=float(i[2])  
                        else:
                            output_file.write("{} {} {}\nLine format is erroneous!\n".format(i[0],i[1],i[2]))

                    try:
                        if i[1]=="+":
                            output_file.write("{} + {}\n={}\n".format(i[0],i[2],round(s+y)))
                        elif i[1]=="-":
                            output_file.write("{} - {}\n={}\n".format(i[0],i[2],s-y))
                        elif i[1]=="*":
                            output_file.write("{} * {}\n={:.2f}\n".format(i[0],i[2],round(s*y,1)))
                        elif i[1]=="/":
                            if i==c[-1]:
                                output_file.write("{} / {}\n={:.2f}".format(i[0],i[2],s/y))
                            else:
                                output_file.write("{} / {}\n={:.2f}\n".format(i[0],i[2],s/y))
                        else:
                            output_file.write("{} {} {}\nERROR: There is no such an operator!\n".format(i[0],i[1],i[2]))

                    except IndexError:
                        output_file.write("")
                except ValueError:
                    output_file.write("{} {} {}\nERROR: Second operand is not a number!\n".format(i[0],i[1],i[2]))
            except ValueError:
                output_file.write("{} {} {}\nERROR: First operand is not a number!\n".format(i[0],i[1],i[2]))
if __name__=="__main__":
    main()


   


