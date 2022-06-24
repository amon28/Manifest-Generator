#pragma comment(lib, "rpcrt4.lib")  // UuidCreate - Minimum supported OS Win 2000
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <direct.h>

using namespace std;

string resourceDependency;
bool gametest = false;
bool dependency = false;

string generateUUID() {
    UUID uuid;
    UuidCreate(&uuid);
    char* str;
    UuidToStringA(&uuid, (RPC_CSTR*)&str);
    //RpcStringFreeA((RPC_CSTR*)&str);
    return str;

}

void behaviorManifest(string addonName,string author, string version) {
    ifstream myfile;
    ofstream myfile2;
    size_t q_idx = 0;
    string output;
    string line;

    //open file
    if (!gametest && !dependency) {
        myfile.open("template/behavior/manifest.json");
    }else if(gametest && !dependency) {
        myfile.open("template/behavior/manifestGametest.json");
    }
    else {
        myfile.open("template/behavior/manifestDependency.json");
    }

    if (myfile.is_open())
    {
        while (getline(myfile, line, '\n'))
        {
            q_idx = line.find("credits");
            if (std::string::npos != q_idx)
            {
                line.replace(q_idx, 7, "//Made using manifest generator by Dewdimpple");
            }
            q_idx = line.find("input0");
            if (std::string::npos != q_idx)
            {
                line.replace(q_idx, 6, ("Made By: " + author));
            }
            q_idx = line.find("input1");
            if (std::string::npos != q_idx)
            {
                line.replace(q_idx, 6, addonName);
            }
            q_idx = line.find("input2");
            if (std::string::npos != q_idx)
            {
                line.replace(q_idx, 6, generateUUID());
            }
            q_idx = line.find("input3");
            if (std::string::npos != q_idx)
            {
                line.replace(q_idx, 6, version);
            }
            q_idx = line.find("inputD");
            if (std::string::npos != q_idx)
            {
                line.replace(q_idx, 6, resourceDependency);
            }
            output = output + line + "\n";
        }
    }
    else {
        cout << "Unable to open file";
    }
    myfile.close();

    const string dirName = "output/" + addonName + " behavior";
    const string scriptsDirName = "output/" + addonName + " behavior" + "/scripts";
    _mkdir(dirName.c_str());
    if (gametest) {
    _mkdir(scriptsDirName.c_str());
    myfile2.open(scriptsDirName + "/Main.js");
    myfile2.close();
    }
    myfile2.open(dirName + "/manifest.json");
    myfile2 << output;
    myfile2.close();
}

void resourceManifest(string addonName, string author, string version) {
    ifstream myfile;
    ofstream myfile2;
    size_t q_idx = 0;
    string output;
    string line;

    myfile.open("template/resource/manifest.json"); //open file
    if (myfile.is_open())
    {
        while (getline(myfile, line, '\n'))
        {
            q_idx = line.find("credits");
            if (std::string::npos != q_idx)
            {
                line.replace(q_idx, 7, "//Made using manifest generator by Dewdimpple");
            }
            q_idx = line.find("input0");
            if (std::string::npos != q_idx)
            {
                line.replace(q_idx, 6, ("Made By: " + author));
            }
            q_idx = line.find("input1");
            if (std::string::npos != q_idx)
            {
                line.replace(q_idx, 6, addonName);
            }
            q_idx = line.find("input2");
            if (std::string::npos != q_idx && dependency)
            {
                line.replace(q_idx, 6, resourceDependency);
            }
            q_idx = line.find("input2");
            if ((std::string::npos != q_idx) && !dependency)
            {
                line.replace(q_idx, 6, generateUUID());
            }
            q_idx = line.find("input3");
            if (std::string::npos != q_idx)
            {
                line.replace(q_idx, 6, version);
            }
            output = output + line + "\n";
        }
    }
    else {
        cout << "Unable to open file";
    }
    myfile.close();

    const string dirName = "output/" + addonName + " resource";
    _mkdir(dirName.c_str());
    myfile2.open(dirName + "/manifest.json");
    myfile2 << output;
    myfile2.close();
}

int main()
{
    string addonName, author, version = "1,0,0";
    string line;
    size_t q_idx = 0;
    ifstream myfile;
    ofstream myfile2;
    int choose,adds;

    string output;

    cout << "Addon Name: ";
    getline(cin, addonName);
    cout << "Author: ";
    getline(cin, author);

    cout << "Manifest Creation:\n[1] Behavior Manifest\n[2] Resource Manifest\n[3] Behavior and Resource Manifest\n";
    cin >> choose;

    if (choose == 1 || choose == 3) {
        cout << "\nAdditional:\n[1] Gametest\n[2] Dependency\n";
        cin >> adds;
        switch (adds) {
        case 1:
            gametest = true;
            break;
        case 2:
            resourceDependency = generateUUID();
            dependency = true;
            break;

        default:
            cout << "Invalid Input\n";
        }
    }

    switch (choose) {
    case 1:
        behaviorManifest(addonName,author,version);
        cout << "Successfully Created Manifest!\n";
        break;

    case 2:
        resourceManifest(addonName, author, version);
        cout << "Successfully Created Manifest!\n";
        break;

    case 3:
        behaviorManifest(addonName, author, version);
        resourceManifest(addonName, author, version);
        cout << "Successfully Created Manifest!\n";
        break;

    default:
        cout << "Invalid Input\n";
    }
    system("pause");
}