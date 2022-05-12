'''
CIS 122 Winter 2020 Lab 7
Author: Ryan Heise
Partner: None
Description: Count total words, Find Longest word, Find shortest word, Find palendromes, First letter counter 
'''

filename = "/Users/ryanheise/Documents/122/Assignments/cis122-assign07/words_alpha.txt"



def count(filename):
    count = 0
    with open(filename) as file:
        for line in file:
            words = line.split()
            count += len(words)
        print ("Count:", count)
        return count

def LongWord(filename):
    with open(filename) as file:
        t = file.readlines()
        # Sorts by length
        m = max(t, key = len)
        m = m.strip("\n")
        print("Shortest Word Is:", m, "(", len(m), ")")


def ShortWord(filename):
    with open(filename) as file:
        t = file.readlines()
        # Sorts by length
        m = min(t, key = len)
        m = m.strip("\n")
        print("Shortest Word Is:", m, "(", len(m), ")")

def palendrome(filename):
    palendrome = 0
    count = 0
    with open(filename) as file:
        for line in file:
            line = line.strip()     
            if line == line[::-1]:
                palendrome += 1
    with open(filename) as file:
        # for every line in that file it will find the comments and update the comment counter
        for line in file:
            if line.find("#") == 0:
                comment_counter += 1
            # if there are no comments will will cac the total of all the lines and find the average
            else:
                count += 1
                average = round((palendrome / count) * 100, 2)
                
        print("Palendromes:", palendrome, "(", average, "%)")

def LetterCount(filename):
    a_count = 0
    b_count = 0
    c_count = 0
    d_count = 0
    e_count = 0
    f_count = 0
    g_count = 0
    h_count = 0
    i_count = 0
    j_count = 0
    k_count = 0
    l_count = 0
    m_count = 0
    n_count = 0
    o_count = 0
    p_count = 0
    q_count = 0
    r_count = 0
    s_count = 0
    t_count = 0
    u_count = 0
    v_count = 0
    w_count = 0
    x_count = 0
    y_count = 0
    z_count = 0
    count = 0
    with open(filename) as file:
        for line in file:
            if line.find("#") == 0:
                comment_counter += 1
            # if there are no comments will will cac the total of all the lines and find the average
            else:
                count += 1
            if line.startswith("a"):
                a_count += 1
            if line.startswith("b"):
                b_count += 1
            if line.startswith("c"):
                c_count += 1
            if line.startswith("d"):
                d_count += 1
            if line.startswith("e"):
                e_count += 1
            if line.startswith("f"):
                f_count += 1
            if line.startswith("g"):
                g_count += 1
            if line.startswith("h"):
                h_count += 1
            if line.startswith("i"):
                i_count += 1
            if line.startswith("j"):
                j_count += 1
            if line.startswith("k"):
                k_count += 1
            if line.startswith("l"):
                l_count += 1
            if line.startswith("m"):
                m_count += 1
            if line.startswith("n"):
                n_count += 1
            if line.startswith("o"):
                o_count += 1
            if line.startswith("p"):
                p_count += 1
            if line.startswith("q"):
                q_count += 1
            if line.startswith("r"):
                r_count += 1
            if line.startswith("s"):
                s_count += 1
            if line.startswith("t"):
                t_count += 1
            if line.startswith("u"):
                u_count += 1
            if line.startswith("v"):
                v_count += 1
            if line.startswith("w"):
                w_count += 1
            if line.startswith("x"):
                x_count += 1
            if line.startswith("y"):
                y_count += 1
            if line.startswith("z"):
                z_count += 1
        print("First letter counts")
        print("A:", a_count, "(" , round((a_count / count) * 100, 2), "%)")
        print("B:", b_count, "(" , round((b_count / count) * 100, 2), "%)")
        print("C:", c_count, "(" , round((c_count / count) * 100, 2), "%)")
        print("D:", d_count, "(" , round((d_count / count) * 100, 2), "%)")
        print("E:", e_count, "(" , round((e_count / count) * 100, 2), "%)")
        print("F:", f_count, "(" , round((f_count / count) * 100, 2), "%)")
        print("G:", g_count, "(" , round((g_count / count) * 100, 2), "%)")
        print("H:", h_count, "(" , round((h_count / count) * 100, 2), "%)")
        print("I:", i_count, "(" , round((i_count / count) * 100, 2), "%)")
        print("J:", j_count, "(" , round((j_count / count) * 100, 2), "%)")
        print("K:", k_count, "(" , round((k_count / count) * 100, 2), "%)")
        print("L:", l_count, "(" , round((l_count / count) * 100, 2), "%)")
        print("M:", m_count, "(" , round((m_count / count) * 100, 2), "%)")
        print("N:", n_count, "(" , round((n_count / count) * 100, 2), "%)")
        print("O:", o_count, "(" , round((o_count / count) * 100, 2), "%)")
        print("P:", p_count, "(" , round((p_count / count) * 100, 2), "%)")
        print("Q:", q_count, "(" , round((q_count / count) * 100, 2), "%)")
        print("R:", r_count, "(" , round((r_count / count) * 100, 2), "%)")
        print("S:", s_count, "(" , round((s_count / count) * 100, 2), "%)")
        print("T:", t_count, "(" , round((t_count / count) * 100, 2), "%)")
        print("U:", u_count, "(" , round((u_count / count) * 100, 2), "%)")
        print("V:", v_count, "(" , round((v_count / count) * 100, 2), "%)")
        print("W:", w_count, "(" , round((w_count / count) * 100, 2), "%)")
        print("X:", x_count, "(" , round((x_count / count) * 100, 2), "%)")
        print("Y:", y_count, "(" , round((y_count / count) * 100, 2), "%)")
        print("Z:", z_count, "(" , round((z_count / count) * 100, 2), "%)")


count(filename)
LongWord(filename)
ShortWord(filename)
palendrome(filename)
LetterCount(filename)