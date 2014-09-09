/*! \file

\brief Convert the MPI native binary file to  ascii file

by Wenqing Wang

12.2013
*/

#include <cmath>
#include <cstdlib>

#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cfloat>

#include <time.h>

using namespace std;


int main(int argc, char* argv[])
{
   stringstream ss;
   string s_buff;
   string fname;
   string fpath; 
   if(argc>1)
   {
       fname = argv[1];
   }
   else //terminal
   {
      cout<<"Input file without extension: "<<endl; 
      getline(cin, s_buff);
      ss.str(s_buff);
	  ss >>  fname;
      ss.clear();
   }
		
   cout<<"Convert the MPI native binary file to  ascii file."<<endl;
	
   clock_t elp_time;
   elp_time = -clock();

   size_t pos_end;
   pos_end = fname.find_last_of('\\');
   //
   if(pos_end != std::string::npos)
   {
      fpath = fname.substr(0,pos_end) + "\\";
   }
   else
   {
      pos_end = fname.find_last_of('/');
      if(pos_end != std::string::npos)
         fpath = fname.substr(0,pos_end) + "/";
   }

   // Read header file
   string ifname = fname + "_" + "domain_" + "node_value_header.txt";
   ifstream is(ifname.c_str());
   if(!is.good())
   {
	   cout << "File "<<fname<<" is not found."<<endl;
	   exit(1);
   }

   cout<<"File path "<< fpath << endl;


   int msize_o; // Original processor number
   is >> msize_o >> ws;

   int nsteps;  // Number of time steps 
   is >> nsteps >> ws;

   string pcs_name;
   int nvars;
   size_t data_size;
   vector<string> variable_names;

   is >> pcs_name;

   is >> nvars >> ws;

   variable_names.resize(nvars);

   for(size_t j=0; j<nvars; j++)
   {
       is >> variable_names[j];	  
   }
   is >> ws;  
      
   is >>  data_size >> ws;

   is.close();

   // Read header file
   ifname = fname+ "_" + "domain_variables" + ".bin";
   double *time_data = new double[msize_o];
   double *value = new double[data_size];


   is.open(ifname.c_str(), ios::binary);

   ofstream os;
   os.setf(ios::scientific, ios::floatfield);
   setw(14);
   os.precision(14);

   string ofname = ""; 
   // Convert int to string

   for(int k=0; k<nsteps; ++k)
   {
      is.read((char*)(time_data), msize_o*sizeof(double));

      // Convert double to string
      string str_time;
      stringstream d2s;
      d2s << time_data[0];
      d2s >> str_time;
      d2s.clear();

      ofname = fname +"_"+ str_time+".vtk";

      cout<<"\nWrite time step at "<<time_data[0]<<" to file "<< ofname << endl;

      os.open(ofname.c_str(), ios::trunc);
     
      // os << "POINT_DATA " << data_size << endl;

      for(size_t j=0; j<nvars; j++)
      {
          is.read((char*)(value), data_size*sizeof(double));

          os << "SCALARS "<<variable_names[j] <<" double 1"<<endl;
          os << "LOOKUP_TABLE default"<< endl;

          cout<<"Convert "<<variable_names[j]<<endl;
	  for(size_t i=0; i<data_size; i++)
          {
	      os << value[i] << "\n";
          }
          os.flush();	  
	  os << endl;
           
          if(is.eof())
          {
             break;
          }
      }
      os.clear();
      os.close();
   }

   is.close();

   elp_time += clock();
   cout<<"\n***Total CPU time elapsed: "
       <<(double)elp_time / CLOCKS_PER_SEC<<"s"<<endl;

  delete [] time_data;
  delete [] value;

  return 0;
}
