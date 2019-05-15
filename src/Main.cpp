#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <cmath>

std::string change_filename_suffix(const std::string& filepath, const std::string& suffix)
{
    std::string new_name("");

    for(int i = 0; filepath[i] != '.'; i++)
    {
        new_name += filepath[i];
    }


    return new_name + '.' + suffix;

}

unsigned int get_file_size(const std::string& filepath)
{
    unsigned int byte_count = 0;

    std::ifstream in_file(filepath, std::ios::binary);

    char ch;
    while(in_file.get(ch))
    {
        byte_count++;
    }
    in_file.close();

    return byte_count;
}

bool encode(const std::string& filepath)
{
    std::ifstream in_file(filepath, std::ios::binary);

    std::ofstream out_file(change_filename_suffix(filepath,"lkd"), std::ios::binary);

    std::string file_type("");
    for(int i = filepath.size()-3; i < filepath.size(); i++)
    {
        out_file << filepath[i];
    }

    unsigned int file_size = get_file_size(filepath);
    char* buffer = (char*)malloc(file_size + 5);

    buffer[0] = 's';
    buffer[1] = 'u';
    buffer[2] = 'p';
    buffer[3] = 'e';
    buffer[4] = 'r';

    in_file.read(&buffer[5], file_size);
    in_file.close();

    for(int i = 0; i < file_size + 5; i++)
    {
        char ch  = rand() % 100;
        for(int p = 0; p < 8; p++)
        {
            bool bit = ((buffer[i] >> p) & 1) ^ ((ch >> p) & 1);

            buffer[i] ^= (-bit ^ buffer[i]) & (1UL << p);

        }
    }


    out_file.write(buffer, file_size+5);

    out_file.close();

    remove(filepath.c_str());

    return true;
}

bool decode(const std::string& filepath)
{
    std::ifstream in_file(filepath, std::ios::binary);

    std::string file_type("");

    char ch;
    in_file.get(ch);
    file_type += ch;
    in_file.get(ch);
    file_type += ch;
    in_file.get(ch);
    file_type += ch;


    unsigned int file_size = get_file_size(filepath);
    char* buffer = (char*)malloc(file_size);

    in_file.read(buffer, file_size-3);
    in_file.close();


    for(int i = 0; i < file_size; i++)
    {
        char ch  = rand() % 100;
        for(int p = 0; p < 8; p++)
        {
            bool bit = ((buffer[i] >> p) & 1) ^ ((ch >> p) & 1);

            buffer[i] ^= (-bit ^ buffer[i]) & (1UL << p);

        }
    }

    std::string check_word("");
    check_word += buffer[0];
    check_word += buffer[1];
    check_word += buffer[2];
    check_word += buffer[3];
    check_word += buffer[4];

    if(check_word != "super")
    {
        printf("Invalid password!\n");
        return false;
    }


    std::ofstream out_file(change_filename_suffix(filepath,file_type), std::ios::binary);

    out_file.write(&buffer[5], file_size-8);

    out_file.close();

    remove(filepath.c_str());

    return true;
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Program takes 1 argument!\n");
        printf("./file_coder [filepath]\n");
        return 0;
    }

    std::string filepath(argv[1]);
    std::string suffix("");
    suffix += filepath[filepath.size()-3];
    suffix += filepath[filepath.size()-2];
    suffix += filepath[filepath.size()-1];

    char task = suffix == "lkd" ? 'd':'e';

    std::ifstream in_file(filepath);
    if(!in_file.is_open())
    {
        printf("Can't open file '%s'!\n", filepath.c_str());
        return 0;
    }
    in_file.close();

    printf("Password : ");
    std::string password;
    std::cin >> password;

    unsigned int seed = 0;
    unsigned int p = password.size();
    for(int i = 0; i < password.size(); i++)
    {
        seed += pow(password[i], p);
        p--;
    }

    srand(seed);

    if(task == 'e')
    {
        encode(filepath);
    }
    else if(task == 'd')
    {
        decode(filepath);
    }


    return 0;
}
