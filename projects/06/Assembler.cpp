#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <bitset>

enum cmdTypes
{
    A_COMMAND,
    C_COMMAND,
    L_COMMAND
};

std::ofstream fout;
class Parser
{
private:
    std::ifstream fin;
    std::string currentCommand;

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
        std::getline(fin, currentCommand);
        if (currentCommand.find("//") != currentCommand.npos)
            currentCommand.erase(currentCommand.begin() + currentCommand.find("//"), currentCommand.end());
        currentCommand.erase(std::remove_if(currentCommand.begin(), currentCommand.end(), isspace), currentCommand.end());
    }

    cmdTypes commandType()
    {
        if (currentCommand[0] == '@')
            return A_COMMAND;
        if (currentCommand[0] == '(' && currentCommand[currentCommand.size() - 1] == ')')
            return L_COMMAND;
        return C_COMMAND;
    }

    std::string symbol()
    {
        if (currentCommand[0] == '@')
            return currentCommand.substr(1);
        else
            return currentCommand.substr(1, currentCommand.size() - 2);
    }

    std::string dest()
    {
        int pos = currentCommand.find("=");
        int len;
        if (pos == -1)
        {
            len = 0;
        }
        else
        {
            len = pos;
        }
        return currentCommand.substr(0, len);
    }

    std::string comp()
    {
        int pos1 = currentCommand.find("=");
        int pos2 = currentCommand.find(";");
        int pos, len;
        if (pos1 == -1)
        {
            pos = 0;
        }
        else
        {
            pos = pos1 + 1;
        }
        if (pos2 == -1)
        {
            len = -1;
        }
        else
        {
            len = pos2 - pos1 - 1;
        }
        if (len >= 0)
            return currentCommand.substr(pos, len);
        return currentCommand.substr(pos);
    }

    std::string jump()
    {
        int pos = currentCommand.find(";");
        int len;
        if (pos == -1)
        {
            pos = 0;
            len = 0;
        }
        else
        {
            return currentCommand.substr(pos + 1);
        }
        return "";
    }
};

class Code
{
public:
    std::string dest(std::string mnemonic)
    {
        if (mnemonic == "M")
            return "001";
        if (mnemonic == "D")
            return "010";
        if (mnemonic == "MD")
            return "011";
        if (mnemonic == "A")
            return "100";
        if (mnemonic == "AM")
            return "101";
        if (mnemonic == "AD")
            return "110";
        if (mnemonic == "AMD")
            return "111";
        return "000";
    }

    std::string comp(std::string mnemonic)
    {
        if (mnemonic == "0")
            return "0101010";
        if (mnemonic == "1")
            return "0111111";
        if (mnemonic == "-1")
            return "0111010";
        if (mnemonic == "D")
            return "0001100";
        if (mnemonic == "A")
            return "0110000";
        if (mnemonic == "!D")
            return "0001101";
        if (mnemonic == "!A")
            return "0110001";
        if (mnemonic == "-D")
            return "0001111";
        if (mnemonic == "-A")
            return "0110011";
        if (mnemonic == "D+1")
            return "0011111";
        if (mnemonic == "A+1")
            return "0110111";
        if (mnemonic == "D-1")
            return "0001110";
        if (mnemonic == "A-1")
            return "0110010";
        if (mnemonic == "D+A")
            return "0000010";
        if (mnemonic == "D-A")
            return "0010011";
        if (mnemonic == "A-D")
            return "0000111";
        if (mnemonic == "D&A")
            return "0000000";
        if (mnemonic == "D|A")
            return "0010101";
        if (mnemonic == "M")
            return "1110000";
        if (mnemonic == "!M")
            return "1110001";
        if (mnemonic == "-M")
            return "1110011";
        if (mnemonic == "M+1")
            return "1110111";
        if (mnemonic == "M-1")
            return "1110010";
        if (mnemonic == "D+M")
            return "1000010";
        if (mnemonic == "D-M")
            return "1010011";
        if (mnemonic == "M-D")
            return "1000111";
        if (mnemonic == "D&M")
            return "1000000";
        if (mnemonic == "D|M")
            return "1010101";
        std::cerr << "Compile Error\n";
        return "1111111";
    }

    std::string jump(std::string mnemonic)
    {
        if (mnemonic == "JGT")
            return "001";
        if (mnemonic == "JEQ")
            return "010";
        if (mnemonic == "JGE")
            return "011";
        if (mnemonic == "JLT")
            return "100";
        if (mnemonic == "JNE")
            return "101";
        if (mnemonic == "JLE")
            return "110";
        if (mnemonic == "JMP")
            return "111";
        return "000";
    }
};

int main(int argc, char const *argv[])
{

    Parser parser(argv[1]);
    Code code;

    fout.open("rect/out.hack");

    while (parser.hasMoreCommands())
    {
        parser.advance();
        cmdTypes type = parser.commandType();
        if (type == A_COMMAND)
        {
            fout << "0";
            std::string bits = std::bitset<15>(std::stoi(parser.symbol())).to_string();
            fout << bits << "\n";
        }
        else if (type == C_COMMAND)
        {
            if (parser.comp().empty())
                continue;
            fout << "111" << code.comp(parser.comp()) << code.dest(parser.dest()) << code.jump(parser.jump()) << "\n";
        }
    }

    fout.close();

    return 0;
}
