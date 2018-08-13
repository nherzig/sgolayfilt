// sgolay.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

template<typename T>
std::vector<T>
conv_valid(std::vector<T> const &f, std::vector<T> const &g) {
  int const nf = f.size();
  int const ng = g.size();
  std::vector<T> const &min_v = (nf < ng)? f : g;
  std::vector<T> const &max_v = (nf < ng)? g : f;
  int const n  = std::max(nf, ng) - std::min(nf, ng) + 1;
  std::vector<T> out(n, T());
  for(auto i(0); i < n; ++i) {
    for(int j(min_v.size() - 1), k(i); j >= 0; --j) {
      out[i] += min_v[j] * max_v[k];
      ++k;
    }
  }
  return out;
}

template<typename T>
std::vector<T> 
sgolay(std::vector<T> const &x_unfilt,std::vector<std::vector<T>> const &sgolaymat){
	std::vector<T> x_filt,xcenter;

	int framelen(sgolaymat.size()); //compute thr frame length
	for(int i(0);i<framelen;i++){
		if(framelen!=sgolaymat[i].size()){ //check the matrix size
			std::cout<<"sgolay matrix is not correct"<<std::endl;
			return x_filt;
		}
	}

	//Compute the values for the part smaller than the frame length at the begining
	for(int i(framelen-1);i>=(framelen+3)/2-1;i--){
		double xtemp(0.0);
		for(int j(0);j<framelen;j++){
			xtemp+=sgolaymat[i][j]*x_unfilt[framelen-1-j];
		}
		x_filt.push_back(xtemp);
	}
	////Compute the values in the center part 
	xcenter=conv_valid(x_unfilt,sgolaymat[(framelen+1)/2-1]);
	x_filt.insert(x_filt.end(),xcenter.begin(),xcenter.end());

	//Compute the values for the part smaller than the frame length at the end
	for(int i((framelen-1)/2-1);i>=0;i--){
		double xtemp(0.0);
		for(int j(0);j<framelen;j++){
			xtemp+=sgolaymat[i][j]*x_unfilt[x_unfilt.size()-1-j];
		}
		x_filt.push_back(xtemp);
	}
	return x_filt;
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<std::vector<double>> sg7_901;
	std::vector<double> x_unfilt,x_filt;
    std::ifstream ifs("sg7_901.txt");
    int nrow,ncol;
	std::vector<double> tempv;
	double num(0.0);

	//Loading SGOLAY matrix 
	ifs>>nrow;
	ifs>>ncol;

	if(!ifs){
		std::cout<<"sg7_901.txt file not loaded"<<std::endl;
	}

	int k(0);
    while (ifs >> num) {
        tempv.push_back(num);
		k++;
		if(k==ncol){
			sg7_901.push_back(tempv);
			tempv.clear();
			k=0;
		}
    }
	ifs.close();
        
	//Loading data to filter
	ifs.open("x_unfilt.txt");
	if(!ifs){
		std::cout<<"x_unfilt.txt file not loaded"<<std::endl;
	}
	while (ifs >> num) {
        x_unfilt.push_back(num);
		}
    
	//Filtering
	std::cout<<"start filtering"<<std::endl;
	x_filt=sgolay(x_unfilt,sg7_901);


	//write x_filt in a file
	std::ofstream myfile ("x_filt.txt");
  if (myfile.is_open())
  {
	for(int i(0);i<x_filt.size();i++){
		myfile << x_filt[i]<<std::endl;
	}
	myfile.close();
  }
  else std::cout << "Unable to open file";

  std::cout<<"end filtering"<<std::endl;

	std::getchar();
}

