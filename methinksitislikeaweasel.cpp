#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

struct progeny{
    char progenyBody[29];
    float percent_match;
};

const char target_body[29] = {'M','E','T','H','I','N','K','S',' ','I','T',' ','I','S',' ','L','I','K','E',' ','A',' ','W','E','A','S','E','L','\0'};
const char gene_Alphabet[27] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',' '};
const float mutation_rate = 0.05;
const int population_size = 10;

char generate_random_Alphabet(){
    int num = rand()%27;
    return gene_Alphabet[num];
}

float percent_match(progeny* p){
    int x = 0;
    for(int i=0; i<28; i++){
        if(p->progenyBody[i] == target_body[i]){
            x++;
        }
    }

    return (static_cast<float>(x) / 28.0f) * 100.0f;
}

progeny* current_generation_parent;
void setFirstParent(){
    current_generation_parent = new progeny;
    for(int i=0; i<28; i++){
        current_generation_parent->progenyBody[i] = generate_random_Alphabet();
    }
    current_generation_parent->progenyBody[28] = '\0';
    current_generation_parent->percent_match = percent_match(current_generation_parent);
}

progeny* generate_progeny(progeny* parent){
    progeny* p = new progeny;
    for(int i=0; i<28; i++){
        double chance = static_cast<double>(rand()) / RAND_MAX;

        if(chance <= mutation_rate){
            p->progenyBody[i] = generate_random_Alphabet();
        }
        else{
            p->progenyBody[i] = parent->progenyBody[i];
        }
        
    }
    p->progenyBody[28] = '\0';

    p->percent_match = percent_match(p);
    return p;
}

void writeProgeny(ofstream& outFile, int progeny_number, const char* progeny_body, float percent_match){
    outFile<< progeny_number << "     " << progeny_body << "     " << percent_match << "\n";
}

void generate_generation(progeny* parent, ofstream& outFile){

    float highest_percent_match = 0.00;
    char highest_matching_progeny_body;
    
    for(int i=0; i<population_size; i++){
        progeny* p = generate_progeny(parent);
        if(p->percent_match > highest_percent_match) {
            highest_percent_match = p->percent_match;
            current_generation_parent = p;
        }
        writeProgeny(outFile, i+1, p->progenyBody, p->percent_match);
    }

    outFile <<"HIGHEST MATCHING PROGENY: "<<current_generation_parent->progenyBody<<"\n";
}


int main(){
    
    srand(time(NULL));
    int generation_number = 0;

    ofstream outFile("evolution.txt");
    if(!outFile.is_open()){
        cout<<"Error: Could not open file"<<endl;
        return  1;
    }

    setFirstParent();

    int choice = 1;

    cout<<"1. START EVOLUTION"<<endl;
    cout<<"2. ABORT"<<endl;
    cin>>choice;
    
    while(choice != 2){

        outFile<<"GENERATION "<< generation_number << "\n";
        generate_generation(current_generation_parent, outFile);
        if(current_generation_parent->percent_match == 100){
            cout<<"EVOLUTION COMPLETED !!"<<endl;
            return 0;
        }
        generation_number++;
        
    }

    return 0;
}