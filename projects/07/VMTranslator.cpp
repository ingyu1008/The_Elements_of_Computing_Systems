#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

enum cmdTypes
{
    C_ARITHMATIC,
    C_PUSH,
    C_POP,
    C_LABEL,
    C_GOTO,
    C_IF,
    C_FUNCTION,
    C_RETURN,
    C_CALL,
    EMPTY
};

class Parser
{
private:
    std::ifstream fin;
    std::vector<std::string> tokens;

public:
    Parser(std::string file)
    {
        this->fin.open(file);
    }
    ~Parser()
    {
        this->fin.close();
    }

    bool hasMoreCommands()
    {
        return !this->fin.eof();
    }

    void advance()
    {
        std::string currentCommand;
        std::getline(fin, currentCommand);
        if (currentCommand.find("//") != currentCommand.npos)
            currentCommand.erase(currentCommand.begin() + currentCommand.find("//"), currentCommand.end());

        tokens.clear();
        std::stringstream ss(currentCommand);
        while (ss >> currentCommand)
        {
            tokens.push_back(currentCommand);
        }
    }

    cmdTypes commandType()
    {
        if (tokens.size() == 0)
            return EMPTY;
        if (tokens[0] == "push")
            return C_PUSH;
        if (tokens[0] == "pop")
            return C_POP;
        if (tokens[0] == "label")
            return C_LABEL;
        if (tokens[0] == "if-goto")
            return C_IF;
        if (tokens[0] == "goto")
            return C_GOTO;
        if (tokens[0] == "function")
            return C_FUNCTION;
        if (tokens[0] == "return")
            return C_RETURN;
        if (tokens[0] == "call")
            return C_CALL;
        if (tokens[0] == "add" || tokens[0] == "sub" || tokens[0] == "neg" || tokens[0] == "eq" || tokens[0] == "gt" || tokens[0] == "lt" || tokens[0] == "and" || tokens[0] == "or" || tokens[0] == "not")
            return C_ARITHMATIC;
        std::cerr << "Compile Error : Invalid Command\n";
    }

    std::string arg1()
    {
        if (commandType() == C_ARITHMATIC)
        {
            return tokens[0];
        }
        return tokens[1];
    }

    int arg2()
    {
        try
        {
            return std::stoi(tokens[2]);
        }
        catch (std::invalid_argument e)
        {
            std::cerr << "Compile Error : Invalid Arguments for " << tokens[0] << " command\n";
        }
    }
};
class CodeWriter
{
private:
    std::ofstream fout;
    std::string filename;
    int idx = 0;

public:
    CodeWriter(std::string file)
    {
        this->fout.open(file);
    }
    ~CodeWriter()
    {
        this->Close();
    }

    void setFileName(std::string filename)
    {
        this->filename = filename;
    }

    void writerArithmetic(std::string command)
    {
        if (command == "add")
        {
            fout << "@SP\n"
                 << "AM=M-1\n"
                 << "D=M\n"
                 << "A=A-1\n"
                 << "M=D+M\n";
        }
        else if (command == "sub")
        {
            fout << "@SP\n"
                 << "AM=M-1\n"
                 << "D=M\n"
                 << "A=A-1\n"
                 << "M=M-D\n";
        }
        else if (command == "neg")
        {
            fout << "@SP\n"
                 << "A=M-1\n"
                 << "M=-M\n";
        }
        else if (command == "eq")
        {
            fout << "@SP\n"
                 << "AM=M-1\n"
                 << "D=M\n"
                 << "A=A-1\n"
                 << "DM=M-D\n"
                 << "@TRUE." << idx << "\n"
                 << "D;JEQ\n"
                 << "@SP\n"
                 << "A=M-1\n"
                 << "M=0\n"
                 << "@END." << idx << "\n"
                 << "0;JMP\n"
                 << "(TRUE." << idx << ")\n"
                 << "@SP\n"
                 << "A=M-1\n"
                 << "M=-1\n"
                 << "(END." << idx << ")\n";
            idx++;
        }
        else if (command == "gt")
        {
            fout << "@SP\n"
                 << "AM=M-1\n"
                 << "D=M\n"
                 << "A=A-1\n"
                 << "DM=M-D\n"
                 << "@TRUE." << idx << "\n"
                 << "D;JGT\n"
                 << "@SP\n"
                 << "A=M-1\n"
                 << "M=0\n"
                 << "@END." << idx << "\n"
                 << "0;JMP\n"
                 << "(TRUE." << idx << ")\n"
                 << "@SP\n"
                 << "A=M-1\n"
                 << "M=-1\n"
                 << "(END." << idx << ")\n";
            idx++;
        }
        else if (command == "lt")
        {
            fout << "@SP\n"
                 << "AM=M-1\n"
                 << "D=M\n"
                 << "A=A-1\n"
                 << "DM=M-D\n"
                 << "@TRUE." << idx << "\n"
                 << "D;JLT\n"
                 << "@SP\n"
                 << "A=M-1\n"
                 << "M=0\n"
                 << "@END." << idx << "\n"
                 << "0;JMP\n"
                 << "(TRUE." << idx << ")\n"
                 << "@SP\n"
                 << "A=M-1\n"
                 << "M=-1\n"
                 << "(END." << idx << ")\n";
            idx++;
        }
        else if (command == "and")
        {
            fout << "@SP\n"
                 << "AM=M-1\n"
                 << "D=M\n"
                 << "A=A-1\n"
                 << "M=D&M\n";
        }
        else if (command == "or")
        {
            fout << "@SP\n"
                 << "AM=M-1\n"
                 << "D=M\n"
                 << "A=A-1\n"
                 << "M=D|M\n";
        }
        else if (command == "not")
        {
            fout << "@SP\n"
                 << "A=M-1\n"
                 << "M=!M\n";
        }
        else
        {
            std::cerr << "Compile Error At writerArithmetic() : Unknown Command (" << command << ")\n";
        }
    }

    void WritePushPop(cmdTypes command, std::string segment, int index)
    {
        if (command == C_PUSH)
        {
            if (segment == "constant")
            {
                fout << "@" << index << "\n"
                     << "D=A\n"
                     << "@SP\n"
                     << "AM=M+1\n"
                     << "A=A-1\n"
                     << "M=D\n";
            }
            else
            {
                std::cerr << "Compile Error At WritePushPop() : Invalid Segment\n";
            }
        }
        else if (command == C_POP)
        {
            if (segment == "constant")
            {
                std::cerr << "Compile Error At WritePushPop() : pop does not support segment constant\n";
            }
            else
            {
                std::cerr << "Compile Error At WritePushPop() : Invalid Segment\n";
            }
        }
        else
        {
            std::cerr << "Compile Error At WritePushPop() : Invalid Command\n";
        }
    }
    void Close()
    {
        this->fout.close();
    }
};

void translate(Parser &parser, CodeWriter &cw)
{
}

int main(int argc, char const *argv[])
{
    

    return 0;
}
