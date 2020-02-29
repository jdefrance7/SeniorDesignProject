FILENAME    = "dsdl"
DEFINE      = "#define"
BOOL        = "BOOL"
INT         = "INT"
UINT        = "UINT"
FLOAT       = "FLOAT"
VOID        = "VOID"

def main():
    with open('{}.h'.format(FILENAME), 'w') as file:

        file.write("#ifndef {}_H\n".format(FILENAME.upper()))
        file.write("#define {}_H\n".format(FILENAME.upper()))

        file.write("\n")

        file.write("// {:/<77}\n\n".format("Description "))

        file.write("// List of DSDL variable types and their bit lengths.\n")
        file.write("// Also includes some library constants for reference.\n")

        file.write("\n")

        file.write("// {:/<77}\n\n".format("Booleans "))

        file.write("{} {:8} {:>4}\n".format(DEFINE, BOOL+"", 1))

        file.write("\n")

        file.write("// {:/<77}\n\n".format("Signed Integers "))

        for n in range(2,64):
            file.write("{} {:8} {:>4}\n".format(DEFINE, INT+str(n), n))

        file.write("\n")

        file.write("// {:/<77}\n\n".format("Unsigned Integers "))

        for n in range(2,64):
            file.write("{} {:8} {:>4}\n".format(DEFINE, UINT+str(n), n))

        file.write("\n")

        file.write("// {:/<77}\n\n".format("Floats "))

        file.write("{} {:8} {:>4}\n".format(DEFINE, FLOAT+str(16), 16))
        file.write("{} {:8} {:>4}\n".format(DEFINE, FLOAT+str(32), 32))
        file.write("{} {:8} {:>4}\n".format(DEFINE, FLOAT+str(64), 64))

        file.write("\n")

        file.write("// {:/<77}\n\n".format("Voids "))

        for n in range(2,64):
            file.write("{} {:8} {:>4}\n".format(DEFINE, VOID+str(n), n))

        file.write("\n")

        file.write("// {:/<77}\n\n".format("Library References "))

        file.write("#define DATA_TYPE_NA -1\n")

        file.write("\n")

        file.write("#endif // {}_H".format(FILENAME.upper()))

if __name__ == '__main__':
    main()
