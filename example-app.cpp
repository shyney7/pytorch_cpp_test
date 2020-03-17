 #include <torch/torch.h>
 #include <iostream>
 #include <fstream>
 #include <vector>
 #include <string>
 //#include <utility> //std::pair
 //#include <stdexcept> // std::runtime_error
 #include <sstream> // std::stringstream


//-----------------------------NETZDEFINITION-----------------------------------------------------
struct MeinNetz : torch::nn::Module {
  MeinNetz() {
    fc1 = register_module("fc1", torch::nn::Linear(10,10));
    fc2 = register_module("fc2", torch::nn::Linear(10, 10));

  }

  torch::Tensor forward(torch::Tensor x) {
    x = torch::relu(fc1->forward(x));
    x = torch::relu(fc2->forward(x));
    return x;
  }

torch::nn::Linear fc1{nullptr}, fc2{nullptr};

};

//----------------------Funktionsprototyp Onelinevector-------------------------------------------

std::vector<float> onelinevector(const std::vector<std::vector<float>>& invector);

//------------------------------------write csv Funktion------------------------------------------


/* 
void write_csv(std::string filename, std::vector<std::pair<std::string, std::vector<int>>> dataset){
    // Make a CSV file with one or more columns of integer values
    // Each column of data is represented by the pair <column name, column data>
    //   as std::pair<std::string, std::vector<int>>
    // The dataset is represented as a vector of these columns
    // Note that all columns should be the same size
    
    // Create an output filestream object
    std::ofstream myFile(filename);
    
    // Send column names to the stream
    for(int j = 0; j < dataset.size(); ++j)
    {
        myFile << dataset.at(j).first;
        if(j != dataset.size() - 1) myFile << ","; // No comma at end of line
    }
    myFile << "\n";
    
    // Send data to the stream
    for(int i = 0; i < dataset.at(0).second.size(); ++i)
    {
        for(int j = 0; j < dataset.size(); ++j)
        {
            myFile << dataset.at(j).second.at(i);
            if(j != dataset.size() - 1) myFile << ","; // No comma at end of line
        }
        myFile << "\n";
    }
    
    // Close the file
    myFile.close();
} */

//------------------------------read_csv Funktion-------------------------------------
/* 
std::vector<std::pair<std::string, std::vector<int>>> read_csv(std::string filename){
    // Reads a CSV file into a vector of <string, vector<int>> pairs where
    // each pair represents <column name, column values>

    // Create a vector of <string, int vector> pairs to store the result
    std::vector<std::pair<std::string, std::vector<int>>> result;

    // Create an input filestream
    std::ifstream myFile(filename);

    // Make sure the file is open
    if(!myFile.is_open()) throw std::runtime_error("Could not open file");

    // Helper vars
    std::string line, colname;
    int val;

    // Read the column names
    if(myFile.good())
    {
        // Extract the first line in the file
        std::getline(myFile, line);

        // Create a stringstream from line
        std::stringstream ss(line);

        // Extract each column name
        while(std::getline(ss, colname, ',')){
            
            // Initialize and add <colname, int vector> pairs to result
            result.push_back({colname, std::vector<int> {}});
        }
    }

    // Read data, line by line
    while(std::getline(myFile, line))
    {
        // Create a stringstream of the current line
        std::stringstream ss(line);
        
        // Keep track of the current column index
        int colIdx = 0;
        
        // Extract each integer
        while(ss >> val){
            
            // Add the current integer to the 'colIdx' column's values vector
            result.at(colIdx).second.push_back(val);
            
            // If the next token is a comma, ignore it and move on
            if(ss.peek() == ',') ss.ignore();
            
            // Increment the column index
            colIdx++;
        }
    }

    // Close file
    myFile.close();

    return result;
} */

//---------------------------------------------------------------------------------------------------

std::vector<std::vector<float>> csv2Dvector(std::string inputFileName) {
  using namespace std;
 
    vector<vector<float> > data;
    ifstream inputFile(inputFileName);
    int l = 0;
 
    while (inputFile) {
        l++;
        string s;
        if (!getline(inputFile, s)) break;
        if (s[0] != '#') {
            istringstream ss(s);
            vector<float> record;
 
            while (ss) {
                string line;
                if (!getline(ss, line, ','))
                    break;
                try {
                    record.push_back(stof(line));
                }
                catch (const std::invalid_argument e) {
                    cout << "NaN found in file " << inputFileName << " line " << l
                         << endl;
                    e.what();
                }
            }
 
            data.push_back(record);
        }
    }
 
    if (!inputFile.eof()) {
        cerr << "Could not read file " << inputFileName << "\n";
        __throw_invalid_argument("File not found.");
    }
 
    return data;
}

//-------------------------------------main-Funktion----------------------------

int main() {
  
auto net = std::make_shared<MeinNetz>();

//torch::Tensor in;



    //---------------------------------------------------------------------------
    std::vector<std::vector<float>> data = csv2Dvector("three_cols_2.csv");

    /* for (auto l : data) {
      for (auto x : l)
      std::cout << x << " ";
      std::cout << std::endl;
    } */
    std::cout << "Importierte Daten aus der csv Datei (Rohformat): " << std::endl;
    for (int i = 0; i< data.size(); i++){
      for (int j = 0; j<3 ; j++){
        std::cout << data[i][j];
      }
      std::cout << std::endl;
    }

//Transformation in 1D für tensor integration:
std::cout << "Transformiere Input Vector mit Zeilenanzahl: " << data.size()
<< " Und Spaltenanzahl: " << data.front().size() << " in einen Tensor:" << std::endl;

unsigned int ivsize = data.size() * data.front().size();
std::vector<float> linevec(ivsize);
linevec = onelinevector(data);
torch::Tensor itensor = torch::from_blob(linevec.data(), {(unsigned int) data.size(),(unsigned int) data.front().size()});
std::cout << "Input Tensor: \n" << itensor << std::endl;
//----------------------------------------------------------------
/*
torch::Tensor t1 = torch::from_blob(data.data(),{100,3});

std::cout << "Ab hier Tensor: " << std::endl;

std::cout << t1 << std::endl;

std::vector<float> testdata = {1,2,3,4};
torch::Tensor t2 = torch::from_blob(testdata.data(),{2,2});
std::cout << t2 << std::endl;

int vsize = (int) data.size() * 3;
std::vector<float> v1d(vsize);
std::cout << data.size() << std::endl;
int k = 0;

for (int i = 0; i<data.size(); i++) {
    
    for (int j = 0; j<3 ; j++) {
      v1d[k]=data[i][j];
      k++;
    }

}

for(int i = 0; i<v1d.size();i++) {
  std::cout << v1d[i] << std::endl;
}

torch::Tensor t3 = torch::from_blob(v1d.data(),{100,3});
std::cout << t3 << std::endl;
*/

  return 0;
}

//--------------------Funktionen--------------------------------------------

std::vector<float> onelinevector(const std::vector<std::vector<float>>& invector){

std::vector<float> v1d;
if (invector.size() == 0) return v1d;
v1d.reserve(invector.size() * invector.front().size());

for (auto& innervector : invector) {
  v1d.insert(v1d.end(), innervector.begin(), innervector.end());
}

return v1d;

}